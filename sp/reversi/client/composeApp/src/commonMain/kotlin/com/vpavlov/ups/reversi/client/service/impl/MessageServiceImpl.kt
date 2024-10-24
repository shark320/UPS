package com.vpavlov.ups.reversi.client.service.impl

import com.vpavlov.ups.reversi.client.di.koin
import com.vpavlov.ups.reversi.client.service.api.MessageService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import kotlinx.coroutines.sync.Mutex

open class MessageServiceImpl : MessageService {

    protected val mutex = Mutex();

    protected val connectionStateService: ConnectionStateService = koin.get()

    protected val clientStateService: ClientStateService = koin.get()

    override fun processLogin(username: String) {

    }

    override fun processHandshake() {

    }

    override fun processPing(){

    }

    override fun processGetLobbies() {

    }
}