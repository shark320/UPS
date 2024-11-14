package com.vpavlov.ups.reversi.client.domains.game

data class Lobby(
    val host: String,
    val players: List<String>,
    val name: String,
)
