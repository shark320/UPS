package com.vpavlov.ups.reversi.client.presentation.menu

import androidx.compose.runtime.MutableState
import androidx.compose.runtime.mutableStateOf
import androidx.lifecycle.viewModelScope
import com.vpavlov.ups.reversi.client.presentation.common.viewModel.CommonScreenViewModel
import com.vpavlov.ups.reversi.client.service.api.PingService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.UserMessageStateService
import com.vpavlov.ups.reversi.client.service.processor.ConnectToLobbyProcessor
import com.vpavlov.ups.reversi.client.service.processor.CreateLobbyProcessor
import com.vpavlov.ups.reversi.client.service.processor.LogoutProcessor
import com.vpavlov.ups.reversi.client.state.ClientState
import com.vpavlov.ups.reversi.client.utils.isValidLobbyName
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach

class MenuScreenViewModel(
    connectionStateService: ConnectionStateService,
    userMessageStateService: UserMessageStateService,
    private val pingService: PingService,
    private val connectToLobbyProcessor: ConnectToLobbyProcessor,
    private val logoutProcessor: LogoutProcessor,
    clientStateService: ClientStateService,
    private val createLobbyProcessor: CreateLobbyProcessor,
) : CommonScreenViewModel<MenuScreenEvent, MenuScreenState>(
    userMessageStateService = userMessageStateService,
    connectionStateService = connectionStateService,
    clientStateService = clientStateService
) {

    init {
        pingService.start()

        clientStateService.getStateFlow().onEach { clientState ->
            if (clientState != null){
                clientStateUpdated(clientState)
            }
        }.launchIn(viewModelScope)
    }

    private fun clientStateUpdated(clientState: ClientState) {
        _state.value = state.value.copy(
            lobbies = clientState.lobbiesList
        )
    }

    override fun onEvent(event: MenuScreenEvent) {
        when (event) {
            is MenuScreenEvent.ConnectToLobby -> connectToLobby(event.lobbyName)
            is MenuScreenEvent.LobbyNameEntered -> lobbyNameEntered(event.lobbyName)
            is MenuScreenEvent.CreateNewLobby -> createNewLobby(event.lobbyName)
            is MenuScreenEvent.LobbyNameInputCancelled -> {
                _state.value = state.value.copy(
                    lobbyNameInputState = LobbyNameInputState()
                )
            }
            is MenuScreenEvent.Logout -> logout()
        }
    }

    private fun logout(){
        logoutProcessor().onEach { finished ->
            defaultFinishedHandler(finished)
        }.launchIn(viewModelScope)
    }

    private fun lobbyNameEntered(lobbyName: String) {
        _state.value = state.value.copy(
            lobbyNameInputState = LobbyNameInputState(
                name = lobbyName,
                isValidName = isValidLobbyName(lobbyName),
                isNameError = lobbyName.isNotEmpty() && !isValidLobbyName(lobbyName)
            )
        )
    }

    private fun createNewLobby(lobbyName: String) {
        if (!isValidLobbyName(lobbyName)){
            return
        }
        createLobbyProcessor(lobbyName).onEach { finished ->
            defaultFinishedHandler(finished)
        }.launchIn(viewModelScope)
    }

    private fun connectToLobby(lobby: String) {
        connectToLobbyProcessor(lobby).onEach { finished ->
            defaultFinishedHandler(finished)
        }.launchIn(viewModelScope)
    }

    override fun initState(): MutableState<MenuScreenState> = mutableStateOf(MenuScreenState())
}