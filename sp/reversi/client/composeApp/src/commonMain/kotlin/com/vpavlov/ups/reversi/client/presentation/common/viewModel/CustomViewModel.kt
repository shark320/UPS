package com.vpavlov.ups.reversi.client.presentation.common.viewModel

import androidx.compose.runtime.State
import androidx.compose.runtime.mutableStateOf
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.state.ConnectionState
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach

open class CustomViewModel(
    protected val errorStateService: ErrorStateService,
    protected val connectionStateService: ConnectionStateService
) : ViewModel() {

    protected val _commonState = mutableStateOf(CommonState())
    val commonState: State<CommonState> = _commonState

    init {
        errorStateService.getStateFlow().onEach { errorState ->
            if (errorState.isError){
                _commonState.value = commonState.value.copy(errorMessage = errorState.errorMessage)
            } else {
                _commonState.value = commonState.value.copy(errorMessage = null)
            }
        }.launchIn(viewModelScope)
        connectionStateService.getConnectionState().onEach {
            handleConnectionStateUpdate(it)
        }
    }

    protected fun handleConnectionStateUpdate(connectionState: ConnectionState){
        _commonState.value = commonState.value.copy(
            isConnectionAlive = connectionState.isAlive
        )
    }

    fun onEvent(commonEvent: CommonEvent){
        when(commonEvent){
            CommonEvent.ClearError -> {
                _commonState.value = commonState.value.copy(errorMessage = null)
                errorStateService.clearError()
            }
        }
    }

}