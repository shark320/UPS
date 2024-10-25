package com.vpavlov.ups.reversi.client.service.api.state

import com.vpavlov.ups.reversi.client.state.ClientFlowState
import com.vpavlov.ups.reversi.client.state.ClientState
import kotlinx.coroutines.flow.StateFlow

interface ClientStateService {

    fun getStateFlow(): StateFlow<ClientState?>

    fun updateState(
        username: String = getStateFlow().value!!.username,
        flowState: ClientFlowState = getStateFlow().value!!.flowState,
    )

    fun initState(
        username: String,
        flowState: ClientFlowState,

    )
}