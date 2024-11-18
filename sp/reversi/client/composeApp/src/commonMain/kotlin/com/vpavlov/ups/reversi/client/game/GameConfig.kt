package com.vpavlov.ups.reversi.client.game

const val DEFAULT_BOARD_WIDTH = 5
const val DEFAULT_BOARD_HEIGHT = 5
const val DEFAULT_INIT_X = 3
const val DEFAULT_INIT_Y = 3

data class GameConfig(
    val boardWidth: Int = DEFAULT_BOARD_WIDTH,
    val boardHeight: Int = DEFAULT_BOARD_HEIGHT,
){
    init{
        require(boardWidth > 0 && boardHeight > 0){
            "Invalid game config values."
        }
    }
}
