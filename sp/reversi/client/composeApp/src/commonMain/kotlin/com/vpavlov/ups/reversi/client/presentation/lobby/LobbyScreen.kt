package com.vpavlov.ups.reversi.client.presentation.lobby

import androidx.compose.foundation.background
import androidx.compose.foundation.border
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Star
import androidx.compose.material3.Button
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.Icon
import androidx.compose.material3.MaterialTheme.colorScheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.navigation.NavHostController
import com.vpavlov.ups.reversi.client.presentation.common.component.ClientFlowStateAwareness
import com.vpavlov.ups.reversi.client.presentation.common.component.ConnectionStateListenerWrapper
import com.vpavlov.ups.reversi.client.ui.theme.defaultCornerRadius
import org.koin.compose.viewmodel.koinViewModel

@Composable
fun LobbyScreen(
    viewModel: LobbyScreenViewModel = koinViewModel(),
    navController: NavHostController
) {
    ClientFlowStateAwareness(
        viewModel = viewModel,
        navController = navController
    )
    ConnectionStateListenerWrapper(
        viewModel = viewModel,
        navController = navController
    ) {
        Content(
            viewModel = viewModel,
            navController = navController
        )
    }

}

@Composable
private fun Content(
    viewModel: LobbyScreenViewModel,
    navController: NavHostController
) {
    val state = viewModel.state.value
    Box(
        contentAlignment = Alignment.Center,
        modifier = Modifier
            .fillMaxSize()
            .background(colorScheme.background)
    ) {
        Column(
            horizontalAlignment = Alignment.CenterHorizontally,
            modifier = Modifier.fillMaxWidth(0.5f)
        ) {
            Text("Lobby: ${state.lobby}", fontSize = 25.sp)
            Spacer(modifier = Modifier.height(24.dp))
            PlayersList(
                state.host,
                state.players
            )
            if (state.username == state.host){
                Button(
                    onClick = {  },
                ) {
                    Text(text = "Start Game")
                }
            }

        }
    }
}

@Composable
private fun PlayersList(
    host: String,
    players: List<String>
) {
    val player1 = players.getOrNull(0)
    val player2 = players.getOrNull(1)
    Text("Players", fontSize = 20.sp)
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
            DisplayPlayer(player = player1, player1 == host, number = 1)
            Spacer(modifier = Modifier.height(8.dp))
            DisplayPlayer(player = player2, player2 == host, number = 2)
        }
    }
}

@Composable
private fun DisplayPlayer(player: String?, isHost: Boolean, number: Int){
    if (player == null){
        return
    }
    Row{

        Text(text = "$number. $player")
        if (isHost){
            Spacer(modifier = Modifier.width(4.dp))
            Icon(imageVector = Icons.Filled.Star, contentDescription = null)
            Spacer(modifier = Modifier.width(2.dp))
            Text(
                text = "(Host)",
                color = colorScheme.onSurface.copy(alpha = 0.5f)
            )
        }


    }
}