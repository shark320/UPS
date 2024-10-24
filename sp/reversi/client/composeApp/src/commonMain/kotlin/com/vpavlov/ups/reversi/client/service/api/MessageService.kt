package com.vpavlov.ups.reversi.client.service.api

import com.vpavlov.ups.reversi.client.domains.connection.message.Message

interface MessageService {

    fun processLogin(username: String)

    fun processHandshake()

    fun processPing()

    fun processGetLobbies()
}