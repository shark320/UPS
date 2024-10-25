package com.vpavlov.ups.reversi.client.service.impl.message.processors

import com.vpavlov.ups.reversi.client.domains.connection.message.Status
import com.vpavlov.ups.reversi.client.domains.connection.message.Subtype
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.channels.ClosedReceiveChannelException
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch
import org.apache.logging.log4j.kotlin.loggerOf

open class CommonProcessor(
    protected val errorStateService: ErrorStateService,
    protected val connectionService: ConnectionService,
    protected val connectionStateService: ConnectionStateService
) {

    protected val LOGGER = loggerOf(this::class.java)


    protected inline fun process(crossinline exchanger: suspend () -> Unit): StateFlow<Boolean> {
        val isComplete = MutableStateFlow(false)
        CoroutineScope(Dispatchers.Default).launch {
            try {
                exchanger()
            }
            catch (e: ClosedReceiveChannelException){
                connectionStateService.connectionLost()
                LOGGER.error("Connection to the server lost", e)
            }
            catch (e: Throwable) {
                errorStateService.setError("A fatal error during the message processing.", fatal = true)
                LOGGER.error("Error during message processing.", e)
            }
            isComplete.value = true
        }
        return isComplete
    }

    protected fun unexpectedErrorStatus(status: Status, errorStateService: ErrorStateService) {
        LOGGER.error("Unexpected response status: $status")
        errorStateService.setError("Unexpected error status")
    }

    protected fun malformedResponse(subtype: Subtype, errorStateService: ErrorStateService){
        LOGGER.error("Malformed response for the subtype [$subtype]")
        errorStateService.setError("Malformed response for the type [$subtype]")
    }
}