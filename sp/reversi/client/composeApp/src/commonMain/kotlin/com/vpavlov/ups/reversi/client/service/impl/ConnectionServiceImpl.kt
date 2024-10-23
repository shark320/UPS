package com.vpavlov.ups.reversi.client.service.impl

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import io.ktor.network.selector.ActorSelectorManager
import io.ktor.network.sockets.aSocket
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import java.net.InetSocketAddress

class ConnectionServiceImpl(private val config: ConnectionConfig): ConnectionService {

    override fun connect() {
        CoroutineScope(Dispatchers.Default).launch{
            val socket = aSocket(ActorSelectorManager(Dispatchers.IO)).tcp()
                .connect(InetSocketAddress(config.ip, config.port))
        }
    }


}