package com.vpavlov.ups.reversi.client.service.processor

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.domains.connection.message.Header
import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.domains.connection.message.Payload
import com.vpavlov.ups.reversi.client.domains.connection.message.Status
import com.vpavlov.ups.reversi.client.domains.connection.message.Subtype
import com.vpavlov.ups.reversi.client.domains.connection.message.Type
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.UserMessageStateService
import com.vpavlov.ups.reversi.client.state.UserMessage

class ExitLobbyProcessor(
    private val config: ConnectionConfig,
    clientStateService: ClientStateService,
    connectionService: ConnectionService,
    userMessageStateService: UserMessageStateService
): CommonClientProcessor(
    connectionService = connectionService,
    userMessageStateService = userMessageStateService,
    clientStateService = clientStateService,
) {

    operator fun invoke() = process{
        LOGGER.debug("Processing exiting the current lobby.")
        val requestHeader = Header(
            type = Type.POST,
            identifier = config.identifier,
            subtype = Subtype.LOBBY_EXIT
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
        LOGGER.trace("Handle error: $response")
        val status = response.header.status
        if (status == Status.BAD_REQUEST){
            userMessageStateService.showError(
                userMessage = UserMessage(
                    message = "Exit the lobby is not possible."
                )
            )
            getAndUpdateState(response)
        } else{
            unexpectedErrorStatus(
                response.header.status,
            )
        }
    }

    private fun handleOk(response: Message) {
        LOGGER.trace("Handle ok: $response")
        getAndUpdateState(response)
    }
}