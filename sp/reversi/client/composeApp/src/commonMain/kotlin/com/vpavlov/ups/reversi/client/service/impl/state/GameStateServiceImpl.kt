package com.vpavlov.ups.reversi.client.service.impl.state

import com.vpavlov.ups.reversi.client.domains.game.Game
import com.vpavlov.ups.reversi.client.domains.game.MoveCoordinates
import com.vpavlov.ups.reversi.client.domains.game.Player
import com.vpavlov.ups.reversi.client.service.api.state.GameStateService
import com.vpavlov.ups.reversi.client.state.GameState
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow

class GameStateServiceImpl: GameStateService {

    private val _state = MutableStateFlow<GameState?>(null)
    private val state: StateFlow<GameState?> = _state.asStateFlow()

    @Synchronized
    override fun getStateFlow(): StateFlow<GameState?> = state

    override fun isInitialized(): Boolean  = _state.value != null

    @Synchronized
    override fun updateState(
        players: List<Player>,
        isOpponentConnected: Boolean,
        currentPlayer: Player,
        lastMoveCoordinates: MoveCoordinates,
        boardCells: List<Int>?
    ) {
        //TODO: check if player usernames are sync with the game
        val game = state.value!!.game
//        if (game.isCurrentPlayerChanged(currentPlayer.username)){
//            game.performMoveForCurrentPlayer(lastMoveCoordinates)
//        }
        if (boardCells != null){
            game.updateBoard(boardCells)
        }

        _state.value = state.value!!.copy(
            players = players,
            isOpponentConnected = isOpponentConnected,
            currentPlayer = currentPlayer,
            lastMoveCoordinates = lastMoveCoordinates
        )
    }

    @Synchronized
    override fun initState(
        game: Game,
        players: List<Player>,
        isOpponentConnected: Boolean,
        currentPlayer: Player,
        lastMoveCoordinates: MoveCoordinates
    ) {
        _state.value = GameState(
            game = game,
            players = players,
            isOpponentConnected = isOpponentConnected,
            currentPlayer = currentPlayer,
            lastMoveCoordinates = lastMoveCoordinates
        )
    }
}