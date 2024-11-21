package com.vpavlov.ups.reversi.client.presentation.common.viewModel

import com.vpavlov.ups.reversi.client.state.ClientFlowState
import com.vpavlov.ups.reversi.client.state.UserMessage

data class CommonScreenState(
    val messageState: CommonScreenErrorState? = null,
    val isConnectionAlive: Boolean = true,
    val clientFlowState: ClientFlowState? = null,
    val username: String? = null,
    val isWaitingScreen: Boolean = false,
    val waitingScreenText: String = ""
)

data class CommonScreenErrorState(
    val userMessage: UserMessage,
    val isFatalError: Boolean = false,
)