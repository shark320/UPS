package com.vpavlov.ups.reversi.client.presentation.common.component

import androidx.compose.runtime.Composable
import androidx.navigation.NavController
import androidx.navigation.NavOptions
import com.vpavlov.ups.reversi.client.presentation.common.viewModel.CommonScreenEvent
import com.vpavlov.ups.reversi.client.presentation.common.viewModel.CommonScreenViewModel
import com.vpavlov.ups.reversi.client.presentation.navigation.ScreenNavigation
import com.vpavlov.ups.reversi.client.state.ClientFlowState
import kotlin.system.exitProcess

@Composable
fun ConnectionStateListenerWrapper(
    viewModel: CommonScreenViewModel<*, *>,
    navController: NavController,
    content: @Composable () -> Unit = {}
) {
    val state = viewModel.commonScreenState.value
    if (!state.isConnectionAlive) {
        ErrorDialog(
            message = "Connection to the server lost.",
            onOkClick = {
                navController.navigate(
                    route = ScreenNavigation.ConnectionScreen.toString(),
                    navOptions = NavOptions.Builder().setLaunchSingleTop(true).build()
                )
            },
        )
    }
    content()
}

@Composable
fun HandleMessages(
    viewModel: CommonScreenViewModel<*, *>,
    okButtonText: String = "Ok",
    onOkClick: () -> Unit = {}
) {
    val messageState = viewModel.commonScreenState.value.messageState
    //TODO: fatal error handling
    val nonFatalReaction = {
        viewModel.onCommonEvent(CommonScreenEvent.ClearError)
        onOkClick()
    }
    val fatalReaction = { exitProcess(228) }
    if (messageState?.userMessage != null) {
        ErrorDialog(
            message = messageState.userMessage.message,
            onOkClick = if (messageState.isFatalError) fatalReaction else nonFatalReaction,
            okButtonText = messageState.userMessage.okButton
        )
    }
}

@Composable
fun ClientFlowStateAwareness(
    viewModel: CommonScreenViewModel<*, *>,
    navController: NavController,
) {
    val state = viewModel.commonScreenState.value
    val messageState = viewModel.commonScreenState.value.messageState
    if (messageState?.userMessage != null) {
        //Prevent other screen navigation in case of message is showing
        return
    }
    if (state.clientFlowState == null) {
        navigateIfNotTheSame(
            navController = navController,
            screenNavigation = ScreenNavigation.LoginScreen
        )
    } else {
        when (state.clientFlowState) {
            ClientFlowState.MENU -> {
                navigateIfNotTheSame(
                    navController = navController,
                    screenNavigation = ScreenNavigation.MenuScreen
                )
            }

            ClientFlowState.LOBBY -> {
                navigateIfNotTheSame(
                    navController = navController,
                    screenNavigation = ScreenNavigation.LobbyScreen
                )
            }

            ClientFlowState.GAME -> {
                navigateIfNotTheSame(
                    navController = navController,
                    screenNavigation = ScreenNavigation.GameScreen
                )
            }
        }
    }

}

@Composable
private fun navigateIfNotTheSame(navController: NavController, screenNavigation: ScreenNavigation) {
    if (navController.currentBackStackEntry?.destination?.route != screenNavigation.toString()) {
        navController.navigate(
            route = screenNavigation.toString(),
            navOptions = NavOptions.Builder().setLaunchSingleTop(true).build()
        )
    }
}

@Composable
fun WaitingScreenAwareness(viewModel: CommonScreenViewModel<*, *>) {
    val state = viewModel.commonScreenState.value
    OnTopCircularProgressIndicator(state.isWaitingScreen, text = state.waitingScreenText)

}