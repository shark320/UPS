package com.vpavlov.ups.reversi.client.service.api.state

import com.vpavlov.ups.reversi.client.state.UserMessage
import com.vpavlov.ups.reversi.client.state.ShowMessageState
import kotlinx.coroutines.flow.StateFlow

interface UserMessageStateService {

    fun getStateFlow(): StateFlow<ShowMessageState>

    fun showError(
        userMessage: UserMessage,
        fatal: Boolean = false,
        initialException: Exception? = null
    )

    fun showInfo(
        userMessage: UserMessage,
    )

    fun clearMessage()
}

