package com.vpavlov.ups.reversi.client.service.api

import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import kotlinx.coroutines.flow.StateFlow

interface MessageService {

    fun processLogin(username: String): StateFlow<Boolean>

    fun processHandshake()

    fun processPing()

    fun processGetLobbies()
}