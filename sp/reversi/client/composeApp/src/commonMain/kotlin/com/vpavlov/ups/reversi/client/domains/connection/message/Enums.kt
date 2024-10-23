package com.vpavlov.ups.reversi.client.domains.connection.message


enum class Type(val id: Int) {
    GET(1),
    POST(2),
    NULL_TYPE(-1)
    ;

    companion object {
        private val values: MutableMap<Int, Type> = HashMap()

        init {
            for (type in entries) {
                values[type.id] = type
            }
        }

        fun getType(id: Int): Type? {
            return values[id]
        }
    }
}

enum class Subtype(val id: Int) {
    PING(1),
    LOGIN(2),
    CREATE_GAME(3),
    LOBBY_EXIT(4),
    LOBBIES_LIST(7),
    LOBBY_CONNECT(8),
    HANDSHAKE(12),
    NULL_SUBTYPE(-1)
    ;

    companion object {
        private val values: MutableMap<Int, Subtype> = HashMap()

        init {
            for (type in entries) {
                values[type.id] = type
            }
        }

        fun getSubtype(id: Int): Subtype? {
            return values[id]
        }
    }
}

enum class Status(val code: Int) {
    NO_STATUS(0),
    OK(200),
    NOT_FOUND(404),
    CONFLICT(409),
    NOT_ALLOWED(405);

    companion object {
        private val values: MutableMap<Int, Status> = HashMap()

        init {
            for (status in entries) {
                values[status.code] = status
            }
        }

        fun getStatus(code: Int): Status? {
            return values[code]
        }
    }
}