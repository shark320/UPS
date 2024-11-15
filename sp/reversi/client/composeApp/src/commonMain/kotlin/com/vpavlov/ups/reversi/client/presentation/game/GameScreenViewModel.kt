package com.vpavlov.ups.reversi.client.presentation.game

import androidx.compose.runtime.MutableState
import androidx.compose.runtime.mutableStateOf
import androidx.lifecycle.viewModelScope
import com.vpavlov.ups.reversi.client.domains.game.MoveCoordinates
import com.vpavlov.ups.reversi.client.presentation.common.viewModel.CommonScreenViewModel
import com.vpavlov.ups.reversi.client.presentation.lobby.LobbyScreenEvent
import com.vpavlov.ups.reversi.client.presentation.lobby.LobbyScreenState
import com.vpavlov.ups.reversi.client.service.api.PingService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.GameStateService
import com.vpavlov.ups.reversi.client.service.api.state.UserMessageStateService
import com.vpavlov.ups.reversi.client.service.processor.ExitLobbyProcessor
import com.vpavlov.ups.reversi.client.service.processor.StartGameProcessor
import com.vpavlov.ups.reversi.client.state.GameState
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach

class GameScreenViewModel(
    clientStateService: ClientStateService,
    connectionStateService: ConnectionStateService,
    userMessageStateService: UserMessageStateService,
    private val pingService: PingService,
    private val exitLobbyProcessor: ExitLobbyProcessor,
    private val startGameProcessor: StartGameProcessor,
    private val gameStateService: GameStateService
) : CommonScreenViewModel<GameScreenEvent, GameScreenState>(
    userMessageStateService = userMessageStateService,
    connectionStateService = connectionStateService,
    clientStateService = clientStateService
) {

    init {
        pingService.start()

        gameStateService.getStateFlow().onEach { gameState ->
            gameStateUpdated(gameState)
        }.launchIn(viewModelScope)
    }

    private fun gameStateUpdated(gameState: GameState?){
        if (gameState == null){
            LOGGER.warn("The updated game state is null!")
            return
        }
        val game = gameState.game
        val possibleMoves = game.getPossibleMoves(gameState.currentPlayer)

        _state.value = state.value.copy(
            currentPlayer = gameState.currentPlayer,
            board = game.board,
            possibleMoves = possibleMoves.asList(),
            players = gameState.players
        )
    }

    override fun onEvent(event: GameScreenEvent) {
        when (event) {
            GameScreenEvent.LeaveGame -> {}
            is GameScreenEvent.PlayerMove -> processPlayerMove(event.moveCoordinates)
        }
    }

    private fun processPlayerMove(moveCoordinates: MoveCoordinates) {
        if (commonScreenState.value.username != state.value.currentPlayer?.username){
            LOGGER.debug("There is opponents player move.")
        }
        LOGGER.debug("Processing player move $moveCoordinates.")
    }

    override fun initState(): MutableState<GameScreenState> = mutableStateOf(GameScreenState())
}