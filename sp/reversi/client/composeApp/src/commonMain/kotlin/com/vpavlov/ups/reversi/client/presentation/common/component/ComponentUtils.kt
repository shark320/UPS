package com.vpavlov.ups.reversi.client.presentation.common.component

import androidx.compose.runtime.Composable
import androidx.navigation.NavController
import androidx.navigation.NavOptions
import com.vpavlov.ups.reversi.client.presentation.common.viewModel.CommonScreenEvent
import com.vpavlov.ups.reversi.client.presentation.common.viewModel.CommonScreenViewModel
import com.vpavlov.ups.reversi.client.presentation.navigation.ScreenNavigation
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
            }
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