package com.vpavlov.ups.reversi.client.state

import io.ktor.network.sockets.Socket

data class ConnectionState(
    val isAlive: Boolean = false,
    val lastPing: Long? = null,

    val socket: Socket? = null,
    val isHandshake: Boolean = false
)
