package com.vpavlov.ups.reversi.client.state

enum class MessageType{
    INFO,
    ERROR,
    CONFIRMATION
}

data class ShowMessageState(
    val messageType: MessageType = MessageType.INFO,
    val isShowMessage: Boolean = false,
    val message: UserMessage? = null,
    val isFatal: Boolean = false,
    val initialException: Exception? = null
)

data class UserMessage(
    val message: String = "",
    val okButton: String = "Ok"
)