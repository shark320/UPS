package com.vpavlov.ups.reversi.client.service.impl.offline

import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.service.api.MessageService
import com.vpavlov.ups.reversi.client.service.impl.MessageServiceImpl

class MessageServiceOfflineImpl : MessageServiceImpl() {
    override fun processLogin(login: String): Message? {
        //TODO: implement
        return null;
    }
}