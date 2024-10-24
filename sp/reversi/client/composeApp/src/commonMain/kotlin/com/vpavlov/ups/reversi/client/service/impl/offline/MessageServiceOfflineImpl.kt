package com.vpavlov.ups.reversi.client.service.impl.offline

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.service.impl.MessageServiceImpl
import com.vpavlov.ups.reversi.client.state.ClientFlowState
import org.apache.logging.log4j.kotlin.loggerOf

class MessageServiceOfflineImpl(
    config: ConnectionConfig,
    connectionStateService: ConnectionStateService,
    clientStateService: ClientStateService,
    connectionService: ConnectionService,
    errorStateService: ErrorStateService
) : MessageServiceImpl(
    config = config,
    connectionStateService = connectionStateService,
    clientStateService = clientStateService,
    connectionService = connectionService,
    errorStateService = errorStateService
) {

    companion object{
        val LOGGER = loggerOf(MessageServiceOfflineImpl::class.java)
    }

    override fun processLogin(username: String) {
        LOGGER.debug("Processing login")
        clientStateService.initState(username = username, flowState = ClientFlowState.MENU)
    }


}