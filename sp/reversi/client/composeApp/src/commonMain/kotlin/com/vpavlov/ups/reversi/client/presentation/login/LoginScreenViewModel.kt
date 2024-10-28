package com.vpavlov.ups.reversi.client.presentation.login

import androidx.compose.runtime.MutableState
import androidx.compose.runtime.mutableStateOf
import androidx.lifecycle.viewModelScope
import com.vpavlov.ups.reversi.client.presentation.common.viewModel.CommonScreenViewModel
import com.vpavlov.ups.reversi.client.service.api.MessageService
import com.vpavlov.ups.reversi.client.service.api.PingService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.utils.isValidUsername
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach

class LoginScreenViewModel(
    private val messageService: MessageService,
    private val clientStateService: ClientStateService,
    connectionStateService: ConnectionStateService,
    errorStateService: ErrorStateService,
    private val pingService: PingService
) : CommonScreenViewModel<LoginScreenEvent, LoginScreenState>(
    errorStateService = errorStateService,
    connectionStateService = connectionStateService
) {

    init {
        clientStateService.getStateFlow().onEach { clientState ->
            clientState?.let {
                _state.value = state.value.copy(
                    loggedIn = true,
                    waitingResponse = false
                )
            }
        }.launchIn(viewModelScope)
    }

    override fun onEvent(event: LoginScreenEvent) {
        when (event) {
            is LoginScreenEvent.UsernameEntered -> usernameEntered(event.username)
            LoginScreenEvent.ProcessLoginScreen -> processLogin()
        }
    }

    private fun usernameEntered(username: String) {
        var error = false
        var validUsername = true
        if (username.isNotEmpty() && !isValidUsername(username)) {
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
        pingService.pause()
        messageService.processLogin(state.value.username).onEach { isComplete ->
            _state.value = state.value.copy(waitingResponse = !isComplete)
        }.launchIn(viewModelScope)
    }

    override fun initState(): MutableState<LoginScreenState> = mutableStateOf(LoginScreenState())
}