package com.vpavlov.ups.reversi.client.state

import com.vpavlov.ups.reversi.client.domains.game.Game
import com.vpavlov.ups.reversi.client.domains.game.Lobby

data class ClientState(
    val isLoggedIn: Boolean = false,
    val username: String,
    val flowState: ClientFlowState,
    val lobbiesList: List<LobbyInfo> = emptyList(),
    val currentLobby: Lobby? = null,
)

enum class ClientFlowState{
    MENU,
    LOBBY,
    GAME;

    companion object{
        private val entriesMap = entries.associateBy { it.toString() }
        fun getValueOrNull(key: String?): ClientFlowState?{
            key ?: return null
            return entriesMap[key]
        }
    }
}

data class LobbyInfo(
    val lobbyName: String,
    val lobbyHost: String
)
