package com.vpavlov.ups.reversi.client.presentation.menu

import androidx.compose.runtime.MutableState
import androidx.compose.runtime.mutableStateOf
import androidx.lifecycle.viewModelScope
import com.vpavlov.ups.reversi.client.presentation.common.viewModel.CommonScreenViewModel
import com.vpavlov.ups.reversi.client.service.api.PingService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.state.ClientState
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach

class MenuScreenViewModel(
    connectionStateService: ConnectionStateService,
    errorStateService: ErrorStateService,
    private val pingService: PingService,
    private val clientStateService: ClientStateService
): CommonScreenViewModel<MenuScreenEvent, MenuScreenState>(
    errorStateService = errorStateService,
    connectionStateService = connectionStateService
) {

    init{
        pingService.start()

        clientStateService.getStateFlow().onEach { clientState ->
            clientStateUpdated(clientState!!)
        }.launchIn(viewModelScope)
    }

    private fun clientStateUpdated(clientState: ClientState){
       _state.value = state.value.copy(
           lobbies = clientState.lobbiesList
       )
    }

    override fun onEvent(event: MenuScreenEvent) {

    }

    override fun initState(): MutableState<MenuScreenState>  = mutableStateOf(MenuScreenState())
}