package com.vpavlov.ups.reversi.client.presentation.game

import androidx.compose.runtime.MutableState
import androidx.compose.runtime.mutableStateOf
import com.vpavlov.ups.reversi.client.domains.game.MoveCoordinates
import com.vpavlov.ups.reversi.client.presentation.common.viewModel.CommonScreenViewModel
import com.vpavlov.ups.reversi.client.presentation.lobby.LobbyScreenEvent
import com.vpavlov.ups.reversi.client.presentation.lobby.LobbyScreenState
import com.vpavlov.ups.reversi.client.service.api.PingService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.UserMessageStateService
import com.vpavlov.ups.reversi.client.service.processor.ExitLobbyProcessor
import com.vpavlov.ups.reversi.client.service.processor.StartGameProcessor

class GameScreenViewModel(
    clientStateService: ClientStateService,
    connectionStateService: ConnectionStateService,
    userMessageStateService: UserMessageStateService,
    private val pingService: PingService,
    private val exitLobbyProcessor: ExitLobbyProcessor,
    private val startGameProcessor: StartGameProcessor
) : CommonScreenViewModel<GameScreenEvent, GameScreenState>(
    userMessageStateService = userMessageStateService,
    connectionStateService = connectionStateService,
    clientStateService = clientStateService
) {

    init {
        pingService.start()
    }

    override fun onEvent(event: GameScreenEvent) {
        when (event) {
            GameScreenEvent.LeaveGame -> {}
            is GameScreenEvent.PlayerMove -> processPlayerMove(event.moveCoordinates)
        }
    }

    private fun processPlayerMove(moveCoordinates: MoveCoordinates) {

    }

    override fun initState(): MutableState<GameScreenState> = mutableStateOf(GameScreenState())
}