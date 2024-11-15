package com.vpavlov.ups.reversi.client.presentation.game

import com.vpavlov.ups.reversi.client.domains.game.MoveCoordinates

sealed interface GameScreenEvent {

    data class PlayerMove(val moveCoordinates: MoveCoordinates): GameScreenEvent

    data object LeaveGame: GameScreenEvent
}