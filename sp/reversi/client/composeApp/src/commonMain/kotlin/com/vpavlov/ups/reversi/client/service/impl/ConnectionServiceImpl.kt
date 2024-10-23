package com.vpavlov.ups.reversi.client.service.impl

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.di.koin
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.ConnectionStateService
import io.ktor.network.selector.ActorSelectorManager
import io.ktor.network.sockets.aSocket
import io.ktor.network.sockets.openReadChannel
import io.ktor.network.sockets.openWriteChannel
import io.ktor.utils.io.ByteReadChannel
import io.ktor.utils.io.ByteWriteChannel
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import org.apache.logging.log4j.kotlin.logger
import org.apache.logging.log4j.kotlin.loggerOf
import java.net.InetSocketAddress

class ConnectionServiceImpl(private val config: ConnectionConfig): ConnectionService {

    companion object{
        private val LOGGER = loggerOf(ConnectionServiceImpl::class.java)
    }

    private val connectionStateService: ConnectionStateService = koin.get()

    private var readChannel:  ByteReadChannel? = null

    private var writeChannel:  ByteWriteChannel? = null

    override fun connect() {
        if (!connectionStateService.isAlive()){
            CoroutineScope(Dispatchers.Default).launch{
                try{
                    val socket = aSocket(ActorSelectorManager(Dispatchers.IO)).tcp()
                        .connect(InetSocketAddress(config.ip, config.port))
                    readChannel = socket.openReadChannel()
                    writeChannel = socket.openWriteChannel(autoFlush = true)
                    connectionStateService.updateConnectionState(isAlive = true, socket = socket)
                    LOGGER.info("Connected to the server [${config.ip}:${config.port}]")
                }catch (e: Throwable){
                    LOGGER.error("Could not connect to the server [${config.ip}:${config.port}]", e)
                }
            }
        }

    }


}