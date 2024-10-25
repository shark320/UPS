package com.vpavlov.ups.reversi.client.service.impl.state

import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.state.ErrorState
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow

class ErrorStateServiceImpl : ErrorStateService {

    private val _state = MutableStateFlow(ErrorState())

    override fun getStateFlow(): StateFlow<ErrorState> = _state

    override fun setError(errorMessage: String, fatal: Boolean) {
        _state.value = ErrorState(
            isError = true,
            errorMessage = errorMessage,
            fatal = fatal
        )
    }

    override fun clearError() {
        _state.value = ErrorState(isError = false, fatal = false)
    }
}