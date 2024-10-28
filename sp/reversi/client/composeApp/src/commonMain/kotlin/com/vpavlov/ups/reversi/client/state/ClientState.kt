package com.vpavlov.ups.reversi.client.state

data class ClientState(
    val username: String,
    val flowState: ClientFlowState,
    val lobbiesList: List<LobbyInfo> = emptyList()
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
