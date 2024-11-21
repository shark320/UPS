package com.vpavlov.ups.reversi.client.service.processor

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.domains.connection.message.Header
import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.domains.connection.message.StatusGroup
import com.vpavlov.ups.reversi.client.domains.connection.message.Subtype
import com.vpavlov.ups.reversi.client.domains.connection.message.Type
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.UserMessageStateService

class LogoutProcessor(
    private val config: ConnectionConfig,
    clientStateService: ClientStateService,
    connectionService: ConnectionService,
    userMessageStateService: UserMessageStateService
): CommonClientProcessor(
    connectionService = connectionService,
    userMessageStateService = userMessageStateService,
    clientStateService = clientStateService
) {

    operator fun invoke() = process {
        LOGGER.debug("Processing logout")
        val requestHeader = Header(
            type = Type.POST,
            identifier = config.identifier,
            subtype = Subtype.LOGOUT
        )
        val response =
            connectionService.exchange(Message(header = requestHeader))
        LOGGER.debug("Response: $response")
        when(response.getStatusGroup()){
            StatusGroup.SUCCESS -> handleOk(response)
            else -> handleNok(response)
        }
    }

    private fun handleNok(response: Message) {
        unexpectedStatus(response)
    }

    private fun handleOk(response: Message) {
        clientStateService.clearClientState()
    }
}