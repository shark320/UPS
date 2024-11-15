package com.vpavlov.ups.reversi.client.presentation.game

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.Button
import androidx.compose.material3.MaterialTheme.colorScheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.navigation.NavHostController
import com.vpavlov.ups.reversi.client.presentation.common.component.ClientFlowStateAwareness
import com.vpavlov.ups.reversi.client.presentation.common.component.ConnectionStateListenerWrapper
import org.koin.compose.viewmodel.koinNavViewModel
import org.koin.compose.viewmodel.koinViewModel

@Composable
fun GameScreen(
    navController: NavHostController,
    viewModel: GameScreenViewModel = koinViewModel()
){

    val state = viewModel.state.value
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
            Button(
                onClick = { navController.navigateUp() },
            ) {
                Text(text = "Back")
            }
        }
    }
}