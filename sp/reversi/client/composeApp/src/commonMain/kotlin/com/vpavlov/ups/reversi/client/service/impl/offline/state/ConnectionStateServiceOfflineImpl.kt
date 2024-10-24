package com.vpavlov.ups.reversi.client.service.impl.offline.state

import com.vpavlov.ups.reversi.client.service.impl.state.ConnectionStateServiceImpl
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.map

class ConnectionStateServiceOfflineImpl : ConnectionStateServiceImpl() {

    @Synchronized
    override fun isAliveFLow(): Flow<Boolean> = state.map { value ->
        value.isAlive
    }

    @Synchronized
    override fun isAlive(): Boolean = state.value.isAlive
}