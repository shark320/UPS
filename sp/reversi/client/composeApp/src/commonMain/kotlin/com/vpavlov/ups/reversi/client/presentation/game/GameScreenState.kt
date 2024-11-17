package com.vpavlov.ups.reversi.client.presentation.game

import com.vpavlov.ups.reversi.client.domains.game.Player
import com.vpavlov.ups.reversi.client.game.Board

data class GameScreenState(
    val board: Board? = null,
    val currentPlayer: Player? = null,
    val possibleMoves: List<Boolean>? = null,
    val players: List<Player>? = null,
    val isOpponentConnected: Boolean = true
)
