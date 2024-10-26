package com.vpavlov.ups.reversi.client.service.impl.message

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.domains.connection.message.Header
import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.domains.connection.message.Status
import com.vpavlov.ups.reversi.client.domains.connection.message.Subtype
import com.vpavlov.ups.reversi.client.domains.connection.message.Type
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.MessageService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.service.impl.message.MessageServiceImpl.Companion.LOGGER
import com.vpavlov.ups.reversi.client.service.impl.message.processors.HandshakeProcessor
import com.vpavlov.ups.reversi.client.service.impl.message.processors.LoginProcessor
import com.vpavlov.ups.reversi.client.state.ErrorMessage
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch
import kotlinx.coroutines.sync.Mutex
import org.apache.logging.log4j.kotlin.KotlinLogger
import org.apache.logging.log4j.kotlin.loggerOf

open class MessageServiceImpl(
    private val loginProcessor: LoginProcessor,
    private val handshakeProcessor: HandshakeProcessor,
) : MessageService {

    companion object {
        private val LOGGER = loggerOf(MessageServiceImpl::class.java)
    }

    protected val mutex = Mutex();

    override fun processLogin(username: String) = loginProcessor(username = username)



    override fun processHandshake() = handshakeProcessor()



    override fun processPing() = process(LOGGER){

    }

    override fun processGetLobbies() = process(LOGGER) {

    }
}

inline fun process(logger: KotlinLogger, crossinline exchanger: suspend () -> Unit): StateFlow<Boolean> {
    val isComplete = MutableStateFlow(false)
    CoroutineScope(Dispatchers.Default).launch {
        try {
            exchanger()
        }
        catch (e: Throwable) {
            //TODO: handle

            logger.error("Error during message processing.", e)
        }
        isComplete.value = true
    }
    return isComplete
}

fun unexpectedErrorStatus(status: Status, errorStateService: ErrorStateService,logger: KotlinLogger ) {
    logger.error("Unexpected response status: $status")
    errorStateService.setError(errorMessage = ErrorMessage(errorMessage = "Unexpected error status"))
}

fun malformedResponse(subtype: Subtype, errorStateService: ErrorStateService, logger: KotlinLogger ){
    logger.error("Malformed response for the subtype [$subtype]")
    errorStateService.setError(errorMessage = ErrorMessage(errorMessage = "Malformed response for the type [$subtype]"))
}