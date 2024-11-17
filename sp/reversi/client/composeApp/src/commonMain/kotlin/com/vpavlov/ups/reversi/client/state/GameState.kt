package com.vpavlov.ups.reversi.client.state

import com.vpavlov.ups.reversi.client.domains.game.Game
import com.vpavlov.ups.reversi.client.domains.game.MoveCoordinates
import com.vpavlov.ups.reversi.client.domains.game.Player

data class GameState(
    val game: Game,
    val players: List<Player>,
    val isOpponentConnected: Boolean,
    val currentPlayer: Player,
//    val lastMoveCoordinates: MoveCoordinates
)
