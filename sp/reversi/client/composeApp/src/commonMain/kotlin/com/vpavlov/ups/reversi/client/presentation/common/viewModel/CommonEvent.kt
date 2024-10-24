package com.vpavlov.ups.reversi.client.presentation.common.viewModel

sealed interface CommonEvent {

    data object ClearError: CommonEvent
}