package com.vpavlov.ups.reversi.client.presentation.common.viewModel

import androidx.compose.runtime.MutableState
import androidx.compose.runtime.State
import androidx.compose.runtime.mutableStateOf
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.UserMessageStateService
import com.vpavlov.ups.reversi.client.state.ClientState
import com.vpavlov.ups.reversi.client.state.ConnectionState
import com.vpavlov.ups.reversi.client.state.ShowMessageState
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach
import org.apache.logging.log4j.kotlin.loggerOf

abstract class CommonScreenViewModel<EventType, StateType>(
    protected val userMessageStateService: UserMessageStateService,
    protected val connectionStateService: ConnectionStateService,
    protected val clientStateService: ClientStateService
) : ViewModel() {

    protected val LOGGER = loggerOf(this::class.java)

    protected val _commonScreenState = mutableStateOf(CommonScreenState())
    val commonScreenState: State<CommonScreenState> = _commonScreenState

    protected val _state = initState()
    val state: State<StateType> = _state

    init {
        userMessageStateService.getStateFlow().onEach { errorState ->
            handleErrorState(showMessageState = errorState)
        }.launchIn(viewModelScope)

        connectionStateService.getConnectionState().onEach {
            handleConnectionStateUpdate(it)
        }.launchIn(viewModelScope)

        clientStateService.getStateFlow().onEach { clientState ->
            handleClientStateUpdate(clientState)
            handleClientStateUpdateCst(clientState)
        }.launchIn(viewModelScope)
    }

    protected fun handleConnectionStateUpdate(connectionState: ConnectionState) {
        _commonScreenState.value = commonScreenState.value.copy(
            isConnectionAlive = connectionState.isAlive
        )
    }

    protected fun handleClientStateUpdate(clientState: ClientState?){
        _commonScreenState.value = commonScreenState.value.copy(
            clientFlowState = clientState?.flowState,
            username = clientState?.username
        )
    }

    protected open fun handleClientStateUpdateCst(clientState: ClientState?){}

    fun onCommonEvent(commonScreenEvent: CommonScreenEvent) {
        when (commonScreenEvent) {
            CommonScreenEvent.ClearError -> {
                _commonScreenState.value = commonScreenState.value.copy(messageState = null)
                userMessageStateService.clearMessage()
            }
        }
    }

    abstract fun onEvent(event: EventType)

    protected abstract fun initState(): MutableState<StateType>

    protected fun handleErrorState(showMessageState: ShowMessageState) {
        if (showMessageState.isShowMessage && showMessageState.message != null) {
            _commonScreenState.value = commonScreenState.value.copy(
                messageState = CommonScreenErrorState(
                    userMessage = showMessageState.message,
                    isFatalError = showMessageState.isFatal
                )
            )
        }
    }

}