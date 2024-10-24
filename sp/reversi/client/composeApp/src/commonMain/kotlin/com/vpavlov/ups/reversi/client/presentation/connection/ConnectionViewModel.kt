package com.vpavlov.ups.reversi.client.presentation.connection

import androidx.compose.runtime.State
import androidx.compose.runtime.mutableStateOf
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.vpavlov.ups.reversi.client.di.koin
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach

class ConnectionViewModel: ViewModel() {

    private val _state = mutableStateOf(ConnectionScreenState())
    val state: State<ConnectionScreenState> = _state

    val connectionStateService: ConnectionStateService = koin.get()

    private val connectionService: ConnectionService = koin.get()

    val connectionState = connectionStateService.getConnectionState()

    init {
        connectionService.connect()
        connectionStateService.isAliveFLow().onEach {
            _state.value = state.value.copy(isAlive = it)
        }.launchIn(viewModelScope)
    }
}