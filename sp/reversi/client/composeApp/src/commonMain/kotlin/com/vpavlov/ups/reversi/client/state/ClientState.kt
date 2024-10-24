package com.vpavlov.ups.reversi.client.state

data class ClientState(
    val username: String,
    val flowState: ClientFlowState
)

enum class ClientFlowState{
    MENU,
    LOBBY,
    GAME
}
