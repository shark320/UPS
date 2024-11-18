package com.vpavlov.ups.reversi.client.service.impl.state

import com.vpavlov.ups.reversi.client.domains.game.Game
import com.vpavlov.ups.reversi.client.domains.game.Lobby
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.state.ClientFlowState
import com.vpavlov.ups.reversi.client.state.ClientState
import com.vpavlov.ups.reversi.client.state.LobbyInfo
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow

open class ClientStateServiceImpl: ClientStateService {

    private val _state = MutableStateFlow<ClientState?>(null)
    private val state: StateFlow<ClientState?> = _state.asStateFlow()

    @Synchronized
    override fun getStateFlow(): StateFlow<ClientState?>  = state

    override fun clearClientState() {
        _state.value = null
    }

    override fun clearLogin() {
        if (_state.value == null){
            return
        }
        updateState(
            isLoggedIn = false
        )
    }

    @Synchronized
    override fun updateState(
        isLoggedIn: Boolean,
        username: String,
        flowState: ClientFlowState,
        lobbiesList: List<LobbyInfo>,
        currentLobby: Lobby?,
    ) {
        _state.value = state.value!!.copy(
            isLoggedIn = isLoggedIn,
            username = username,
            flowState = flowState,
            lobbiesList = lobbiesList,
            currentLobby = currentLobby,
        )
    }


    @Synchronized
    override fun initState(username: String, flowState: ClientFlowState) {
        _state.value = ClientState(
            isLoggedIn = true,
            username = username,
            flowState = flowState
        )
    }


}