package com.vpavlov.ups.reversi.client.presentation.game

import androidx.compose.foundation.background
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.material3.Button
import androidx.compose.material3.MaterialTheme.colorScheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.size
import androidx.navigation.NavHostController
import com.vpavlov.ups.reversi.client.domains.game.MoveCoordinates
import com.vpavlov.ups.reversi.client.domains.game.PlayerCode
import com.vpavlov.ups.reversi.client.game.Board
import com.vpavlov.ups.reversi.client.presentation.common.component.ClientFlowStateAwareness
import com.vpavlov.ups.reversi.client.presentation.common.component.ConnectionStateListenerWrapper
import org.koin.compose.viewmodel.koinNavViewModel
import org.koin.compose.viewmodel.koinViewModel

private val possibleMoveColor = Color.Magenta.copy(alpha = 0.5f)
private val commonBoardCellColor = Color.Green.copy(alpha = 0.5f)
private val blackPlayerColor = Color.Black.copy(alpha = 0.5f)
private val whitePlayerColor = Color.White.copy(alpha = 0.5f)

@Composable
fun GameScreen(
    navController: NavHostController,
    viewModel: GameScreenViewModel = koinViewModel()
) {


    ClientFlowStateAwareness(
        viewModel = viewModel,
        navController = navController
    )
    ConnectionStateListenerWrapper(
        viewModel = viewModel,
        navController = navController
    )

    Box(
        contentAlignment = Alignment.Center,
        modifier = Modifier
            .fillMaxSize()
            .background(colorScheme.background)
    ) {
        Column(
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            Text(text = "Game mask")
            GameBoard(
                viewModel = viewModel
            )
            Button(
                onClick = { navController.navigateUp() },
            ) {
                Text(text = "Back")
            }
        }
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
    Box {
        for (y in 0..<board.rows) {
            Row {
                for (x in 0..board.cols) {
                    val cellCode = board.getAt(y, x)
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
    val color = if (isPossibleMove) possibleMoveColor else commonBoardCellColor
    Box(
        modifier = Modifier.background(color).size(15.dp).clickable(onClick = onClick),
        contentAlignment = Alignment.Center
    ) {
        when (playerCode) {
            PlayerCode.BLACK_PLAYER -> {
                ColoredCircle(
                    color = blackPlayerColor,
                    size = 10.dp
                )
            }

            PlayerCode.WHITE_PLAYER -> {
                ColoredCircle(
                    color = whitePlayerColor,
                    size = 10.dp
                )
            }

            PlayerCode.NO_PLAYER -> {

            }
        }
    }
}

@Composable
fun ColoredCircle(
    color: Color,
    size: Dp
) {
    Box(
        modifier = Modifier
            .size(size)
            .background(color, CircleShape)
    )
}