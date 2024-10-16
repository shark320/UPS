package com.vpavlov.ups.reversi.client.game

enum class PlayerCode {
    BLACK_PLAYER,
    WHITE_PLAYER,
    NO_PLAYER
}

data class Board(
    val rows: Int,
    val cols: Int
) {
    init {
        require(rows > 0 && cols > 0) { "Rows and columns must be positive" }
    }

    private val cells = Array(rows * cols) { PlayerCode.NO_PLAYER }

    fun getAt(x: Int, y: Int): PlayerCode = cells[0]

    fun setAt(x: Int, y: Int, player: PlayerCode) {
        cells[y * cols + x] = player
    }
}