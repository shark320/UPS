package com.vpavlov.ups.reversi.client.service.processor

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.domains.connection.message.Header
import com.vpavlov.ups.reversi.client.domains.connection.message.Message
import com.vpavlov.ups.reversi.client.domains.connection.message.Status
import com.vpavlov.ups.reversi.client.domains.connection.message.Subtype
import com.vpavlov.ups.reversi.client.domains.connection.message.Type
import com.vpavlov.ups.reversi.client.domains.game.Game
import com.vpavlov.ups.reversi.client.domains.game.MoveCoordinates
import com.vpavlov.ups.reversi.client.domains.game.Player
import com.vpavlov.ups.reversi.client.domains.game.PlayerCode
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.UserMessageStateService
import com.vpavlov.ups.reversi.client.service.api.state.GameStateService
import com.vpavlov.ups.reversi.client.state.ClientFlowState
import com.vpavlov.ups.reversi.client.state.UserMessage
import com.vpavlov.ups.reversi.client.utils.requireAllNotNull

class GetGameStateProcessor(
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

    operator fun invoke() = process {
        LOGGER.debug("Processing get game state.")
        val requestHeader = Header(
            type = Type.GET,
            identifier = config.identifier,
            subtype = Subtype.GAME_STATE
        )
        val response =
            connectionService.exchange(Message(header = requestHeader))
        if (response.isOk()) {
            handleOk(response)
        } else if (response.isRedirect()) {
            handleRedirect(response)
        } else {
            handleError(response)
        }
    }

    private fun handleRedirect(response: Message) {
        userMessageStateService.showInfo(
            userMessage = UserMessage(
                message = "The game is terminated! Probably the opponent leaved the game."
            )
        )
        getAndUpdateState(response)
    }

    private fun handleError(response: Message) {
        LOGGER.error("Error response: $response")
        unexpectedErrorStatus(
            response.header.status,
        )
    }

    private fun handleOk(response: Message) {
        when (response.header.status) {
            Status.OK -> handleStateUpdate(response)
            Status.RESET -> handleGameOver(response)
            else -> unexpectedStatus(response)
        }
    }

    private fun handleGameOver(response: Message) {
        val state = ClientFlowState.getValueOrNull(response.payload.getStringValue("state"))
        val playersUsernames = response.payload.getListOfStrings("players")
        val winner = response.payload.getStringValue("winner")

        if (!requireAllNotNull(
                state, playersUsernames, winner
            )
        ) {
            malformedResponse(
                subtype = response.header.subtype,
            )
            return
        }

        userMessageStateService.showInfo(
            userMessage = UserMessage(
                message = "Game is over! Winner is '$winner'"
            )
        )

        clientStateService.updateState(
            flowState = state!!
        )
    }

    private fun handleStateUpdate(response: Message) {
        val state = ClientFlowState.getValueOrNull(response.payload.getStringValue("state"))
        val playersUsernames = response.payload.getListOfStrings("players")
        val playerCodes = response.payload.getListOfStrings("player_codes")
        val isOpponentConnected = response.payload.getBooleanOrNull("is_opponent_connected")
        val currentPlayerUsername = response.payload.getStringValue("current_player")
        val lastMoveX = response.payload.getIntegerOrNull("x")
        val lastMoveY = response.payload.getIntegerOrNull("y")
        val boardCells = response.payload.getListOfIntegers("board")

        if (!requireAllNotNull(
                state,
                playersUsernames,
                playerCodes,
                isOpponentConnected,
                currentPlayerUsername,
                lastMoveX,
                lastMoveY,
                boardCells
            )
        ) {
            malformedResponse(
                subtype = response.header.subtype,
            )
            return
        }

        clientStateService.updateState(
            flowState = state!!
        )

        try {
            val players =
                retrievePlayers(usernames = playersUsernames!!, playerCodes = playerCodes!!)
            val currentPlayer = getPlayer(currentPlayerUsername!!, players)

            if (currentPlayer == null) {
                throw IllegalStateException("Could not find current player by name '$currentPlayerUsername'")
            }

            val lastMoveCoordinates = MoveCoordinates(
                x = lastMoveX!!,
                y = lastMoveY!!
            )

            if (!gameStateService.isInitialized()) {
                gameStateService.initState(
                    game = Game(
                        players = players,
                        boardCells = boardCells
                    ),
                    players = players,
                    isOpponentConnected = isOpponentConnected!!,
                    currentPlayer = currentPlayer,
                    lastMoveCoordinates = lastMoveCoordinates
                )
            } else {
                gameStateService.updateState(
                    currentPlayer = currentPlayer,
                    lastMoveCoordinates = lastMoveCoordinates,

                    players = players,
                    isOpponentConnected = isOpponentConnected!!,
                    boardCells = boardCells
                )
            }
        } catch (e: Throwable) {
            LOGGER.error("Error during 'getState' response processing", e)
            userMessageStateService.showError(
                userMessage = UserMessage(
                    message = "Error during 'getState' response processing"
                )
            )
        }


    }

    @Throws(IllegalArgumentException::class)
    private fun retrievePlayers(usernames: List<String>, playerCodes: List<String>): List<Player> {
        if (usernames.size != playerCodes.size) {
            throw IllegalStateException("Count of get usernames does not match with count of player codes")
        }
        val players = mutableListOf<Player>()
        usernames.forEachIndexed { index, username ->
            val playerCodeStr = playerCodes.get(index)
            val playerCode = PlayerCode.getValueOrNull(playerCodeStr)
                ?: throw IllegalStateException("Could not get player code from string '$playerCodeStr'")
            players.add(
                Player(
                    username = username,
                    code = playerCode
                )
            )

        }

        return players
    }

    fun getPlayer(username: String, players: List<Player>): Player? {
        return players.firstOrNull { player ->
            player.username == username
        }
    }
}