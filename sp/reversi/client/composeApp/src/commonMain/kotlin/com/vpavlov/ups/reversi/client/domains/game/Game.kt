package com.vpavlov.ups.reversi.client.domains.game

import com.vpavlov.ups.reversi.client.game.Board
import com.vpavlov.ups.reversi.client.game.DEFAULT_INIT_X
import com.vpavlov.ups.reversi.client.game.DEFAULT_INIT_Y
import com.vpavlov.ups.reversi.client.game.GameConfig
import com.vpavlov.ups.reversi.client.game.GameEngine

data class MoveCoordinates(
    val x: Int,
    val y: Int
)

class Game(
    val whitePlayer: Player,
    val blackPlayer: Player,
) {

    val gameConfig = GameConfig()

    var currentPlayer = whitePlayer

    val gameEngine = GameEngine(
        gameConfig = gameConfig
    )

    var lastMove = MoveCoordinates(
        x = DEFAULT_INIT_X,
        y = DEFAULT_INIT_Y
    )

    val board: Board
        get() = gameEngine.board

    fun isLastMoveChanged(getLastMove: MoveCoordinates) = lastMove == getLastMove

    fun isCurrentPlayerChanged(getCurrentPlayerUsername: String) = currentPlayer.username == getCurrentPlayerUsername

    fun performMoveForCurrentPlayer(move: MoveCoordinates): Boolean{
        val isSuccess = gameEngine.processMove(move, currentPlayer.code)
        if (isSuccess){
            currentPlayer = getOpponent(currentPlayer)
            lastMove = move
        }
        return isSuccess
    }

    fun getOpponent(player: Player): Player {
        return when(player.code){
            PlayerCode.BLACK_PLAYER -> this.whitePlayer
            PlayerCode.WHITE_PLAYER -> this.blackPlayer
            PlayerCode.NO_PLAYER -> throw IllegalArgumentException("Invalid player code [${player.code}] to get opponent!")
        }
    }

    fun isMoveValid(move: MoveCoordinates) = gameEngine.isValidMove(move, currentPlayer.code)


}