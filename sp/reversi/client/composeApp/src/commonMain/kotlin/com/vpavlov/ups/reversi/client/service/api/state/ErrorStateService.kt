package com.vpavlov.ups.reversi.client.service.api.state

import com.vpavlov.ups.reversi.client.state.ErrorState
import kotlinx.coroutines.flow.StateFlow

interface ErrorStateService {

    fun getStateFlow(): StateFlow<ErrorState>

    fun setError(
        errorMessage: String
    )

    fun clearError()
}