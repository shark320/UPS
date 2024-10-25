package com.vpavlov.ups.reversi.client.service.impl.message.processors

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.domains.connection.message.Header
import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.domains.connection.message.Subtype
import com.vpavlov.ups.reversi.client.domains.connection.message.Type
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.service.impl.message.process
import org.apache.logging.log4j.kotlin.loggerOf

class PingProcessor(
    private val config: ConnectionConfig,
    private val connectionStateService: ConnectionStateService,
    private val connectionService: ConnectionService
) {
    companion object {
        private val LOGGER = loggerOf(PingProcessor::class.java)
    }

    operator fun invoke() = process(LOGGER) {

        LOGGER.debug("Processing handshake with username.")
        val requestHeader = Header(
            type = Type.GET,
            identifier = config.identifier,
            subtype = Subtype.PING
        )
        val response = connectionService.exchange(Message(header = requestHeader))
        if (response.isError()) {
            handlePingError(response)
        } else {
            handlePingOk()
        }

    }

    private fun handlePingError(response: Message) {
        LOGGER.error("Handle ping response error status. $response")
    }

    private fun handlePingOk() {
        connectionStateService.updateConnectionState(
            lastPing = System.currentTimeMillis()
        )
    }
}