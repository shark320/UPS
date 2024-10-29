package com.vpavlov.ups.reversi.client.service.impl.message.processors

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.domains.connection.message.Header
import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.domains.connection.message.Subtype
import com.vpavlov.ups.reversi.client.domains.connection.message.Type
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.state.ErrorMessage

class HandshakeProcessor(
    private val config: ConnectionConfig,
    connectionService: ConnectionService,
    errorStateService: ErrorStateService
): CommonProcessor(
    connectionService = connectionService,
    errorStateService = errorStateService,
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
        errorStateService.setError(
            errorMessage = ErrorMessage("Could not connect to the server.", okButton = "Reconnect")
        )
        LOGGER.error("Could not connect to the server.", exception)
    }

    private fun handleHandshakeError(){
        connectionService.handshakeError()
        errorStateService.setError(
            errorMessage = ErrorMessage(errorMessage = "Fatal error: could not process handshake.", okButton = "Exit"),
            fatal = true
        )
    }

    private fun handleHandshakeOk() {
        connectionService.handshakePerformed()
    }
}