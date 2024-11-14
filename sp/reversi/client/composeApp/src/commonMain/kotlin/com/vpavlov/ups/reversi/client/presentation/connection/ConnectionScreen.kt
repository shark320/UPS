package com.vpavlov.ups.reversi.client.presentation.connection

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.CircularProgressIndicator
import androidx.compose.material3.MaterialTheme.colorScheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.navigation.NavHostController
import androidx.navigation.NavOptions
import com.vpavlov.ups.reversi.client.presentation.common.component.HandleMessages
import com.vpavlov.ups.reversi.client.presentation.navigation.ScreenNavigation
import org.koin.compose.viewmodel.koinViewModel
import org.koin.core.annotation.KoinExperimentalAPI

@OptIn(KoinExperimentalAPI::class)
@Composable
fun ConnectionScreen(
    navController: NavHostController,
    viewModel: ConnectionScreenViewModel = koinViewModel()
) {
//    ConnectionStateListenerWrapper(
//        viewModel = viewModel,
//        navController = navController
//    ) {
    HandleMessages(
        viewModel = viewModel,
        okButtonText = "Reconnect"
    ) {
        viewModel.onEvent(ConnectionScreenEvent.Reconnect)
    }
    Content(
        viewModel = viewModel,
        navController = navController
    )
//    }
}

@Composable
private fun Content(
    navController: NavHostController,
    viewModel: ConnectionScreenViewModel
) {
    if (viewModel.state.value.isAliveAndHandshake) {
        navController.navigate(
            ScreenNavigation.LoginScreen.toString(),
            navOptions = NavOptions.Builder().setLaunchSingleTop(true).build()
        )
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
            CircularProgressIndicator()
            Text(text = "Connection...")
        }

    }
}
