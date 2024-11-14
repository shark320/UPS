package com.vpavlov.ups.reversi.client.service.processor

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.domains.connection.message.Header
import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.domains.connection.message.Status
import com.vpavlov.ups.reversi.client.domains.connection.message.Subtype
import com.vpavlov.ups.reversi.client.domains.connection.message.Type
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.UserMessageStateService
import com.vpavlov.ups.reversi.client.state.UserMessage

class StartGameProcessor(
    private val config: ConnectionConfig,
    clientStateService: ClientStateService,
    connectionService: ConnectionService,
    userMessageStateService: UserMessageStateService
): CommonClientProcessor(
    connectionService = connectionService,
    userMessageStateService = userMessageStateService,
    clientStateService = clientStateService,
)  {
    operator fun invoke() = process{
        LOGGER.debug("Processing game start.")
        val requestHeader = Header(
            type = Type.POST,
            identifier = config.identifier,
            subtype = Subtype.START_GAME
        )
        val response =
            connectionService.exchange(Message(header = requestHeader))
        if (response.isError()) {
            handleError(response)
        } else {
            handleOk(response)
        }
    }

    private fun handleError(response: Message) {
        val status = response.header.status

        when(status){
            Status.UNAUTHORIZED -> {
                userMessageStateService.showError(
                    userMessage = UserMessage(
                        message = "You are not the lobby host."
                    )
                )
                getAndUpdateState(response)
            }
            Status.NOT_ALLOWED -> {
                userMessageStateService.showError(
                    userMessage = UserMessage(
                        message = "Not enough players!"
                    )
                )
                getAndUpdateState(response)
            }
            else -> {
                unexpectedErrorStatus(response)
            }
        }
    }

    private fun handleOk(response: Message) {
        getAndUpdateState(response)
    }


}