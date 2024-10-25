package com.vpavlov.ups.reversi.client.service.impl.message.processors

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.domains.connection.message.Header
import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.domains.connection.message.Payload
import com.vpavlov.ups.reversi.client.domains.connection.message.Status
import com.vpavlov.ups.reversi.client.domains.connection.message.Subtype
import com.vpavlov.ups.reversi.client.domains.connection.message.Type
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.service.impl.message.malformedResponse
import com.vpavlov.ups.reversi.client.service.impl.message.unexpectedErrorStatus
import com.vpavlov.ups.reversi.client.state.ClientFlowState

class LoginProcessor(
    private val config: ConnectionConfig,
    private val clientStateService: ClientStateService,
    connectionStateService: ConnectionStateService,
    connectionService: ConnectionService,
    errorStateService: ErrorStateService
): CommonProcessor(
    connectionService = connectionService,
    errorStateService = errorStateService,
    connectionStateService = connectionStateService
) {

    operator fun invoke(username: String) = process {
        LOGGER.debug("Processing login with username '$username'")
        val requestHeader = Header(
            type = Type.POST,
            identifier = config.identifier,
            subtype = Subtype.LOGIN
        )
        val payload = Payload();
        payload.setValue("username", username)
        val response =
            connectionService.exchange(Message(header = requestHeader, payload = payload))
        if (response.isError()) {
            handleLoginError(response)
        } else {
            handleLoginOk(response, username = username)
        }
    }

    private fun handleLoginError(response: Message) {
        when (response.header.status) {
            Status.BAD_REQUEST,
            Status.UNAUTHORIZED,
            Status.NOT_FOUND,
            Status.NOT_ALLOWED -> unexpectedErrorStatus(
                response.header.status,
                errorStateService = errorStateService,
                logger = LOGGER
            )

            Status.CONFLICT -> {
                LOGGER.info("Provided username conflict. $response")
                errorStateService.setError("The username is already in use")
            }

            Status.OK, Status.NULL_STATUS -> LOGGER.warn("Could not handle error code")
        }
    }

    private fun handleLoginOk(response: Message, username: String) {
        val responsePayload = response.payload;
        val state = ClientFlowState.getValueOrNull(responsePayload.getStringValue("state"))
        if (state == null) {
            malformedResponse(
                subtype = response.header.subtype,
                errorStateService = errorStateService,
                logger = LOGGER
            )
        } else {
            clientStateService.initState(username = username, flowState = state)
        }
    }
}