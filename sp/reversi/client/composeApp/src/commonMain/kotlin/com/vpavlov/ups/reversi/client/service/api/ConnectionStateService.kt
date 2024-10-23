package com.vpavlov.ups.reversi.client.service.api

import com.vpavlov.ups.reversi.client.state.ApplicationState
import com.vpavlov.ups.reversi.client.state.ConnectionState
import io.ktor.network.sockets.Socket

interface ConnectionStateService {

    fun getConnectionState(): ConnectionState

    fun updateConnectionState(
        isAlive: Boolean = getConnectionState().isAlive,
        lastPing: Long? = getConnectionState().lastPing,
        socket: Socket? = getConnectionState().socket
    )
}