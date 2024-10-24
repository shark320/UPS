package com.vpavlov.ups.reversi.client.service.impl.state

import androidx.compose.runtime.State
import androidx.compose.runtime.mutableStateOf
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.state.ClientFlowState
import com.vpavlov.ups.reversi.client.state.ClientState

open class ClientStateServiceImpl: ClientStateService {

    protected val _state = mutableStateOf<ClientState?>(null)
    protected val state: State<ClientState?> = _state

    @Synchronized
    override fun getState(): State<ClientState?>  = state

    @Synchronized
    override fun updateState(
        username: String,
        flowState: ClientFlowState
    ) {
        _state.value = state.value!!.copy(
            username = username,
            flowState = flowState
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