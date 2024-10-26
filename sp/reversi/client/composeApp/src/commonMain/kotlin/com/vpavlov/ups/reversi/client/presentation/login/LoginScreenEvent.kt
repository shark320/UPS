package com.vpavlov.ups.reversi.client.presentation.login

sealed interface LoginScreenEvent {

    data class UsernameEntered(val username: String): LoginScreenEvent

    data object ProcessLoginScreen: LoginScreenEvent
}