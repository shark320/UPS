package com.vpavlov.ups.reversi.client.service.api.state

import androidx.compose.runtime.State
import com.vpavlov.ups.reversi.client.state.ClientFlowState
import com.vpavlov.ups.reversi.client.state.ClientState

interface ClientStateService {

    fun getState(): State<ClientState?>

    fun updateState(
        username: String = getState().value!!.username,
        flowState: ClientFlowState = getState().value!!.flowState
    )

    fun initState(
        username: String,
        flowState: ClientFlowState,

    )
}