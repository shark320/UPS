package com.vpavlov.ups.reversi.client.utils

import io.ktor.utils.io.ByteReadChannel
import kotlin.reflect.KClass

val VALID_USERNAME = Regex("^[a-zA-Z][a-zA-Z0-9_]*$")

val VALID_LOBBY_NAME = VALID_USERNAME

fun isValidUsername(username: String) = VALID_USERNAME.matches(username)

fun isValidLobbyName(username: String) = VALID_LOBBY_NAME.matches(username)


suspend fun ByteReadChannel.readExactChars(readSize: Int): String{
    val buffer = ByteArray(readSize)
    this.readFully(buffer, 0, readSize)
    return String(buffer, Charsets.UTF_8)
}

fun checkListTypes(list: List<*>, type: KClass<out Any>): Boolean {
    return list.all { it != null && it::class == type }
}

fun checkListTypes(list: List<*>, types: Set<KClass<out Any>>): Boolean {
    return list.all { it != null && it::class in types }
}

fun requireAllNotNull(vararg vars: Any?): Boolean{
    return vars.all { it != null }
}