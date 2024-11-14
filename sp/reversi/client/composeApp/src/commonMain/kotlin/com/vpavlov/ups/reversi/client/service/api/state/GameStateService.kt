package com.vpavlov.ups.reversi.client.service.api.state

import com.vpavlov.ups.reversi.client.domains.game.Game
import com.vpavlov.ups.reversi.client.domains.game.MoveCoordinates
import com.vpavlov.ups.reversi.client.domains.game.Player
import com.vpavlov.ups.reversi.client.state.GameState
import kotlinx.coroutines.flow.StateFlow

interface GameStateService {

    fun getStateFlow(): StateFlow<GameState?>

    fun updateState(
        game: Game = getStateFlow().value!!.game,
        players: List<Player> = getStateFlow().value!!.players,
        isOpponentConnected: Boolean = getStateFlow().value!!.isOpponentConnected,
        currentPlayer: Player = getStateFlow().value!!.currentPlayer,
        lastMoveCoordinates: MoveCoordinates = getStateFlow().value!!.lastMoveCoordinates
    )

    fun initState(
        game: Game,
        players: List<Player>,
        isOpponentConnected: Boolean,
        currentPlayer: Player,
        lastMoveCoordinates: MoveCoordinates
    )
}