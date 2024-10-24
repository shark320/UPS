package com.vpavlov.ups.reversi.client.service.impl.offline

import androidx.compose.runtime.State
import androidx.compose.runtime.mutableStateOf
import com.vpavlov.ups.reversi.client.service.api.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.impl.ConnectionStateServiceImpl
import com.vpavlov.ups.reversi.client.state.ConnectionState
import io.ktor.network.sockets.Socket

class ConnectionStateServiceOfflineImpl : ConnectionStateServiceImpl() {

    @Synchronized
    override fun isAlive(): Boolean = state.value.isAlive

}