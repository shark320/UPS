package com.vpavlov.ups.reversi.client.service.impl.state

import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.state.ErrorMessage
import com.vpavlov.ups.reversi.client.state.ErrorState
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import java.lang.Exception

class ErrorStateServiceImpl : ErrorStateService {

    private val _state = MutableStateFlow(ErrorState())

    override fun getStateFlow(): StateFlow<ErrorState> = _state

    override fun setError(errorMessage: ErrorMessage, fatal: Boolean, initialException: Exception?) {
        _state.value = ErrorState(
            isError = true,
            errorMessage = errorMessage,
            isFatal = fatal,
            initialException = initialException
        )
    }

    override fun clearError() {
        _state.value = ErrorState(isError = false, isFatal = false, errorMessage = null, initialException = null)
    }
}