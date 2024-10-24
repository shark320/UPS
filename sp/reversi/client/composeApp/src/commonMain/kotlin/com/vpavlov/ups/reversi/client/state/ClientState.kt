package com.vpavlov.ups.reversi.client.state

data class ClientState(
    val username: String,
    val flowState: ClientFlowState
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
