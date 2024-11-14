package com.vpavlov.ups.reversi.client.service.processor

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.domains.connection.message.Header
import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.domains.connection.message.Subtype
import com.vpavlov.ups.reversi.client.domains.connection.message.Type
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.UserMessageStateService
import com.vpavlov.ups.reversi.client.state.UserMessage

class HandshakeProcessor(
    private val config: ConnectionConfig,
    connectionService: ConnectionService,
    userMessageStateService: UserMessageStateService
): CommonProcessor(
    connectionService = connectionService,
    userMessageStateService = userMessageStateService,
) {

    operator fun invoke() = process{
        LOGGER.debug("Processing handshake.")
        val requestHeader = Header(
            type = Type.POST,
            identifier = config.identifier,
            subtype = Subtype.HANDSHAKE
        )
        val response = connectionService.exchange(Message(header = requestHeader))
        if (response.isError()) {
            handleHandshakeError()
        } else {
            handleHandshakeOk()
        }
    }

    override fun onConnectionError(exception: Exception) {
        connectionService.connectionLost()
        userMessageStateService.showError(
            userMessage = UserMessage("Could not connect to the server.", okButton = "Reconnect")
        )
        LOGGER.error("Could not connect to the server.", exception)
    }

    private fun handleHandshakeError(){
        connectionService.handshakeError()
        userMessageStateService.showError(
            userMessage = UserMessage(message = "Fatal error: could not process handshake.", okButton = "Exit"),
            fatal = true
        )
    }

    private fun handleHandshakeOk() {
        connectionService.handshakePerformed()
    }
}