package com.vpavlov.ups.reversi.client.domains.game

enum class PlayerCode {
    BLACK_PLAYER,
    WHITE_PLAYER,
    NO_PLAYER;

    fun getOpponent(): PlayerCode = when(this){
        BLACK_PLAYER -> WHITE_PLAYER
        WHITE_PLAYER -> BLACK_PLAYER
        NO_PLAYER -> NO_PLAYER
    }
}


data class Player(
    val code: PlayerCode,
    val username: String
)