package com.vpavlov.ups.reversi.client.service.impl.message.processors

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.domains.connection.message.Header
import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.domains.connection.message.Subtype
import com.vpavlov.ups.reversi.client.domains.connection.message.Type
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.service.impl.message.MessageServiceImpl.Companion.LOGGER
import com.vpavlov.ups.reversi.client.service.impl.message.process

class HandshakeProcessor(
    private val config: ConnectionConfig,
    private val connectionStateService: ConnectionStateService,
    private val connectionService: ConnectionService,
    private val errorStateService: ErrorStateService
) {
    operator fun invoke() = process {
        LOGGER.debug("Processing handshake with username.")
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

    private fun handleHandshakeError(){
        connectionStateService.updateConnectionState(isHandshake = false)
        //FATAL error -> show error and terminate execution
    }

    private fun handleHandshakeOk() {
        connectionStateService.updateConnectionState(isHandshake = true)
    }
}