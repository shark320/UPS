package com.vpavlov.ups.reversi.client.service.impl

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.di.koin
import com.vpavlov.ups.reversi.client.domains.connection.MSG_HEADER_LENGTH
import com.vpavlov.ups.reversi.client.domains.connection.message.Header
import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.domains.connection.message.Payload
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.exceptions.ConnectionException
import com.vpavlov.ups.reversi.client.utils.readExactChars
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

open class ConnectionServiceImpl(private val config: ConnectionConfig) : ConnectionService {

    companion object {
        private val LOGGER = loggerOf(ConnectionServiceImpl::class.java)
    }

    protected val connectionStateService: ConnectionStateService = koin.get()

    protected var readChannel: ByteReadChannel? = null

    protected var writeChannel: ByteWriteChannel? = null

    protected val mutex = Mutex()

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

    override suspend fun exchange(request: Message): Message {
        mutex.withLock(this) {
            if (writeChannel == null || readChannel == null) {
                throw ConnectionException("No available read or write channel fot the socket.")
            }
            val constructed = request.construct()
            LOGGER.debug("Sending message: $request")
            LOGGER.debug("Constructed: $constructed")
            //TODO catch error
            writeChannel!!.writeStringUtf8(constructed)
            return readMessageUnsafe()
        }
    }

    protected open suspend fun readHeaderUnsafe(): Header {
        if (readChannel == null) {
            throw ConnectionException("No available write channel fot the socket.")
        }
        val headerStr: String = readChannel!!.readExactChars(MSG_HEADER_LENGTH)
            ?: throw ConnectionException("Read header string is null.")
        return Header.parse(headerStr)
    }

    protected open suspend fun readPayloadUnsafe(length: Int): Payload {
        if (readChannel == null) {
            throw ConnectionException("No available write channel fot the socket.")
        }
        val payloadStr: String = readChannel!!.readExactChars(length)
            ?: throw ConnectionException("Read payload string is null.")
        return Payload.parse(payloadStr)
    }

    protected open suspend fun readMessageUnsafe(): Message {
        val header = readHeaderUnsafe()
        val payload = readPayloadUnsafe(header.length)
        val message = Message(header = header, payload = payload)
        LOGGER.debug("Read message: $message")
        return message
    }


}