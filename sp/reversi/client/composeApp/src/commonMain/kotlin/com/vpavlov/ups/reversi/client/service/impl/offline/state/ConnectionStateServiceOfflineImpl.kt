package com.vpavlov.ups.reversi.client.service.impl.offline.state

import com.vpavlov.ups.reversi.client.service.impl.state.ConnectionStateServiceImpl

class ConnectionStateServiceOfflineImpl : ConnectionStateServiceImpl() {

    @Synchronized
    override fun isAlive(): Boolean = state.value.isAlive

}