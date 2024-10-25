package com.vpavlov.ups.reversi.client.service.api.state

import com.vpavlov.ups.reversi.client.state.ConnectionState
import io.ktor.network.sockets.Socket

import androidx.compose.runtime.State
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.StateFlow

interface ConnectionStateService {

    fun getConnectionState(): StateFlow<ConnectionState>

    fun updateConnectionState(
        isAlive: Boolean = getConnectionState().value.isAlive,
        lastPing: Long? = getConnectionState().value.lastPing,
        socket: Socket? = getConnectionState().value.socket,
        isHandshake: Boolean = getConnectionState().value.isHandshake
    )

    fun connectionLost()

    fun isAliveFLow(): Flow<Boolean>

    fun isAlive(): Boolean

    fun isAliveAndHandshake(): Boolean

    fun isAliveAndHandshakeFlow(): Flow<Boolean>
}