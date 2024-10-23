package com.vpavlov.ups.reversi.client.service.impl

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.di.koin
import com.vpavlov.ups.reversi.client.domains.connection.MSG_HEADER_LENGTH
import com.vpavlov.ups.reversi.client.domains.connection.message.Header
import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.domains.connection.message.Payload
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.ConnectionStateService
import io.ktor.network.selector.ActorSelectorManager
import io.ktor.network.sockets.aSocket
import io.ktor.network.sockets.openReadChannel
import io.ktor.network.sockets.openWriteChannel
import io.ktor.utils.io.ByteReadChannel
import io.ktor.utils.io.ByteWriteChannel
import io.ktor.utils.io.read
import io.ktor.utils.io.writeStringUtf8
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.sync.Mutex
import kotlinx.coroutines.sync.withLock
import org.apache.logging.log4j.kotlin.loggerOf
import java.net.InetSocketAddress

class ConnectionServiceImpl(private val config: ConnectionConfig) : ConnectionService {

    companion object {
        private val LOGGER = loggerOf(ConnectionServiceImpl::class.java)
    }

    private val connectionStateService: ConnectionStateService = koin.get()

    private var readChannel: ByteReadChannel? = null

    private var writeChannel: ByteWriteChannel? = null

    private val mutex = Mutex()

    @Synchronized
    override fun connect() {
        if (!connectionStateService.isAlive()) {
            CoroutineScope(Dispatchers.Default).launch {
                try {
                    val socket = aSocket(ActorSelectorManager(Dispatchers.IO)).tcp()
                        .connect(InetSocketAddress(config.ip, config.port))
                    readChannel = socket.openReadChannel()
                    writeChannel = socket.openWriteChannel(autoFlush = true)
                    connectionStateService.updateConnectionState(isAlive = true, socket = socket)
                    LOGGER.info("Connected to the server [${config.ip}:${config.port}]")
                } catch (e: Throwable) {
                    //TODO: reconnect on error
                    LOGGER.error("Could not connect to the server [${config.ip}:${config.port}]", e)
                }
            }
        }

    }

    override suspend fun exchange(message: Message): Message? {
        mutex.withLock(this) {
            if (writeChannel == null || readChannel == null) {
                LOGGER.error("No available read or write channel fot the socket")
                return null
            }
            writeChannel!!.writeStringUtf8(message.construct())
            return readMessageUnsafe()
        }
    }

    private suspend fun readHeaderUnsafe(): Header? {
        if (readChannel == null) {
            LOGGER.error("No available write channel fot the socket.")
            return null
        }
        var headerStr: String? = null
        var header: Header? = null
        readChannel!!.read(MSG_HEADER_LENGTH) { input ->
            headerStr = String(input.array())
        }
        headerStr?.let{str -> header = Header.parse(str)}
        return header
    }

    private suspend fun readPayloadUnsafe(length: Int): Payload? {
        if (readChannel == null) {
            LOGGER.error("No available write channel fot the socket.")
            return null
        }
        var payloadStr: String? = null
        var payload: Payload? = null
        readChannel!!.read(length) { input ->
            payloadStr = String(input.array())
        }
        payloadStr?.let { str -> payload = Payload.parse(str) }
        return payload
    }

    private suspend fun readMessageUnsafe(): Message? {
        val header = readHeaderUnsafe() ?: return null
        val payload = readPayloadUnsafe(header.length) ?: return null
        return Message(header = header, payload = payload)
    }


}