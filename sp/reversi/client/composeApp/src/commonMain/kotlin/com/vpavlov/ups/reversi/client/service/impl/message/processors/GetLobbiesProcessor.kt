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
import com.vpavlov.ups.reversi.client.state.ClientFlowState
import com.vpavlov.ups.reversi.client.state.LobbyInfo
import com.vpavlov.ups.reversi.client.utils.requireAllNotNull

class GetLobbiesProcessor(
    private val config: ConnectionConfig,
    private val clientStateService: ClientStateService,
    connectionService: ConnectionService,
    errorStateService: ErrorStateService
) : CommonProcessor(
    connectionService = connectionService,
    errorStateService = errorStateService,
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
            errorStateService = errorStateService,
            logger = LOGGER
        )
    }

    private fun handleOk(response: Message) {
        val state = ClientFlowState.getValueOrNull(response.payload.getStringValue("state"))
        val lobbiesList = response.payload.getListOfStrings("lobbies")
        val lobbyHostsList = response.payload.getListOfStrings("lobby_hosts")
        if (!requireAllNotNull(state, lobbiesList, lobbyHostsList) || lobbiesList!!.size != lobbyHostsList!!.size) {
            malformedResponse(
                subtype = response.header.subtype,
                logger = LOGGER
            )
            return
        }
        val lobbiesInfoList = mutableListOf<LobbyInfo>()
        lobbiesList.forEachIndexed { index, lobby ->
            lobbiesInfoList.addLast(
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