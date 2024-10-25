package com.vpavlov.ups.reversi.client.service.impl.offline

import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.MessageService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.service.impl.message.process
import com.vpavlov.ups.reversi.client.state.ClientFlowState
import org.apache.logging.log4j.kotlin.loggerOf

class MessageServiceOfflineImpl(
    private val connectionStateService: ConnectionStateService,
    private val clientStateService: ClientStateService,
    private val errorStateService: ErrorStateService
) : MessageService{

    companion object{
        val LOGGER = loggerOf(MessageServiceOfflineImpl::class.java)
    }

    override fun processLogin(username: String) = process {
        LOGGER.debug("Processing login")
        clientStateService.initState(username = username, flowState = ClientFlowState.MENU)
    }

    override fun processHandshake() = process {
    }

    override fun processPing() = process {
    }

    override fun processGetLobbies() = process {
    }


}