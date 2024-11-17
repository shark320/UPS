package com.vpavlov.ups.reversi.client.service.processor

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.domains.connection.message.Header
import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.domains.connection.message.Payload
import com.vpavlov.ups.reversi.client.domains.connection.message.Status
import com.vpavlov.ups.reversi.client.domains.connection.message.StatusGroup
import com.vpavlov.ups.reversi.client.domains.connection.message.Subtype
import com.vpavlov.ups.reversi.client.domains.connection.message.Type
import com.vpavlov.ups.reversi.client.domains.game.MoveCoordinates
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.GameStateService
import com.vpavlov.ups.reversi.client.service.api.state.UserMessageStateService
import com.vpavlov.ups.reversi.client.state.UserMessage
import com.vpavlov.ups.reversi.client.utils.requireAllNotNull

class GameMoveProcessor(
    private val config: ConnectionConfig,
    private val gameStateService: GameStateService,
    clientStateService: ClientStateService,
    connectionService: ConnectionService,
    userMessageStateService: UserMessageStateService
) : CommonClientProcessor(
    connectionService = connectionService,
    userMessageStateService = userMessageStateService,
    clientStateService = clientStateService,
) {

    operator fun invoke(moveCoordinates: MoveCoordinates) =
        processWithResult(MoveProcessingResult.UNEXPECTED_ERROR) {
            LOGGER.debug("Processing game move.")
            val requestHeader = Header(
                type = Type.POST,
                identifier = config.identifier,
                subtype = Subtype.GAME_MOVE
            )
            val requestPayload = Payload()
            requestPayload.setValue("x", moveCoordinates.x)
            requestPayload.setValue("y", moveCoordinates.y)
            val response =
                connectionService.exchange(Message(header = requestHeader, payload = requestPayload))
            return@processWithResult when (response.getStatusGroup()) {
                StatusGroup.SUCCESS -> handleOk(response)
                StatusGroup.REDIRECT -> {
                    unexpectedStatus(response)
                    MoveProcessingResult.UNEXPECTED_ERROR
                }
                StatusGroup.CLIENT_ERROR -> handleError(response)
                StatusGroup.SERVER_ERROR -> {
                    unexpectedStatus(response)
                    MoveProcessingResult.UNEXPECTED_ERROR
                }
                StatusGroup.UNDEFINED -> {
                    unexpectedStatus(response)
                    MoveProcessingResult.UNEXPECTED_ERROR
                }
            }
        }

    private fun handleError(response: Message): MoveProcessingResult {
        when (response.header.status) {
            Status.CONFLICT -> {
                userMessageStateService.showError(
                    userMessage = UserMessage(
                        message = "The move can not be processed. It's opponent's turn now."
                    )
                )
                return MoveProcessingResult.WRONG_PLAYER
            }

            Status.NOT_ALLOWED -> {
                userMessageStateService.showError(
                    userMessage = UserMessage(
                        message = "The move can not be processed. The move coordinates are invalid."
                    )
                )
                return MoveProcessingResult.INVALID_MOVE
            }

            else -> unexpectedErrorStatus(response)
        }
        getAndUpdateState(response)
        return MoveProcessingResult.UNEXPECTED_ERROR
    }

    private fun handleOk(response: Message): MoveProcessingResult {
        val x = response.payload.getIntegerOrNull("x")
        val y = response.payload.getIntegerOrNull("y")
        val currentPlayerUsername = response.payload.getStringValue("current_player")
        val boardCells = response.payload.getListOfIntegers("board")


        if (!requireAllNotNull(x, y, currentPlayerUsername, boardCells)) {
            malformedResponse(
                subtype = response.header.subtype,
            )
            return MoveProcessingResult.UNEXPECTED_ERROR
        }

        val currentPlayer = gameStateService.getStateFlow().value!!.game.getPlayer(currentPlayerUsername!!)
        if (currentPlayer == null){
            LOGGER.error("Can not get current player by username '$currentPlayerUsername'!")
            return MoveProcessingResult.UNEXPECTED_ERROR
        }


        gameStateService.updateState(
            lastMoveCoordinates = MoveCoordinates(x!!, y!!),
            currentPlayer = currentPlayer,
             boardCells = boardCells
        )

        return MoveProcessingResult.OK

    }

}

enum class MoveProcessingResult {
    INVALID_MOVE,
    UNEXPECTED_ERROR,
    WRONG_PLAYER,
    OK
}