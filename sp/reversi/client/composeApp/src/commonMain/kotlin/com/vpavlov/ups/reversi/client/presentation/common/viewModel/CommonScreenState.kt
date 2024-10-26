package com.vpavlov.ups.reversi.client.presentation.common.viewModel

import com.vpavlov.ups.reversi.client.state.ErrorMessage

data class CommonScreenState(
    val errorState: CommonScreenErrorState? = null,
    val isConnectionAlive: Boolean = true
)

data class CommonScreenErrorState(
    val errorMessage: ErrorMessage,
    val isFatalError: Boolean = false,
)