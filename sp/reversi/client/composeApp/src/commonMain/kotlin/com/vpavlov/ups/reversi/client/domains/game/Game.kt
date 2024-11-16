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
    players: List<Player>,
    boardCells: List<Int>?
) {

    private lateinit var whitePlayer: Player

    private lateinit var blackPlayer: Player

    private val gameConfig = GameConfig()

    private var currentPlayer: Player

    private val gameEngine = GameEngine(
        gameConfig = gameConfig,
        boardCells = boardCells
    )

    private var lastMove = MoveCoordinates(
        x = DEFAULT_INIT_X,
        y = DEFAULT_INIT_Y
    )

    val board: Board
        get() = gameEngine.board


    init {
        if (players.size != 2){
            throw IllegalArgumentException("Should be exactly 2 players.")
        }
        var isBlackSet = false
        var isWhiteSet = false
        players.forEach { player->

            when(player.code){
                PlayerCode.BLACK_PLAYER -> {
                    if (isBlackSet){
                        throw IllegalArgumentException("Player with code '${player.code}' is duplicated")
                    }
                    blackPlayer = player
                    isBlackSet = true
                }
                PlayerCode.WHITE_PLAYER -> {
                    if (isWhiteSet){
                        throw IllegalArgumentException("Player with code '${player.code}' is duplicated")
                    }
                    whitePlayer = player
                    isWhiteSet = true
                }
                PlayerCode.NO_PLAYER -> throw IllegalArgumentException("Player with code '${player.code}' could not be processed.")
            }
        }
        currentPlayer = whitePlayer
    }


    fun isLastMoveChanged(getLastMove: MoveCoordinates) = lastMove != getLastMove

    fun isCurrentPlayerChanged(getCurrentPlayerUsername: String) = currentPlayer.username != getCurrentPlayerUsername

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

    fun getPlayer(username: String): Player?{
        if (whitePlayer.username == username){
            return whitePlayer
        }
        if (blackPlayer.username == username){
            return blackPlayer
        }
        return null
    }

    fun getPossibleMoves(player: Player): Array<Boolean> = gameEngine.getPossibleMoves(player.code).second

    fun getCurrentPlayer() = this.currentPlayer
}