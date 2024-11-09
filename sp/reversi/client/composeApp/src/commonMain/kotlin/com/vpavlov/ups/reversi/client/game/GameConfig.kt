package com.vpavlov.ups.reversi.client.game

const val DEFAULT_BOARD_WIDTH = 8
const val DEFAULT_BOARD_HEIGHT = 8
const val DEFAULT_INIT_X = 3
const val DEFAULT_INIT_Y = 3

data class GameConfig(
    val boardWidth: Int = DEFAULT_BOARD_WIDTH,
    val boardHeight: Int = DEFAULT_BOARD_HEIGHT,
    val initX: Int = DEFAULT_INIT_X,
    val initY: Int = DEFAULT_INIT_Y
){
    init{
        require(boardWidth > 0 && boardHeight > 0 && initX >= 0 && initX < boardWidth && initY >= 0 && initY < boardHeight){
            "Invalid game config values."
        }
    }
}
