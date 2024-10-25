package com.vpavlov.ups.reversi.client.presentation.common.component

import androidx.compose.runtime.Composable
import androidx.navigation.NavController
import androidx.navigation.NavOptions
import com.vpavlov.ups.reversi.client.presentation.navigation.ScreenNavigation

@Composable
fun ConnectionStateListenerWrapper(
    isConnectionAlive: Boolean,
    navController: NavController,
    content: @Composable () -> Unit
) {

    if (!isConnectionAlive) {
        ErrorDialog(
            message = "Connection to the server lost.",
            onOkClick = {
                navController.navigate(
                    route = ScreenNavigation.ConnectionScreen.toString(),
                    navOptions = NavOptions.Builder().setLaunchSingleTop(true).build()
                )
            }
        )
    }
}