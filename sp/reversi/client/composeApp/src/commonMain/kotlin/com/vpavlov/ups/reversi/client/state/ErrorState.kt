package com.vpavlov.ups.reversi.client.state

data class ErrorState(
    val isError: Boolean = false,
    val errorMessage: ErrorMessage? = null,
    val isFatal: Boolean = false,
    val initialException: Exception? = null
)

data class ErrorMessage(
    val errorMessage: String = "An error occured",
    val okButton: String = "Ok"
)