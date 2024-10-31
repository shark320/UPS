package com.vpavlov.ups.reversi.client.presentation.menu

import com.vpavlov.ups.reversi.client.state.LobbyInfo
import javax.swing.text.StyledEditorKit.BoldAction

data class MenuScreenState(
    val lobbies: List<LobbyInfo> = emptyList(),
    val lobbyNameInputState: LobbyNameInputState = LobbyNameInputState()
)

data class LobbyNameInputState(
    val name: String = "",
    val isValidName: Boolean = false,
    val isNameError: Boolean = false
)
