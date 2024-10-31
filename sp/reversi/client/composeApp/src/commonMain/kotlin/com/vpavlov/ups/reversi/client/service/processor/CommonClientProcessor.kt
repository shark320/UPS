package com.vpavlov.ups.reversi.client.service.processor

import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.state.ClientFlowState

abstract class CommonClientProcessor(
    protected val clientStateService: ClientStateService,
    connectionService: ConnectionService,
    errorStateService: ErrorStateService
): CommonProcessor(
    connectionService = connectionService,
    errorStateService = errorStateService,
) {
    protected fun getAndUpdateState(response: Message){
        val state = ClientFlowState.getValueOrNull(response.payload.getStringValue("state"))
        if (state == null) {
            malformedResponse(
                subtype = response.header.subtype,
            )
        } else {
            clientStateService.updateState(flowState = state)
        }
    }
}