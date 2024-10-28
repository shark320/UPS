package com.vpavlov.ups.reversi.client.service.impl.message.processors

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.domains.connection.message.Header
import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.domains.connection.message.Subtype
import com.vpavlov.ups.reversi.client.domains.connection.message.Type
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import org.apache.logging.log4j.kotlin.loggerOf

class PingProcessor(
    private val config: ConnectionConfig,
    connectionStateService: ConnectionStateService,
    connectionService: ConnectionService,
    errorStateService: ErrorStateService
): CommonProcessor(
    connectionService = connectionService,
    errorStateService = errorStateService,
    connectionStateService = connectionStateService
) {

    operator fun invoke() = process {
        LOGGER.debug("Processing ping.")
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