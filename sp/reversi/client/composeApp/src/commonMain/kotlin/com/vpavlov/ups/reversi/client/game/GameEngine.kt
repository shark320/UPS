package com.vpavlov.ups.reversi.client.game

class GameEngine(val gameConfig: GameConfig) {

    private val board: Board = Board(rows = gameConfig.boardWidth, cols = gameConfig.boardHeight)

    fun getPossibleMovesCount(player: PlayerCode): Int {

    }

}