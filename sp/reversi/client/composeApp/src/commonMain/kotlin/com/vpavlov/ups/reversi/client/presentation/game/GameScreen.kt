package com.vpavlov.ups.reversi.client.presentation.game

import androidx.compose.foundation.BorderStroke
import androidx.compose.foundation.background
import androidx.compose.foundation.border
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.KeyboardArrowRight
import androidx.compose.material.icons.filled.AccountCircle
import androidx.compose.material.icons.filled.KeyboardArrowRight
import androidx.compose.material.icons.filled.Star
import androidx.compose.material3.Button
import androidx.compose.material3.ButtonDefaults
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.Icon
import androidx.compose.material3.MaterialTheme.colorScheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.navigation.NavHostController
import com.vpavlov.ups.reversi.client.domains.game.MoveCoordinates
import com.vpavlov.ups.reversi.client.domains.game.Player
import com.vpavlov.ups.reversi.client.domains.game.PlayerCode
import com.vpavlov.ups.reversi.client.game.Board
import com.vpavlov.ups.reversi.client.presentation.common.component.ClientFlowStateAwareness
import com.vpavlov.ups.reversi.client.presentation.common.component.ConfirmationDialog
import com.vpavlov.ups.reversi.client.presentation.common.component.ConnectionStateListenerWrapper
import com.vpavlov.ups.reversi.client.presentation.common.component.HandleMessages
import com.vpavlov.ups.reversi.client.presentation.common.component.OnTopCircularProgressIndicator
import com.vpavlov.ups.reversi.client.presentation.common.component.WaitingScreenAwareness
import com.vpavlov.ups.reversi.client.presentation.lobby.LobbyScreenEvent
import com.vpavlov.ups.reversi.client.ui.theme.defaultCornerRadius
import com.vpavlov.ups.reversi.client.utils.requireAllNotNull
import org.koin.compose.viewmodel.koinViewModel

private val possibleMoveColor = Color.Magenta.copy(alpha = 0.15f)
private val commonBoardCellColor = Color.Green.copy(alpha = 0.5f)
private val blackPlayerColor = Color.Black.copy(alpha = 0.8f)
private val whitePlayerColor = Color.White.copy(alpha = 0.8f)

@Composable
fun GameScreen(
    navController: NavHostController,
    viewModel: GameScreenViewModel = koinViewModel()
) {
    var isConfirmationMessageVisible by remember { mutableStateOf(false) }
    val state = viewModel.state.value

    WaitingScreenAwareness(viewModel = viewModel)
    HandleMessages(viewModel)
    ClientFlowStateAwareness(
        viewModel = viewModel,
        navController = navController
    )
    ConnectionStateListenerWrapper(
        viewModel = viewModel,
        navController = navController
    )

    OnTopCircularProgressIndicator(
        show = state.board == null,
        text = "Loading game data..."
    )

    OnTopCircularProgressIndicator(
        show = !state.isOpponentConnected,
        text = "Opponent is offline..."
    ) {
        Button(
            onClick = {
                isConfirmationMessageVisible = true
            },
            colors = ButtonDefaults.outlinedButtonColors(
                contentColor = Color.Red,
            ),
            border = BorderStroke(2.dp, Color.Red),
        ){
            Text(text = "Leave")
        }
    }


    Box(
        contentAlignment = Alignment.Center,
        modifier = Modifier
            .fillMaxSize()
            .background(colorScheme.background)
    ) {
        Column(
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            Text(text = "Game", fontSize = 25.sp)
            PlayersList(state = viewModel.state.value)
            Spacer(modifier = Modifier.height(24.dp))
            GameBoard(
                viewModel = viewModel
            )
            Button(
                onClick = {
                    isConfirmationMessageVisible = true
                },
                colors = ButtonDefaults.outlinedButtonColors(
                    contentColor = Color.Red,
                ),
                border = BorderStroke(2.dp, Color.Red),
            ) {
                Text(text = "Leave")
            }
        }
    }

    if (isConfirmationMessageVisible){
        ConfirmationDialog(
            title = "Leave the Game Confirmation",
            message = "Are You sure you want to leave the game?",
            onOkClick = {
                isConfirmationMessageVisible = false
                viewModel.onEvent(GameScreenEvent.LeaveGame)
            },
            onCancelClick = {
                isConfirmationMessageVisible = false
            }
        )
    }
}

@Composable
private fun GameBoard(viewModel: GameScreenViewModel) {
    val state = viewModel.state.value
    val board = state.board
    val possibleMoves = state.possibleMoves
    if (board == null || possibleMoves == null) {
        return
    }
    Column {
        for (y in 0..<board.rows) {
            Row {
                for (x in 0..<board.cols) {
                    val cellCode = board.getAt(x, y)
                    val isPossibleMove = possibleMoves[y * board.cols + x]
                    GameBoardCell(
                        cellCode,
                        isPossibleMove,
                        onClick = {
                            if (isPossibleMove) {
                                viewModel.onEvent(
                                    GameScreenEvent.PlayerMove(
                                        moveCoordinates = MoveCoordinates(
                                            x = x,
                                            y = y
                                        )
                                    )
                                )
                            }
                        }
                    )
                }
            }
        }
    }
}


@Composable
private fun GameBoardCell(playerCode: PlayerCode, isPossibleMove: Boolean, onClick: () -> Unit) {
    val cellSize = 40.dp
    Box(
        modifier = Modifier.background(commonBoardCellColor).size(cellSize)
            .clickable(onClick = onClick)
            .border(1.dp, Color.DarkGray),
        contentAlignment = Alignment.Center
    ) {
        when (playerCode) {
            PlayerCode.BLACK_PLAYER -> {
                ColoredCircle(
                    color = blackPlayerColor,
                    size = 20.dp
                )
            }

            PlayerCode.WHITE_PLAYER -> {
                ColoredCircle(
                    color = whitePlayerColor,
                    size = 20.dp
                )
            }

            PlayerCode.NO_PLAYER -> {
                if (isPossibleMove) {
                    Box(
                        modifier = Modifier.background(possibleMoveColor).size(cellSize)
                            .border(1.dp, Color.DarkGray),
                    )
                }
            }
        }
    }
}

@Composable
private fun ColoredCircle(
    color: Color,
    size: Dp
) {
    Box(
        modifier = Modifier
            .size(size)
            .background(color, CircleShape)
    )
}

@Composable
private fun PlayersList(state: GameScreenState) {
    val playersList = state.players
    val currentPlayer = state.currentPlayer
    if (!requireAllNotNull(currentPlayer, playersList)) {
        return
    }
    val player1 = playersList!![0]
    val player2 = playersList[1]
    Column(
        modifier = Modifier.fillMaxWidth(0.5f),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Text("Players", fontSize = 20.sp)
        Spacer(modifier = Modifier.height(8.dp))
        Card(
            shape = RoundedCornerShape(defaultCornerRadius),
            modifier = Modifier
                .border(
                    width = 1.dp, // Set the border width
                    color = Color.Blue.copy(alpha = 0.9f), // Set the border color
                    shape = RoundedCornerShape(defaultCornerRadius) // Apply the same shape as the card
                )
                .fillMaxWidth(),
            colors = CardDefaults.elevatedCardColors(),
        ) {
            Column(
                modifier = Modifier.padding(16.dp)
            ) {
                DisplayPlayer(player = player1, player1 == currentPlayer, number = 1)
                Spacer(modifier = Modifier.height(8.dp))
                DisplayPlayer(player = player2, player2 == currentPlayer, number = 2)
            }
        }
    }
}

@Composable
private fun DisplayPlayer(player: Player, isCurrentPlayer: Boolean, number: Int) {
    Row {

        Text(text = "$number. ${player.username}")
        when (player.code) {
            PlayerCode.BLACK_PLAYER -> {
                Icon(
                    imageVector = Icons.Filled.AccountCircle,
                    contentDescription = null,
                    tint = Color.Black
                )
            }
            PlayerCode.WHITE_PLAYER -> {
                Icon(
                    imageVector = Icons.Filled.AccountCircle,
                    contentDescription = null,
                    tint = Color.White
                )
            }
            PlayerCode.NO_PLAYER -> {}
        }
        if (isCurrentPlayer) {
            Spacer(modifier = Modifier.width(4.dp))
            Text(
                text = "(On Turn)",
                color = colorScheme.onSurface.copy(alpha = 0.5f)
            )
        }


    }
}