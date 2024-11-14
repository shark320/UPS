package com.vpavlov.ups.reversi.client.service.impl.state

import com.vpavlov.ups.reversi.client.service.api.state.UserMessageStateService
import com.vpavlov.ups.reversi.client.state.MessageType
import com.vpavlov.ups.reversi.client.state.UserMessage
import com.vpavlov.ups.reversi.client.state.ShowMessageState
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import java.lang.Exception

class UserMessageStateServiceImpl : UserMessageStateService {

    private val _state = MutableStateFlow(ShowMessageState())

    override fun getStateFlow(): StateFlow<ShowMessageState> = _state

    override fun showError(userMessage: UserMessage, fatal: Boolean, initialException: Exception?) {
        _state.value = ShowMessageState(
            messageType = MessageType.ERROR,
            isShowMessage = true,
            message = userMessage,
            isFatal = fatal,
            initialException = initialException
        )
    }

    override fun showInfo(userMessage: UserMessage) {
        _state.value = ShowMessageState(
            messageType = MessageType.INFO,
            isShowMessage = true,
            message = userMessage,
            isFatal = false,
        )
    }

    override fun clearMessage() {
        _state.value = ShowMessageState(isShowMessage = false, isFatal = false, message = null, initialException = null)
    }
}