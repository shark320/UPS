package com.vpavlov.ups.reversi.client.presentation.menu

import androidx.compose.runtime.MutableState
import androidx.compose.runtime.mutableStateOf
import com.vpavlov.ups.reversi.client.presentation.common.viewModel.CommonScreenViewModel
import com.vpavlov.ups.reversi.client.service.api.PingService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService

class MenuScreenViewModel(
    connectionStateService: ConnectionStateService,
    errorStateService: ErrorStateService,
    private val pingService: PingService
): CommonScreenViewModel<MenuScreenEvent, MenuScreenState>(
    errorStateService = errorStateService,
    connectionStateService = connectionStateService
) {

    init{
        pingService.resume()
    }

    override fun onEvent(event: MenuScreenEvent) {

    }

    override fun initState(): MutableState<MenuScreenState>  = mutableStateOf(MenuScreenState())
}