package com.vpavlov.ups.reversi.client.domains.connection.message

data class Message(
    val header: Header,
    val payload: Payload = Payload()
){
    fun construct(): String {
        val payloadConstruct: String = payload.construct()
        header.length = payloadConstruct.length
        val headerConstruct = header.construct()
        return headerConstruct + payloadConstruct
    }

    companion object{
        fun parse(requestMsg: String): Message {
            val header: Header = Header.parse(requestMsg)
            val payload: Payload = Payload.parse(requestMsg)

            return Message(header, payload)
        }
    }

    fun isError() = header.isError()

    fun isRedirect() = header.isRedirect()
}