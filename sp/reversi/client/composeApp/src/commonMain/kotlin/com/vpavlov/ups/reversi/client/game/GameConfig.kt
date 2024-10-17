package com.vpavlov.ups.reversi.client.game

data class GameConfig(
    val boardWidth: Int = 8,
    val boardHeight: Int = 8,
    val initX: Int = 3,
    val initY: Int = 3
){
    init{
        require(boardWidth > 0 && boardHeight > 0 && initX >= 0 && initX < boardWidth && initY >= 0 && initY < boardHeight){
            "Invalid game config values."
        }
    }
}
