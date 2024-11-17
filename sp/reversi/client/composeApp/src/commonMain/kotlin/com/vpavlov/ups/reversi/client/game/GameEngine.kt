package com.vpavlov.ups.reversi.client.game

import com.vpavlov.ups.reversi.client.domains.game.MoveCoordinates
import com.vpavlov.ups.reversi.client.domains.game.PlayerCode

class GameEngine(
    gameConfig: GameConfig,
    boardCells: List<Int>? = null
) {

    val board: Board = Board(rows = gameConfig.boardWidth, cols = gameConfig.boardHeight)

    init{
        //initializedBoard(DEFAULT_INIT_X, DEFAULT_INIT_Y, boardCells)
        if (boardCells != null){
            board.copyValues(boardCells)
        }

    }

    private fun initializedBoard(initX: Int, initY: Int, boardCells: List<Int>?){
        if (boardCells == null){
            board.setAt(initX, initY, PlayerCode.WHITE_PLAYER)
            board.setAt(initX+ 1, initY, PlayerCode.BLACK_PLAYER)
            board.setAt(initX, initY + 1, PlayerCode.BLACK_PLAYER)
            board.setAt(initX + 1, initY + 1, PlayerCode.WHITE_PLAYER)
        } else {
            board.copyValues(boardCells)
        }
    }

    fun getPossibleMovesCount(player: PlayerCode): Int {
        val (movesCount, _) = getPossibleMoves(player)
        return movesCount
    }

    fun getPossibleMoves(player: PlayerCode): Pair<Int, Array<Boolean>> {
        val moves = Array(board.rows * board.cols) { false }
        var movesCount = 0;
        for (x in 0..<board.cols) {
            for (y in 0..<board.rows) {
                if (isValidMove(MoveCoordinates(x, y), player)) {
                    moves[y * board.cols + x] = true
                    ++movesCount
                }
            }
        }
        return Pair(movesCount, moves)
    }

    fun isValidMove(move: MoveCoordinates, player: PlayerCode): Boolean {
        if (move.x !in 0..<board.cols || move.y !in 0..<board.rows) {
            return false
        }
        if (board.getAt(move.x, move.y) != PlayerCode.NO_PLAYER || player == PlayerCode.NO_PLAYER) {
            return false
        }
        /*Check throw all possible directions*/
        for (dirX in -1..1) {
            for (dirY in -1..1) {
                if (countSteps(move.x, move.y, dirX, dirY, player) > 0) {
                    return true
                }
            }
        }
        return false
    }

    private fun countSteps(x: Int, y: Int, dirX: Int, dirY: Int, player: PlayerCode): Int {
        val opponent = player.getOpponent()
        var steps = 1
        if (dirX == 0 && dirY == 0) {
            return 0
        }
        /*Check if move is in bounds and an opponent cell*/
        if ((x + dirX) !in 0..<board.cols || (y + dirY) !in 0..<board.rows || board.getAt(
                x + dirX,
                y + dirY
            ) != opponent
        ) {
            return 0
        }
        var posX = x + steps * dirX
        var posY = y + steps * dirY
        /*Check specified direction for a valid sequence of opponent cells*/
        while (posX in 0..<board.cols && posY in 0..<board.rows) {
            val cell = board.getAt(posX, posY)
            if (cell == PlayerCode.NO_PLAYER) {
                return 0
            }
            if (cell == player) {
                return steps
            }
            ++steps
            posX = x + steps * dirX
            posY = y + steps * dirY
        }
        return 0
    }

    fun countPlayersScores(): Pair<Int, Int> {
        var bpScores = 0
        var wpScores = 0
        for (x in 0..<board.cols) {
            for (y in 0..<board.rows) {
                val cell = board.getAt(x, y)
                when (cell) {
                    PlayerCode.BLACK_PLAYER -> ++bpScores
                    PlayerCode.WHITE_PLAYER -> ++wpScores
                    PlayerCode.NO_PLAYER -> {}
                }
            }
        }

        return Pair(wpScores, bpScores)
    }

    private fun makeMove(x: Int, y: Int, player: PlayerCode): Int {
        var steps = 0
        for (dirX in -1..1) {
            for (dirY in -1..1) {
                val tmpSteps = countSteps(x, y, dirX, dirY, player)
                steps += tmpSteps
                for (i in 1..tmpSteps) {
                    board.setAt(x + i * dirX, y + i * dirY, player)
                }
            }
        }
        return steps
    }

    fun processMove(move: MoveCoordinates, player: PlayerCode): Boolean {
        if (!isValidMove(move, player)) {
            return false
        }
        makeMove(move.x, move.y, player)
        return true
    }


}