package com.vpavlov.ups.reversi.client.presentation.connection

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material.CircularProgressIndicator
import androidx.compose.material3.Button
import androidx.compose.material3.MaterialTheme.colorScheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.navigation.NavHostController
import com.vpavlov.ups.reversi.client.presentation.navigation.ScreenNavigation
import com.vpavlov.ups.reversi.client.service.api.ConnectionStateService

@Composable
fun ConnectionScreen(
    navController: NavHostController,
    connectionStateService: ConnectionStateService
) {
    if (connectionStateService.getConnectionState().isAlive){
        navController.navigate(ScreenNavigation.LoginScreen.toString())
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
            Button(
                onClick = { navController.navigate(ScreenNavigation.LoginScreen.toString()) },
            ) {
                Text(text = "Go To Login")
            }
        }

    }
}