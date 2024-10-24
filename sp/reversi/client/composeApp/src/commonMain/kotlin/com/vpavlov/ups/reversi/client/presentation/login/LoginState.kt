package com.vpavlov.ups.reversi.client.presentation.login

data class LoginState(
    val username: String = "",
    val usernameError: Boolean = false,
    val validUsername: Boolean = false,
    val waitingResponse: Boolean = false,
    val loggedIn: Boolean = false,
    val errorMsg: String? = null
)
