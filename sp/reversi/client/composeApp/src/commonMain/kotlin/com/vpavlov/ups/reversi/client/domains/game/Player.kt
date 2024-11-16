package com.vpavlov.ups.reversi.client.domains.game

enum class PlayerCode(val code: Int) {
    BLACK_PLAYER(-1),
    WHITE_PLAYER(1),
    NO_PLAYER(0);

    fun getOpponent(): PlayerCode = when(this){
        BLACK_PLAYER -> WHITE_PLAYER
        WHITE_PLAYER -> BLACK_PLAYER
        NO_PLAYER -> NO_PLAYER
    }

    companion object{
        fun getValueOrNull(key: String): PlayerCode? {
            val codes = PlayerCode.values()
            return codes.firstOrNull { code ->
                code.name == key
            }
        }

        fun getFromCodeOrNull(code: Int): PlayerCode?{
            return PlayerCode.values().firstOrNull { value ->
                value.code == code
            }
        }
    }

}


data class Player(
    val code: PlayerCode,
    val username: String
)