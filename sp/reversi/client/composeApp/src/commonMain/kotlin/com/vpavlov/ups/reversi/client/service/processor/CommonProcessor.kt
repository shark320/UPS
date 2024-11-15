package com.vpavlov.ups.reversi.client.service.processor

import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.domains.connection.message.Status
import com.vpavlov.ups.reversi.client.domains.connection.message.Subtype
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.UserMessageStateService
import com.vpavlov.ups.reversi.client.service.exceptions.ConnectionException
import com.vpavlov.ups.reversi.client.state.UserMessage
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.channels.ClosedReceiveChannelException
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.launch
import org.apache.logging.log4j.kotlin.loggerOf

open class CommonProcessor(
    protected val userMessageStateService: UserMessageStateService,
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
                userMessageStateService.showError(
                    userMessage = UserMessage(
                        message = "A fatal error during the message processing.",
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

    protected inline fun <T: ExecutionResult> processWithResult(errorResult: T,crossinline exchanger: suspend () -> T): StateFlow<T?> {
        val result = MutableStateFlow<T?>(null)
        CoroutineScope(Dispatchers.Default).launch {
            var isError = false
            try {
                result.value = exchanger()
            } catch (e: ConnectionException) {
                onConnectionError(e)
                isError = true
            } catch (e: ClosedReceiveChannelException) {
                onConnectionError(e)
                isError = true
            } catch (e: Throwable) {
                userMessageStateService.showError(
                    userMessage = UserMessage(
                        message = "A fatal error during the message processing.",
                        okButton = "Exit"
                    ),
                    fatal = true
                )
                LOGGER.error("Error during message processing.", e)
                isError = true
            }
            if (isError){
                result.value = errorResult
            }
        }
        return result
    }

    protected fun unexpectedErrorStatus(status: Status) {
        LOGGER.error("Unexpected response status: $status")
        userMessageStateService.showError(userMessage = UserMessage(message = "Unexpected error status"))
    }

    protected fun unexpectedErrorStatus(response: Message) {
        LOGGER.error("Unexpected response status: ${response.header.status}")
        userMessageStateService.showError(userMessage = UserMessage(message = "Unexpected error status. Message: ${response.payload.getStringValue("msg")}"))
    }

    protected fun unexpectedStatus(response: Message) {
        LOGGER.error("Unexpected response status: ${response.header.status}")
        userMessageStateService.showError(userMessage = UserMessage(message = "Unexpected response status: ${response.header.status}"))
    }

    protected open fun onConnectionError(exception: Exception) {
        connectionService.connectionLost()
        LOGGER.error("Connection to the server lost", exception)
    }

    protected fun malformedResponse(subtype: Subtype) {
        LOGGER.error("Malformed response for the subtype [$subtype]")
        userMessageStateService.showError(userMessage = UserMessage(message = "Malformed response for the type [$subtype]"))
    }
}


open class ExecutionResult ()