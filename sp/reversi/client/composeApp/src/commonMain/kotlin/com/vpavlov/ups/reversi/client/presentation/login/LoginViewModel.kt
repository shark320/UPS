package com.vpavlov.ups.reversi.client.presentation.login

import androidx.compose.runtime.State
import androidx.compose.runtime.mutableStateOf
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import androidx.navigation.NavHostController
import com.vpavlov.ups.reversi.client.di.koin
import com.vpavlov.ups.reversi.client.presentation.common.viewModel.CustomViewModel
import com.vpavlov.ups.reversi.client.service.api.MessageService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.utils.isValidUsername
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach

class LoginViewModel(
    private val navController: NavHostController,
    private val messageService: MessageService,
    private val clientStateService: ClientStateService,
    errorStateService: ErrorStateService
): CustomViewModel(
    errorStateService = errorStateService
) {

    private val _state = mutableStateOf(LoginState())
    val state: State<LoginState> = _state

    init{
        clientStateService.getStateFlow().onEach{ clientState ->
            clientState?.let{
                _state.value = state.value.copy(
                    loggedIn = true,
                    waitingResponse = false
                )
            }
        }.launchIn(viewModelScope)
    }

    fun onEvent(event: LoginEvent){
        when(event){
            is LoginEvent.UsernameEntered -> usernameEntered(event.username)
            LoginEvent.ProcessLogin -> processLogin()
        }
    }

    private fun usernameEntered(username: String){
        var error = false
        var validUsername = true
        if (username.isNotEmpty() && !isValidUsername(username)){
            error = true
            validUsername = false
        }
        _state.value = state.value.copy(
            username = username,
            usernameError = error,
            validUsername = validUsername
            )
    }

    private fun processLogin() {
        messageService.processLogin(state.value.username).onEach { isComplete ->
            _state.value = state.value.copy(waitingResponse = !isComplete)
        }.launchIn(viewModelScope)
    }
}