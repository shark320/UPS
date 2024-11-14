package com.vpavlov.ups.reversi.client.service.processor

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.domains.connection.message.Header
import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.domains.connection.message.Payload
import com.vpavlov.ups.reversi.client.domains.connection.message.Status
import com.vpavlov.ups.reversi.client.domains.connection.message.Subtype
import com.vpavlov.ups.reversi.client.domains.connection.message.Type
import com.vpavlov.ups.reversi.client.domains.game.Lobby
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.UserMessageStateService
import com.vpavlov.ups.reversi.client.state.ClientFlowState
import com.vpavlov.ups.reversi.client.state.UserMessage
import com.vpavlov.ups.reversi.client.utils.requireAllNotNull

class CreateLobbyProcessor(
    private val config: ConnectionConfig,
    clientStateService: ClientStateService,
    connectionService: ConnectionService,
    userMessageStateService: UserMessageStateService
) : CommonClientProcessor(
    connectionService = connectionService,
    userMessageStateService = userMessageStateService,
    clientStateService = clientStateService,
) {
    operator fun invoke(lobbyName: String) = process {
        LOGGER.debug("Processing creating a new lobby.")
        val requestHeader = Header(
            type = Type.POST,
            identifier = config.identifier,
            subtype = Subtype.CREATE_GAME
        )
        val payload = Payload();
        payload.setValue("name", lobbyName)
        val response =
            connectionService.exchange(Message(header = requestHeader, payload = payload))
        if (response.isError()) {
            handleError(response)
        } else {
            handleOk(response)
        }
    }

    private fun handleError(response: Message) {
        val status = response.header.status
        if (status == Status.CONFLICT){
            userMessageStateService.showError(
                userMessage = UserMessage(
                    message = "Lobby with the entered name already exists."
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
        val state = ClientFlowState.getValueOrNull(response.payload.getStringValue("state"))
        val user = response.payload.getStringValue("user")
        //TODO: change documentation
        val lobbyName = response.payload.getStringValue("game")
        if (!requireAllNotNull(state, user, lobbyName)) {
            malformedResponse(
                subtype = response.header.subtype,
            )
            return
        }
        clientStateService.updateState(
            flowState = state!!,
            currentLobby = Lobby(
                host = user!!,
                players = listOf(user),
                name = lobbyName!!
            )
        )
    }

}