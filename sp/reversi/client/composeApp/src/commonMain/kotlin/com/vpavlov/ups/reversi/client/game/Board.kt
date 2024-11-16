package com.vpavlov.ups.reversi.client.game

import com.vpavlov.ups.reversi.client.domains.game.PlayerCode


data class Board(
    val rows: Int,
    val cols: Int
) {
    init {
        require(rows > 0 && cols > 0) { "Rows and columns must be positive" }
    }

    private val cells = Array(rows * cols) { PlayerCode.NO_PLAYER }

    fun getAt(x: Int, y: Int): PlayerCode = cells[y * cols + x]

    fun setAt(x: Int, y: Int, player: PlayerCode) {
        cells[y * cols + x] = player
    }

    fun copyValues(cells: List<Int>) {
        cells.forEachIndexed { index, cell ->
            this.cells[index] = PlayerCode.getFromCodeOrNull(cell)
                ?: throw IllegalArgumentException("Could not parse player code from value '$cell'!")
        }
    }


}