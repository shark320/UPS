package com.vpavlov.ups.reversi.client.service.api.state

import com.vpavlov.ups.reversi.client.state.ConnectionState
import io.ktor.network.sockets.Socket

import androidx.compose.runtime.State
interface ConnectionStateService {

    fun getConnectionState(): State<ConnectionState>

    fun updateConnectionState(
        isAlive: Boolean = getConnectionState().value.isAlive,
        lastPing: Long? = getConnectionState().value.lastPing,
        socket: Socket? = getConnectionState().value.socket
    )

    fun isAlive(): Boolean
}