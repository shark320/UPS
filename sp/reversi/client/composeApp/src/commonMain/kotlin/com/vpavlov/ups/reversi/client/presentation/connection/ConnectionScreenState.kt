package com.vpavlov.ups.reversi.client.presentation.connection

data class ConnectionScreenState(
    val isAliveAndHandshake: Boolean = false,
    val isHandshakeStarted: Boolean = false
)