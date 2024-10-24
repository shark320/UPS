package com.vpavlov.ups.reversi.client.service.impl.offline

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.domains.connection.message.Header
import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.domains.connection.message.Payload
import com.vpavlov.ups.reversi.client.service.impl.ConnectionServiceImpl
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import org.apache.logging.log4j.kotlin.loggerOf

class ConnectionServiceOfflineImpl(config: ConnectionConfig) :
    ConnectionServiceImpl(config = config) {

    companion object {
        private val LOGGER = loggerOf(ConnectionServiceOfflineImpl::class.java)
    }

    @Synchronized
    override fun connect() {
        if (!connectionStateService.isAliveFLow()) {
            CoroutineScope(Dispatchers.Default).launch {
                delay(1000L)
                connectionStateService.updateConnectionState(isAlive = true)
            }
        }

    }

    override suspend fun exchange(message: Message): Message? {
        LOGGER.debug("Offline message sending is not possible")
        return null
    }

    override suspend fun readHeaderUnsafe(): Header? {
        LOGGER.debug("Offline communication is not possible")
        return null
    }

    override suspend fun readPayloadUnsafe(length: Int): Payload? {
        LOGGER.debug("Offline communication is not possible")
        return null
    }

    override suspend fun readMessageUnsafe(): Message? {
        LOGGER.debug("Offline communication is not possible")
        return null
    }


}