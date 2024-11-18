package com.vpavlov.ups.reversi.client.presentation.login

import androidx.compose.runtime.MutableState
import androidx.compose.runtime.mutableStateOf
import androidx.lifecycle.viewModelScope
import com.vpavlov.ups.reversi.client.presentation.common.viewModel.CommonScreenViewModel
import com.vpavlov.ups.reversi.client.service.api.PingService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.UserMessageStateService
import com.vpavlov.ups.reversi.client.service.processor.LoginProcessor
import com.vpavlov.ups.reversi.client.utils.isValidUsername
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach

class LoginScreenViewModel(
    private val loginProcessor: LoginProcessor,
    clientStateService: ClientStateService,
    connectionStateService: ConnectionStateService,
    userMessageStateService: UserMessageStateService,
    private val pingService: PingService
) : CommonScreenViewModel<LoginScreenEvent, LoginScreenState>(
    userMessageStateService = userMessageStateService,
    connectionStateService = connectionStateService,
    clientStateService = clientStateService
) {

    private var initializedClientName: String? = null

    init {
        clientStateService.getStateFlow().onEach { clientState ->
            clientState?.let {
                initializedClientName = it.username
                _state.value = state.value.copy(
                    loggedIn = it.isLoggedIn
                )
            }
        }.launchIn(viewModelScope)
        if (initializedClientName != null && !state.value.loggedIn){
            processLogin(initializedClientName)
        }
    }

    override fun onEvent(event: LoginScreenEvent) {
        when (event) {
            is LoginScreenEvent.UsernameEntered -> usernameEntered(event.username)
            LoginScreenEvent.ProcessLoginScreen -> processLogin(state.value.username)
        }
    }

    private fun usernameEntered(username: String) {

        _state.value = state.value.copy(
            username = username,
            usernameError = username.isNotEmpty() && !isValidUsername(username),
            validUsername = isValidUsername(username)
        )
    }

    private fun processLogin(username: String?) {
        if (username == null){
            LOGGER.warn("Could not login, username is null.")
            return
        }
        pingService.stop()
        loginProcessor(username).onEach { isComplete ->
            _state.value = state.value.copy(waitingResponse = !isComplete)
        }.launchIn(viewModelScope)
    }

    override fun initState(): MutableState<LoginScreenState> = mutableStateOf(LoginScreenState())
}