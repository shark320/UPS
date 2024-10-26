package com.vpavlov.ups.reversi.client.presentation.common.viewModel

sealed interface CommonScreenEvent {

    data object ClearError: CommonScreenEvent
}