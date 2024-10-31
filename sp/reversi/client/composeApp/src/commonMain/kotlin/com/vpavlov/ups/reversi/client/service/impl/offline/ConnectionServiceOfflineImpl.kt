package com.vpavlov.ups.reversi.client.service.impl.offline

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.domains.connection.message.Header
import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.domains.connection.message.Payload
import com.vpavlov.ups.reversi.client.service.api.PingService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.service.impl.ConnectionServiceImpl
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import org.apache.logging.log4j.kotlin.loggerOf

class ConnectionServiceOfflineImpl(
    config: ConnectionConfig,
    connectionStateService: ConnectionStateService, errorStateService: ErrorStateService,
) :
    ConnectionServiceImpl(
        config = config, connectionStateService = connectionStateService,
        errorStateService = errorStateService
    ) {

    companion object {
        private val LOGGER = loggerOf(ConnectionServiceOfflineImpl::class.java)
    }

    @Synchronized
    override fun connect() {
        if (!connectionStateService.isAlive()) {
            CoroutineScope(Dispatchers.Default).launch {
                delay(1000L)
                connectionStateService.updateConnectionState(isAlive = true)
            }
        }

    }

//    override suspend fun exchange(request: Message): Message {
//        LOGGER.debug("Offline message sending is not possible")
//
//    }
//
//    override fun readHeaderUnsafe(){
//        LOGGER.debug("Offline communication is not possible")
//
//    }
//
//    override suspend fun readPayloadUnsafe(length: Int): Payload {
//        LOGGER.debug("Offline communication is not possible")
//        return null
//    }
//
//    override suspend fun readMessageUnsafe(): Message {
//        LOGGER.debug("Offline communication is not possible")
//        return null
//    }


}