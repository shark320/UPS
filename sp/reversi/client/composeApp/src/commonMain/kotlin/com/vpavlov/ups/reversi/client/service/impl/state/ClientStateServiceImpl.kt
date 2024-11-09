package com.vpavlov.ups.reversi.client.service.impl.state

import com.vpavlov.ups.reversi.client.domains.game.Lobby
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.state.ClientFlowState
import com.vpavlov.ups.reversi.client.state.ClientState
import com.vpavlov.ups.reversi.client.state.LobbyInfo
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow

open class ClientStateServiceImpl: ClientStateService {

    protected val _state = MutableStateFlow<ClientState?>(null)
    protected val state: StateFlow<ClientState?> = _state.asStateFlow()

    @Synchronized
    override fun getStateFlow(): StateFlow<ClientState?>  = state

    @Synchronized
    override fun updateState(
        username: String,
        flowState: ClientFlowState,
        lobbiesList: List<LobbyInfo>,
        currentLobby: Lobby?
    ) {
        _state.value = state.value!!.copy(
            username = username,
            flowState = flowState,
            lobbiesList = lobbiesList,
            currentLobby = currentLobby
        )
    }


    @Synchronized
    override fun initState(username: String, flowState: ClientFlowState) {
        _state.value = ClientState(
            username = username,
            flowState = flowState
        )
    }


}