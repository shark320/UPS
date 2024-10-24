package com.vpavlov.ups.reversi.client.presentation.login

sealed interface LoginEvent {

    data class UsernameEntered(val username: String): LoginEvent

    data object ProcessLogin: LoginEvent
}