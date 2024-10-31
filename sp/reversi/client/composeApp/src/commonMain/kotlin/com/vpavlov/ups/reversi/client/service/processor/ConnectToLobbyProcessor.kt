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
import com.vpavlov.ups.reversi.client.state.ClientFlowState
import com.vpavlov.ups.reversi.client.state.ErrorMessage
import com.vpavlov.ups.reversi.client.utils.requireAllNotNull

class ConnectToLobbyProcessor(
    private val config: ConnectionConfig,
    private val clientStateService: ClientStateService,
    connectionService: ConnectionService,
    errorStateService: ErrorStateService
) : CommonProcessor(
    connectionService = connectionService,
    errorStateService = errorStateService,
) {

    operator fun invoke(lobby: String) = process {
        LOGGER.debug("Processing connecting to he lobby with username '$lobby'")
        val requestHeader = Header(
            type = Type.POST,
            identifier = config.identifier,
            subtype = Subtype.LOBBY_CONNECT
        )
        val payload = Payload();
        payload.setValue("lobby", lobby)
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
                    errorMessage = "The lobby is not available anymore."
                )
            )
            val state = ClientFlowState.getValueOrNull(response.payload.getStringValue("state"))
            if (state == null) {
                malformedResponse(
                    subtype = response.header.subtype,
                )
            } else {
                clientStateService.updateState(flowState = state)
            }
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