package com.vpavlov.ups.reversi.client.presentation.common.component

import androidx.compose.runtime.Composable
import androidx.navigation.NavController
import androidx.navigation.NavOptions
import com.vpavlov.ups.reversi.client.presentation.common.viewModel.CommonScreenEvent
import com.vpavlov.ups.reversi.client.presentation.common.viewModel.CommonScreenViewModel
import com.vpavlov.ups.reversi.client.presentation.navigation.ScreenNavigation
import com.vpavlov.ups.reversi.client.state.ClientFlowState
import org.koin.core.qualifier._q
import kotlin.system.exitProcess

@Composable
fun ConnectionStateListenerWrapper(
    viewModel: CommonScreenViewModel<*,*>,
    navController: NavController,
    content: @Composable () -> Unit
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
fun HandleErrors(viewModel: CommonScreenViewModel<*,*>, okButtonText: String = "Ok", onOkClick: () -> Unit = {}){
    val errorState = viewModel.commonScreenState.value.errorState
    //TODO: fatal error handling
    val nonFatalReaction =  {
        viewModel.onCommonEvent(CommonScreenEvent.ClearError)
        onOkClick()
    }
    val fatalReaction = { exitProcess(228) }
    if (errorState?.errorMessage != null){
        ErrorDialog(
            message = errorState.errorMessage.errorMessage,
            onOkClick = if (errorState.isFatalError) fatalReaction else nonFatalReaction,
            okButtonText = errorState.errorMessage.okButton
        )
    }
}

@Composable
fun ClientFlowStateAwareness(
    viewModel: CommonScreenViewModel<*,*>,
    navController: NavController,
){
    val state = viewModel.commonScreenState.value
    if (state.clientFlowState == null){
        navigateIfNotTheSame(navController = navController, screenNavigation = ScreenNavigation.LoginScreen)
    }else{
        when(state.clientFlowState){
            ClientFlowState.MENU -> {
                navigateIfNotTheSame(navController = navController, screenNavigation = ScreenNavigation.MenuScreen)
            }
            ClientFlowState.LOBBY -> {
                navigateIfNotTheSame(navController = navController, screenNavigation = ScreenNavigation.LobbyScreen)
            }
            ClientFlowState.GAME -> {
                navigateIfNotTheSame(navController = navController, screenNavigation = ScreenNavigation.GameScreen)
            }
        }
    }

}

@Composable
private fun navigateIfNotTheSame(navController: NavController, screenNavigation: ScreenNavigation){
    if (navController.currentBackStackEntry?.destination?.route != screenNavigation.toString()){
        navController.navigate(
            route = screenNavigation.toString(),
            navOptions = NavOptions.Builder().setLaunchSingleTop(true).build()
        )
    }
}