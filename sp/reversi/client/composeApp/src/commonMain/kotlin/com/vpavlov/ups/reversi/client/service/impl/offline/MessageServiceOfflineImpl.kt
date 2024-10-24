package com.vpavlov.ups.reversi.client.service.impl.offline

import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.service.impl.MessageServiceImpl
import com.vpavlov.ups.reversi.client.state.ClientFlowState

class MessageServiceOfflineImpl : MessageServiceImpl() {

    override suspend fun processLogin(username: String): Message? {
        clientStateService.initState(username = username, flowState = ClientFlowState.MENU)
    }


}