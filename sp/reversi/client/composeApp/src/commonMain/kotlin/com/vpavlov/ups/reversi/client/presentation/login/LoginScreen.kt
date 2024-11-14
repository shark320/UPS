package com.vpavlov.ups.reversi.client.presentation.login

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.material3.Button
import androidx.compose.material3.MaterialTheme.colorScheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.navigation.NavHostController
import com.vpavlov.ups.reversi.client.presentation.common.component.ClientFlowStateAwareness
import com.vpavlov.ups.reversi.client.presentation.common.component.ConnectionStateListenerWrapper
import com.vpavlov.ups.reversi.client.presentation.common.component.CustomOutlinedTextField
import com.vpavlov.ups.reversi.client.presentation.common.component.HandleMessages
import com.vpavlov.ups.reversi.client.presentation.common.component.OnTopCircularProgressIndicator
import org.koin.compose.viewmodel.koinViewModel
import org.koin.core.annotation.KoinExperimentalAPI

@OptIn(KoinExperimentalAPI::class)
@Composable
fun LoginScreen(
    navController: NavHostController,
    viewModel: LoginScreenViewModel = koinViewModel()
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
            contentAlignment = Alignment.Center,
            modifier = Modifier
                .fillMaxSize()
                .background(colorScheme.background)
        ) {
            Column(
                horizontalAlignment = Alignment.CenterHorizontally
            ) {
                Text(text = "Enter your username:")
                CustomOutlinedTextField(
                    modifier = Modifier
                        .fillMaxWidth(0.3f),
                    label = { Text(text = "Username") },
                    value = state.username,
                    onValueChange = { viewModel.onEvent(LoginScreenEvent.UsernameEntered(it)) },
                    isError = state.usernameError,
                    errorMessage = "Username is not valid!"
                )
                Button(
                    onClick = { viewModel.onEvent(LoginScreenEvent.ProcessLoginScreen) },
                    enabled = state.validUsername
                ) {
                    Text(text = "Login")
                }


            }
        }

        OnTopCircularProgressIndicator(show = state.waitingResponse)

        HandleMessages(viewModel)
    }

}