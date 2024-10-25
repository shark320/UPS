package com.vpavlov.ups.reversi.client.service.impl

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.di.koin
import com.vpavlov.ups.reversi.client.domains.connection.message.Header
import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.domains.connection.message.Payload
import com.vpavlov.ups.reversi.client.domains.connection.message.Status
import com.vpavlov.ups.reversi.client.domains.connection.message.Subtype
import com.vpavlov.ups.reversi.client.domains.connection.message.Type
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.MessageService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.state.ClientFlowState
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch
import kotlinx.coroutines.sync.Mutex
import org.apache.logging.log4j.kotlin.loggerOf

open class MessageServiceImpl(
    private val config: ConnectionConfig,
    protected val connectionStateService: ConnectionStateService,
    protected val clientStateService: ClientStateService,
    protected val connectionService: ConnectionService,
    protected val errorStateService: ErrorStateService
) : MessageService {

    companion object {
        val LOGGER = loggerOf(MessageServiceImpl::class.java)
    }

    protected val mutex = Mutex();

    private fun unexpectedErrorStatus(status: Status) {
        LOGGER.error("Unexpected response status: $status")
        errorStateService.setError("Unexpected error status")
    }

    private fun malformedResponse(subtype: Subtype){
        LOGGER.error("Malformed response for the subtype [$subtype]")
        errorStateService.setError("Malformed response for the type [$subtype]")
    }

    override fun processLogin(username: String) = processWithResult {
        LOGGER.debug("Processing login with username '$username'")
        val requestHeader = Header(
            type = Type.POST,
            identifier = config.identifier,
            subtype = Subtype.LOGIN
        )
        val payload = Payload();
        payload.setValue("username", username)
        val response = connectionService.exchange(Message(header = requestHeader, payload = payload))
        if (response.isError()) {
            handleLoginError(response)
        } else {
            handleLoginOk(response, username = username)
        }
    }

    private fun handleLoginError(response: Message) {
        when (response.header.status) {
            Status.BAD_REQUEST,
            Status.UNAUTHORIZED,
            Status.NOT_FOUND,
            Status.NOT_ALLOWED -> unexpectedErrorStatus(response.header.status)
            Status.CONFLICT -> {
                LOGGER.info("Provided username conflict. $response")
                errorStateService.setError("The username is already in use")
            }

            Status.OK, Status.NULL_STATUS -> LOGGER.warn("Could not handle error code")
        }
    }

    private fun handleLoginOk(response: Message, username: String){
        val responsePayload = response.payload;
        val state = ClientFlowState.getValueOrNull(responsePayload.getStringValue("state"))
        if (state == null){
            malformedResponse(subtype = response.header.subtype)
        } else{
            clientStateService.initState(username = username, flowState = state)
        }
    }



    override fun processHandshake() = process {
        LOGGER.debug("Processing handshake with username.")
        val requestHeader = Header(
            type = Type.POST,
            identifier = config.identifier,
            subtype = Subtype.HANDSHAKE
        )
        val response = connectionService.exchange(Message(header = requestHeader))
        if (response.isError()) {
            handleHandshakeError()
        } else {
            handleHandshakeOk()
        }
    }

    private fun handleHandshakeError(){
        connectionStateService.updateConnectionState(isHandshake = false)
        //FATAL error -> show error and terminate execution
    }

    private fun handleHandshakeOk() {
        connectionStateService.updateConnectionState(isHandshake = true)
    }

    override fun processPing() {

    }

    override fun processGetLobbies() {

    }

    protected inline fun process(crossinline exchanger: suspend () -> Unit) {
        CoroutineScope(Dispatchers.Default).launch {
            try {
                exchanger()
            } catch (e: Throwable) {
                LOGGER.error("Error during message processing.", e)
            }
        }

    }

    protected inline fun processWithResult(crossinline exchanger: suspend () -> Unit): StateFlow<Boolean> {
        val isComplete = MutableStateFlow(false)
        CoroutineScope(Dispatchers.Default).launch {
            try {
                exchanger()
            } catch (e: Throwable) {
                LOGGER.error("Error during message processing.", e)
            }
            isComplete.value = true
        }
        return isComplete
    }
}