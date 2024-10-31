package com.vpavlov.ups.reversi.client.presentation.lobby

sealed interface LobbyScreenEvent {

    data object ExitLobby: LobbyScreenEvent
}