package com.vpavlov.ups.reversi.client.state

data class ErrorState(
    val isError: Boolean = false,
    val errorMessage: String = "",
    val fatal: Boolean = false
)