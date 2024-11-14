package com.vpavlov.ups.reversi.client.service.processor

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.domains.connection.message.Header
import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.domains.connection.message.Subtype
import com.vpavlov.ups.reversi.client.domains.connection.message.Type
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.UserMessageStateService
import com.vpavlov.ups.reversi.client.state.ClientFlowState
import com.vpavlov.ups.reversi.client.state.LobbyInfo
import com.vpavlov.ups.reversi.client.utils.requireAllNotNull

class GetLobbiesProcessor(
    private val config: ConnectionConfig,
    clientStateService: ClientStateService,
    connectionService: ConnectionService,
    userMessageStateService: UserMessageStateService
) : CommonClientProcessor(
    connectionService = connectionService,
    userMessageStateService = userMessageStateService,
    clientStateService = clientStateService
) {

    operator fun invoke() = process {
        LOGGER.debug("Processing getting lobbies list.")
        val requestHeader = Header(
            type = Type.GET,
            identifier = config.identifier,
            subtype = Subtype.LOBBIES_LIST
        )
        val response = connectionService.exchange(Message(header = requestHeader))
        if (response.isError()) {
            handleError(response)
        } else {
            handleOk(response)
        }
    }

    private fun handleError(response: Message) {
        unexpectedErrorStatus(
            response.header.status,
        )
    }

    private fun handleOk(response: Message) {
        val state = ClientFlowState.getValueOrNull(response.payload.getStringValue("state"))
        val lobbiesList = response.payload.getListOfStrings("lobbies")
        val lobbyHostsList = response.payload.getListOfStrings("lobby_hosts")
        if (!requireAllNotNull(state, lobbiesList, lobbyHostsList) || lobbiesList!!.size != lobbyHostsList!!.size) {
            malformedResponse(
                subtype = response.header.subtype,
            )
            return
        }
        val lobbiesInfoList = mutableListOf<LobbyInfo>()
        lobbiesList.forEachIndexed { index, lobby ->
            lobbiesInfoList.add(
                LobbyInfo(
                    lobbyName = lobby,
                    lobbyHost = lobbyHostsList[index]
                )
            )
        }
        clientStateService.updateState(
            flowState = state!!,
            lobbiesList = lobbiesInfoList
        )
    }
}