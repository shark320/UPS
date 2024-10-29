package com.vpavlov.ups.reversi.client.service.processor.common

import com.vpavlov.ups.reversi.client.domains.connection.message.Payload
import com.vpavlov.ups.reversi.client.domains.connection.message.Status
import com.vpavlov.ups.reversi.client.domains.connection.message.Subtype
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.service.exceptions.ConnectionException
import com.vpavlov.ups.reversi.client.state.ErrorMessage
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.channels.ClosedReceiveChannelException
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch
import org.apache.logging.log4j.kotlin.loggerOf

abstract class CommonProcessor(
    protected val errorStateService: ErrorStateService,
    protected val connectionService: ConnectionService,
) {


    protected val LOGGER = loggerOf(this::class.java)


    protected inline fun process(crossinline exchanger: suspend () -> Unit): StateFlow<Boolean> {
        val isComplete = MutableStateFlow(false)
        CoroutineScope(Dispatchers.Default).launch {
            try {
                exchanger()
            } catch (e: ConnectionException) {
                onConnectionError(e)
            } catch (e: ClosedReceiveChannelException) {
                onConnectionError(e)
            } catch (e: Throwable) {
                errorStateService.setError(
                    errorMessage = ErrorMessage(
                        errorMessage = "A fatal error during the message processing.",
                        okButton = "Exit"
                    ),
                    fatal = true
                )
                LOGGER.error("Error during message processing.", e)
            }
            isComplete.value = true
        }
        return isComplete
    }

    protected fun unexpectedErrorStatus(status: Status) {
        LOGGER.error("Unexpected response status: $status")
        errorStateService.setError(errorMessage = ErrorMessage(errorMessage = "Unexpected error status"))
    }

    protected open fun onConnectionError(exception: Exception) {
        connectionService.connectionLost()
        LOGGER.error("Connection to the server lost", exception)
    }

    protected fun malformedResponse(subtype: Subtype) {
        LOGGER.error("Malformed response for the subtype [$subtype]")
        errorStateService.setError(errorMessage = ErrorMessage(errorMessage = "Malformed response for the type [$subtype]"))
    }


}
