package com.vpavlov.ups.reversi.client.presentation.connection

sealed interface ConnectionScreenEvent {

    data object Reconnect: ConnectionScreenEvent
}