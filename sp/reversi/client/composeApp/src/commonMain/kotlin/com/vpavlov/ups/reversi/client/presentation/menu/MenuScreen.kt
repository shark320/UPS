package com.vpavlov.ups.reversi.client.presentation.menu

import androidx.compose.foundation.background
import androidx.compose.foundation.border
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxHeight
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.Button
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.FloatingActionButton
import androidx.compose.material3.MaterialTheme.colorScheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.navigation.NavHostController
import com.vpavlov.ups.reversi.client.presentation.common.component.ClientFlowStateAwareness
import com.vpavlov.ups.reversi.client.presentation.common.component.ConnectionStateListenerWrapper
import com.vpavlov.ups.reversi.client.state.LobbyInfo
import com.vpavlov.ups.reversi.client.ui.theme.defaultCornerRadius
import org.koin.compose.viewmodel.koinViewModel

@Composable
fun MenuScreen(
    navController: NavHostController,
    viewModel: MenuScreenViewModel = koinViewModel()
) {
    val state = viewModel.state.value
    ClientFlowStateAwareness(
        viewModel = viewModel,
        navController = navController
    )
    ConnectionStateListenerWrapper(
        viewModel = viewModel,
        navController = navController
    ) {
        Box(
            modifier = Modifier
                .fillMaxSize()
                .background(colorScheme.background),
            contentAlignment = Alignment.TopCenter,

            ) {
            Column(
                modifier = Modifier.fillMaxWidth(0.5f).fillMaxHeight(),
                horizontalAlignment = Alignment.CenterHorizontally
            ) {
                Text(
                    text = "Open Lobbies",
                    textAlign = TextAlign.Center,
                    fontSize = 35.sp
                )
                LazyColumn(
                    modifier = Modifier.fillMaxWidth().height(400.dp)
                ) {
                    items(state.lobbies.size) { index ->
                        val lobbyInfo = state.lobbies[index]
                        LobbyCard(
                            lobbyInfo = lobbyInfo,
                            onClick = { lobbyName ->
                                viewModel.onEvent(
                                    MenuScreenEvent.ConnectToLobby(
                                        lobbyName
                                    )
                                )
                            }
                        )
                        Spacer(modifier = Modifier.height(8.dp))
                    }
                }

            }
            FloatingActionButton(
                modifier = Modifier.align(Alignment.BottomCenter).padding(bottom = 30.dp),
                onClick = {},

                ) {
                Text(
                    modifier = Modifier.padding(8.dp),
                    text = "Create new lobby")
            }
        }
    }

}

@Composable
private fun LobbyCard(lobbyInfo: LobbyInfo, onClick: (String) -> Unit) {
    Card(
        shape = RoundedCornerShape(defaultCornerRadius),
        modifier = Modifier
            .border(
                width = 2.dp, // Set the border width
                color = Color.Cyan.copy(alpha = 0.9f), // Set the border color
                shape = RoundedCornerShape(defaultCornerRadius) // Apply the same shape as the card
            )
            .fillMaxWidth(),
        colors = CardDefaults.elevatedCardColors(),
    ) {
        Row(modifier = Modifier.fillMaxWidth().padding(8.dp)) {
            Column(
                modifier = Modifier.weight(1f)
            ) {
                Row {
                    Text("Lobby:")
                    Spacer(modifier = Modifier.width(4.dp))
                    Text(lobbyInfo.lobbyName)
                }
                Spacer(modifier = Modifier.height(8.dp))
                Row {
                    Text("Host:")
                    Spacer(modifier = Modifier.width(4.dp))
                    Text(lobbyInfo.lobbyHost)
                }
            }

            Button(
                onClick = { onClick(lobbyInfo.lobbyName) },
            ) {
                Text(text = "Connect")
            }
        }
    }
}