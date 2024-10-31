package com.vpavlov.ups.reversi.client.presentation.lobby

import androidx.compose.runtime.MutableState
import androidx.compose.runtime.mutableStateOf
import com.vpavlov.ups.reversi.client.presentation.common.viewModel.CommonScreenViewModel
import com.vpavlov.ups.reversi.client.service.api.PingService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.state.ClientState

class LobbyScreenViewModel(
    clientStateService: ClientStateService,
    connectionStateService: ConnectionStateService,
    errorStateService: ErrorStateService,
    private val pingService: PingService
) : CommonScreenViewModel<LobbyScreenEvent, LobbyScreenState>(
    errorStateService = errorStateService,
    connectionStateService = connectionStateService,
    clientStateService = clientStateService
) {

    init {
        pingService.start()
    }

    override fun onEvent(event: LobbyScreenEvent) {

    }

    override fun handleClientStateUpdateCst(clientState: ClientState?) {
        if (clientState != null){
            _state.value = state.value.copy(
                host = clientState.currentLobby?.host ?: "",
                players = clientState.currentLobby?.players ?: emptyList(),
                username = clientState.username,
                lobby = clientState.currentLobby?.name ?: ""
            )
        }
    }

    override fun initState(): MutableState<LobbyScreenState>  = mutableStateOf(LobbyScreenState())
}