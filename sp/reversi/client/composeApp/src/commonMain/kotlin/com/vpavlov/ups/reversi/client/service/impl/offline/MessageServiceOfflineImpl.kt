package com.vpavlov.ups.reversi.client.service.impl.offline

import com.vpavlov.ups.reversi.client.service.impl.MessageServiceImpl
import com.vpavlov.ups.reversi.client.state.ClientFlowState
import org.apache.logging.log4j.kotlin.loggerOf

class MessageServiceOfflineImpl : MessageServiceImpl() {

    companion object{
        val LOGGER = loggerOf(MessageServiceOfflineImpl::class.java)
    }

    override fun processLogin(username: String) {
        LOGGER.debug("Processing login")
        clientStateService.initState(username = username, flowState = ClientFlowState.MENU)
    }


}