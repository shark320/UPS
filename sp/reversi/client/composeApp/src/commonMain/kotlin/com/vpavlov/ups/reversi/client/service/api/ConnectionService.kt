package com.vpavlov.ups.reversi.client.service.api

import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.service.exceptions.ConnectionException
import kotlin.jvm.Throws

interface ConnectionService {

    fun connect()

    @Throws(ConnectionException::class)
    suspend fun exchange(request: Message): Message
}

