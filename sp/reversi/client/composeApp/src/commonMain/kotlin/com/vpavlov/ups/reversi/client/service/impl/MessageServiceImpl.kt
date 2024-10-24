package com.vpavlov.ups.reversi.client.service.impl

import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.service.api.MessageService

open class MessageServiceImpl : MessageService {
    override fun processLogin(login: String): Message? {
        //TODO: implement
        return null;
    }
}