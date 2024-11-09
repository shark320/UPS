package com.vpavlov.ups.reversi.client.domains.connection.message

import com.vpavlov.ups.reversi.client.domains.connection.MSG_HEADER_LENGTH
import com.vpavlov.ups.reversi.client.domains.connection.MSG_IDENTIFIER_FIELD_LENGTH
import com.vpavlov.ups.reversi.client.domains.connection.MSG_IDENTIFIER_FIELD_POS
import com.vpavlov.ups.reversi.client.domains.connection.MSG_LENGTH_FIELD_LENGTH
import com.vpavlov.ups.reversi.client.domains.connection.MSG_LENGTH_FIELD_POS
import com.vpavlov.ups.reversi.client.domains.connection.MSG_MAX_LENGTH
import com.vpavlov.ups.reversi.client.domains.connection.MSG_STATUS_FIELD_LENGTH
import com.vpavlov.ups.reversi.client.domains.connection.MSG_STATUS_FIELD_POS
import com.vpavlov.ups.reversi.client.domains.connection.MSG_SUBTYPE_FIELD_LENGTH
import com.vpavlov.ups.reversi.client.domains.connection.MSG_SUBTYPE_FIELD_POS
import com.vpavlov.ups.reversi.client.domains.connection.MSG_TYPE_FIELD_LENGTH
import com.vpavlov.ups.reversi.client.domains.connection.MSG_TYPE_FIELD_POS

data class Header(
    var identifier: String,
    var type: Type,
    var subtype: Subtype,
    var status: Status = Status.OK,
    private var _length: Int = 0,
) {
    var length: Int
        get() = _length
        @Throws(IllegalArgumentException::class)
        set(value) {
            require(length < MSG_MAX_LENGTH || length >= 0) {
                "Length too large. Length must be: [0,$MSG_MAX_LENGTH]"
            }
            _length = value
        }

    fun construct(): String {
        val length = java.lang.String.format("%07d", _length)
        val type = String.format("%1d", type.id)
        val subType = String.format("%02d", subtype.id)
        val status = String.format("%03d", status.code)
        return identifier + length + type + subType + status
    }

    companion object{
        @Throws(IllegalArgumentException::class)
        fun parse(message: String): Header {
            require(message.length >= MSG_HEADER_LENGTH) { "Message header is too short" }

            val identifier = message.substring(
                MSG_IDENTIFIER_FIELD_POS,
                MSG_IDENTIFIER_FIELD_LENGTH
            )
            val msgLength = message.substring(
                MSG_LENGTH_FIELD_POS,
                MSG_LENGTH_FIELD_POS + MSG_LENGTH_FIELD_LENGTH
            ).toInt()
            val msgTypeInt = message.substring(
                MSG_TYPE_FIELD_POS,
                MSG_TYPE_FIELD_POS + MSG_TYPE_FIELD_LENGTH
            ).toInt()
            val msgSubtypeInt = message.substring(
                MSG_SUBTYPE_FIELD_POS,
                MSG_SUBTYPE_FIELD_POS + MSG_SUBTYPE_FIELD_LENGTH
            ).toInt()
            val msgStatusInt = message.substring(
                MSG_STATUS_FIELD_POS,
                MSG_STATUS_FIELD_POS + MSG_STATUS_FIELD_LENGTH
            ).toInt()

            val msgType = Type.getType(msgTypeInt)
            val msgSubtype = Subtype.getSubtype(msgSubtypeInt)
            val msgStatus = Status.getStatus(msgStatusInt)
            require(msgType != null && msgSubtype != null && msgStatus != null){
                "Message string '$message' contains values which can not be parsed."
            }
            return Header(
                identifier = identifier,
                type = msgType, subtype = msgSubtype, status = msgStatus, _length = msgLength
            )
        }
    }

    fun isRedirect() = status.code in 300..399

    fun isError() = status.code in 400..499

    fun isOk() = status.code in 200..299

}