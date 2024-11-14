package com.vpavlov.ups.reversi.client.presentation.game

import com.vpavlov.ups.reversi.client.domains.game.Game

data class GameScreenState(
    val currentPlayerUsername: String? = null,
    val game: Game? = null
)
