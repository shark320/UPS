package com.vpavlov.ups.reversi.client.service.processor

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.domains.connection.message.Header
import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.domains.connection.message.Status
import com.vpavlov.ups.reversi.client.domains.connection.message.Subtype
import com.vpavlov.ups.reversi.client.domains.connection.message.Type
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.state.ClientFlowState
import com.vpavlov.ups.reversi.client.utils.requireAllNotNull

class GetGameStateProcessor(
    private val config: ConnectionConfig,
    clientStateService: ClientStateService,
    connectionService: ConnectionService,
    errorStateService: ErrorStateService
): CommonClientProcessor(
    connectionService = connectionService,
    errorStateService = errorStateService,
    clientStateService = clientStateService,
) {

    operator fun invoke() = process {
        LOGGER.debug("Processing get game state.")
        val requestHeader = Header(
            type = Type.POST,
            identifier = config.identifier,
            subtype = Subtype.GAME_STATE
        )
        val response =
            connectionService.exchange(Message(header = requestHeader))
        if (response.isOk()) {
            handleOk(response)
        } else if (response.isRedirect()) {
            handleRedirect(response)
        } else {
            handleError(response)
        }
    }

    private fun handleRedirect(response: Message) {
        //TODO: redirect
    }

    private fun handleError(response: Message) {
        LOGGER.error("Error response: $response")
        //TODO: error
    }

    private fun handleOk(response: Message) {
        when(response.header.status){
            Status.OK -> handleStateUpdate(response)
            Status.RESET -> handleGameOver(response)
            else -> unexpectedStatus(response)
        }
    }

    private fun handleGameOver(response: Message) {
        //TODO: game over
    }

    private fun handleStateUpdate(response: Message){
        val state = ClientFlowState.getValueOrNull(response.payload.getStringValue("state"))
        val players = response.payload.getListOfStrings("players")
        val playerCodes = response.payload.getListOfStrings("player_codes")
        val isOpponentConnected = response.payload.getBooleanOrNull("opponent_connected")
        val currentPlayer = response.payload.getStringValue("current_player")
        val lastMoveX = response.payload.getIntegerOrNull("x")
        val lastMoveY = response.payload.getIntegerOrNull("y")

        if (!requireAllNotNull(state, players, playerCodes, isOpponentConnected, currentPlayer, lastMoveX, lastMoveY)) {
            malformedResponse(
                subtype = response.header.subtype,
            )
            return
        }

        //TODO: game state update
    }
}