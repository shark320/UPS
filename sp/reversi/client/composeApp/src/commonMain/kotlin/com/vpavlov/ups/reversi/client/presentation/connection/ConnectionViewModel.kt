package com.vpavlov.ups.reversi.client.presentation.connection

import androidx.compose.runtime.State
import androidx.compose.runtime.mutableStateOf
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.vpavlov.ups.reversi.client.di.koin
import com.vpavlov.ups.reversi.client.presentation.common.viewModel.CustomViewModel
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.MessageService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach

class ConnectionViewModel(
    errorStateService: ErrorStateService,
    private val connectionStateService: ConnectionStateService,
    private val connectionService: ConnectionService,
    private val messageService: MessageService
): CustomViewModel(
    errorStateService = errorStateService
) {

    private val _state = mutableStateOf(ConnectionScreenState())
    val state: State<ConnectionScreenState> = _state

    init {
        connectionService.connect()

        connectionStateService.isAliveFLow().onEach { isAlive ->
            if (isAlive && !state.value.isHandshakeStarted){
                _state.value = state.value.copy(isHandshakeStarted = true)
                messageService.processHandshake()
            }
        }.launchIn(viewModelScope)

        connectionStateService.isAliveAndHandshakeFlow().onEach {
            _state.value = state.value.copy(isAliveAndHandshake = it)
        }.launchIn(viewModelScope)
    }
}