package com.vpavlov.ups.reversi.client.presentation.menu

interface MenuScreenEvent {

    data class ConnectToLobby(val lobbyName: String): MenuScreenEvent

    data class CreateNewLobby(val lobbyName: String): MenuScreenEvent

    data class LobbyNameEntered(val lobbyName: String): MenuScreenEvent

    data object LobbyNameInputCancelled: MenuScreenEvent

    data object Logout: MenuScreenEvent
}