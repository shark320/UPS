package com.vpavlov.ups.reversi.client.presentation.menu

import com.vpavlov.ups.reversi.client.state.LobbyInfo

data class MenuScreenState(
    val lobbies: List<LobbyInfo> = emptyList()
)
