package com.vpavlov.ups.reversi.client.presentation.lobby

data class LobbyScreenState(
    val host: String = "",
    val players: List<String> = emptyList(),
    val username: String = "",
    val lobby: String = ""
)
