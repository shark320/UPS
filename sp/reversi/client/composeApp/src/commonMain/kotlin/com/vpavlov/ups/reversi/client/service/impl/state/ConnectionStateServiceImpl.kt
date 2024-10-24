package com.vpavlov.ups.reversi.client.service.impl.state

import androidx.compose.runtime.State
import androidx.compose.runtime.mutableStateOf
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.state.ConnectionState
import io.ktor.network.sockets.Socket
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.map

open class ConnectionStateServiceImpl : ConnectionStateService {

    protected val _state = MutableStateFlow(ConnectionState())
    protected val state: StateFlow<ConnectionState> = _state.asStateFlow()

    @Synchronized
    override fun getConnectionState() = state

    @Synchronized
    override fun updateConnectionState(
        isAlive: Boolean, lastPing: Long?, socket: Socket?
    ) {
        _state.value = state.value.copy(
            isAlive = isAlive,
            lastPing = lastPing,
            socket = socket
        )
    }

    @Synchronized
    override fun isAliveFLow(): Flow<Boolean> = state.map { value ->
        value.isAlive && value.socket != null
    }

}