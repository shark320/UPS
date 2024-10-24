package com.vpavlov.ups.reversi.client.presentation.login

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.material3.Button
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.MaterialTheme.colorScheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.navigation.NavHostController
import com.vpavlov.ups.reversi.client.presentation.common.CustomOutlinedTextField
import com.vpavlov.ups.reversi.client.presentation.common.OnTopCircularProgressIndicator
import org.koin.compose.viewmodel.koinViewModel
import org.koin.core.annotation.KoinExperimentalAPI
import org.koin.core.parameter.parametersOf

@OptIn(KoinExperimentalAPI::class, ExperimentalMaterial3Api::class)
@Composable
fun LoginScreen(
    navController: NavHostController,
    viewModel: LoginViewModel = koinViewModel(
        parameters = {
            parametersOf(navController)
        }
    )
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
                value = viewModel.state.value.username,
                onValueChange = { viewModel.onEvent(LoginEvent.UsernameEntered(it)) },
                isError = viewModel.state.value.usernameError,
                errorMessage = "Username is not valid!"
            )
            Button(
                onClick = { viewModel.onEvent(LoginEvent.ProcessLogin) },
            ) {
                Text(text = "Login")
            }

            OnTopCircularProgressIndicator(show = viewModel.state.value.waitingResponse)
        }

    }
}