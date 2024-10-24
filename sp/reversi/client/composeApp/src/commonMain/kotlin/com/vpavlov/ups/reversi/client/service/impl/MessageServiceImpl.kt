package com.vpavlov.ups.reversi.client.service.impl

import com.vpavlov.ups.reversi.client.di.koin
import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.MessageService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import kotlinx.coroutines.sync.Mutex

open class MessageServiceImpl : MessageService {

    protected val mutex = Mutex();

    protected val connectionStateService: ConnectionStateService = koin.get()

    protected val clientStateService: ClientStateService = koin.get()


    override suspend fun processLogin(login: String): Message? {
        return null
    }

    override suspend fun processHandshake(): Message? {
        return null
    }

    override suspend fun processPing(): Message?{
        return null
    }

    override suspend fun processGetLobbies(): Message? {
        return null
    }
}