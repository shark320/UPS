package com.vpavlov.ups.reversi.client.presentation.connection

import androidx.compose.runtime.MutableState
import androidx.compose.runtime.mutableStateOf
import androidx.lifecycle.viewModelScope
import com.vpavlov.ups.reversi.client.presentation.common.viewModel.CommonScreenViewModel
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.PingService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.service.processor.HandshakeProcessor
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach

class ConnectionScreenViewModel(
    errorStateService: ErrorStateService,
    connectionStateService: ConnectionStateService,
    private val connectionService: ConnectionService,
    private val handshakeProcessor: HandshakeProcessor,
    private val pingService: PingService,
    clientStateService: ClientStateService
) : CommonScreenViewModel<ConnectionScreenEvent, ConnectionScreenState>(
    errorStateService = errorStateService,
    connectionStateService = connectionStateService,
    clientStateService = clientStateService
) {

    init {
        connectionService.connect()

        connectionStateService.isAliveFLow().onEach { isAlive ->
            if (isAlive && !state.value.isHandshakeStarted) {
                _state.value = state.value.copy(isHandshakeStarted = true)
                handshakeProcessor()
            }
        }.launchIn(viewModelScope)

        connectionStateService.isAliveAndHandshakeFlow().onEach {
            _state.value = state.value.copy(isAliveAndHandshake = it)
            if (it && !pingService.isRunning()){
                pingService.start()
            }
        }.launchIn(viewModelScope)
    }

    override fun onEvent(event: ConnectionScreenEvent) {
        when(event){
            ConnectionScreenEvent.Reconnect -> {
                _state.value = state.value.copy(
                    isHandshakeStarted = false,
                    isAliveAndHandshake = false
                )
                connectionService.connect()
            }
        }
    }

    override fun initState(): MutableState<ConnectionScreenState> =
        mutableStateOf(ConnectionScreenState())
}