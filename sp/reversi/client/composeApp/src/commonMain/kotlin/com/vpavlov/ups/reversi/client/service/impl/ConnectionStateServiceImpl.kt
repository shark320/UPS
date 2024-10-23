package com.vpavlov.ups.reversi.client.service.impl

import androidx.compose.runtime.State
import androidx.compose.runtime.mutableStateOf
import com.vpavlov.ups.reversi.client.service.api.ConnectionStateService
import com.vpavlov.ups.reversi.client.state.ConnectionState
import io.ktor.network.sockets.Socket

class ConnectionStateServiceImpl : ConnectionStateService {

    private val _state = mutableStateOf(ConnectionState())
    private val state: State<ConnectionState> = _state

    @Synchronized
    override fun getConnectionState(): ConnectionState = state.value

    override fun updateConnectionState(

        isAlive: Boolean, lastPing: Long?, socket: Socket?
    ) {
        _state.value = state.value.copy(
            isAlive = isAlive,
            lastPing = lastPing,
            socket = socket
        )
    }

}