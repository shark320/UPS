package com.vpavlov.ups.reversi.client.service.api

import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import kotlinx.coroutines.flow.StateFlow

interface MessageService {

    fun processLogin(username: String): StateFlow<Boolean>

    fun processHandshake(): StateFlow<Boolean>

    fun processPing(): StateFlow<Boolean>

    fun processGetLobbies(): StateFlow<Boolean>
}