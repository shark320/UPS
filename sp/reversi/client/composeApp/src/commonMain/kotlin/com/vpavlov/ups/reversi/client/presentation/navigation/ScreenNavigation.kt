package com.vpavlov.ups.reversi.client.presentation.navigation

import kotlinx.serialization.Serializable

sealed interface ScreenNavigation {

    @Serializable
    data object ConnectionScreen : ScreenNavigation

    @Serializable
    data object LoginScreen : ScreenNavigation

    @Serializable
    data object MenuScreen : ScreenNavigation

    @Serializable
    data object LobbyScreen : ScreenNavigation

    @Serializable
    data object GameScreen : ScreenNavigation

}