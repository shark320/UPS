package com.vpavlov.ups.reversi.client.presentation.common.viewModel

import androidx.compose.runtime.MutableState
import androidx.compose.runtime.State
import androidx.compose.runtime.mutableStateOf
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.state.ClientState
import com.vpavlov.ups.reversi.client.state.ConnectionState
import com.vpavlov.ups.reversi.client.state.ErrorState
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach

abstract class CommonScreenViewModel<EventType, StateType>(
    protected val errorStateService: ErrorStateService,
    protected val connectionStateService: ConnectionStateService,
    protected val clientStateService: ClientStateService
) : ViewModel() {

    protected val _commonScreenState = mutableStateOf(CommonScreenState())
    val commonScreenState: State<CommonScreenState> = _commonScreenState

    protected val _state = initState()
    val state: State<StateType> = _state

    init {
        errorStateService.getStateFlow().onEach { errorState ->
            handleErrorState(errorState = errorState)
        }.launchIn(viewModelScope)

        connectionStateService.getConnectionState().onEach {
            handleConnectionStateUpdate(it)
        }.launchIn(viewModelScope)

        clientStateService.getStateFlow().onEach { clientState ->
            handleClientStateUpdate(clientState)
        }.launchIn(viewModelScope)
    }

    protected fun handleConnectionStateUpdate(connectionState: ConnectionState) {
        _commonScreenState.value = commonScreenState.value.copy(
            isConnectionAlive = connectionState.isAlive
        )
    }

    protected fun handleClientStateUpdate(clientState: ClientState?){
        _commonScreenState.value = commonScreenState.value.copy(
            clientFlowState = clientState?.flowState
        )
    }

    fun onCommonEvent(commonScreenEvent: CommonScreenEvent) {
        when (commonScreenEvent) {
            CommonScreenEvent.ClearError -> {
                _commonScreenState.value = commonScreenState.value.copy(errorState = null)
                errorStateService.clearError()
            }
        }
    }

    abstract fun onEvent(event: EventType)

    protected abstract fun initState(): MutableState<StateType>

    protected fun handleErrorState(errorState: ErrorState) {
        if (errorState.isError && errorState.errorMessage != null) {
            _commonScreenState.value = commonScreenState.value.copy(
                errorState = CommonScreenErrorState(
                    errorMessage = errorState.errorMessage,
                    isFatalError = errorState.isFatal
                )
            )
        }
    }

}