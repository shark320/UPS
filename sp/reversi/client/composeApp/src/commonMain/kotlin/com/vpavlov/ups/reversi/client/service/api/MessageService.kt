package com.vpavlov.ups.reversi.client.service.api

import com.vpavlov.ups.reversi.client.domains.connection.message.Message

interface MessageService {

    suspend fun processLogin(login: String): Message?

    suspend fun processHandshake(): Message?

    suspend fun processPing(): Message?

    suspend fun processGetLobbies(): Message?
}