package com.vpavlov.ups.reversi.client.presentation.menu

interface MenuScreenEvent {

    data class ConnectToLobby(val lobbyName: String): MenuScreenEvent
}