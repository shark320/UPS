package com.vpavlov.ups.reversi.client.service.api

import com.vpavlov.ups.reversi.client.domains.connection.message.Message

interface ConnectionService {

    fun connect()

    suspend fun exchange(message: Message): Message?
}