package com.vpavlov.ups.reversi.client.service.processor

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.domains.connection.message.Header
import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.domains.connection.message.Payload
import com.vpavlov.ups.reversi.client.domains.connection.message.Status
import com.vpavlov.ups.reversi.client.domains.connection.message.Subtype
import com.vpavlov.ups.reversi.client.domains.connection.message.Type
import com.vpavlov.ups.reversi.client.domains.game.Lobby
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.service.processor.common.CommonClientProcessor
import com.vpavlov.ups.reversi.client.service.processor.common.CommonProcessor
import com.vpavlov.ups.reversi.client.state.ClientFlowState
import com.vpavlov.ups.reversi.client.state.ErrorMessage
import com.vpavlov.ups.reversi.client.utils.requireAllNotNull

class GetLobbyStateProcessor(
    private val config: ConnectionConfig,
    clientStateService: ClientStateService,
    connectionService: ConnectionService,
    errorStateService: ErrorStateService
): CommonClientProcessor(
    connectionService = connectionService,
    errorStateService = errorStateService,
    clientStateService = clientStateService
) {

    operator fun invoke() = process {
        LOGGER.debug("Processing getting current lobby state.")
        val requestHeader = Header(
            type = Type.GET,
            identifier = config.identifier,
            subtype = Subtype.LOBBY_STATE
        )
        val payload = Payload();
        val response =
            connectionService.exchange(Message(header = requestHeader, payload = payload))
        if (response.isError()) {
            handleError(response)
        } else {
            handleOk(response)
        }
    }

    private fun handleError(response: Message) {
        val status = response.header.status
        if (status == Status.NOT_FOUND) {
            errorStateService.setError(
                errorMessage = ErrorMessage(
                    errorMessage = "No lobby is assigned to the client."
                )
            )
            getAndUpdateState(response)
        } else {
            unexpectedErrorStatus(
                response.header.status,
            )
        }
    }

    private fun handleOk(response: Message) {
        val state = ClientFlowState.getValueOrNull(response.payload.getStringValue("state"))
        val host = response.payload.getStringValue("host")
        val lobby = response.payload.getStringValue("lobby")
        val players = response.payload.getListOfStrings("players")
        if (!requireAllNotNull(state, host, players, lobby)) {
            malformedResponse(
                subtype = response.header.subtype,
            )
            return
        }
        clientStateService.updateState(
            flowState = state!!,
            currentLobby = Lobby(
                host = host!!,
                players = players!!,
                name = lobby!!
            )
        )

    }
}