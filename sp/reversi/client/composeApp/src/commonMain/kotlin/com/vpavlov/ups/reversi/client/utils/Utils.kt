package com.vpavlov.ups.reversi.client.utils

import io.ktor.utils.io.ByteReadChannel

val VALID_USERNAME = Regex("^[a-zA-Z][a-zA-Z0-9_]*$");

fun isValidUsername(username: String) = VALID_USERNAME.matches(username)

suspend fun ByteReadChannel.readExactChars(readSize: Int): String{
    val buffer = ByteArray(readSize)
    this.readFully(buffer, 0, readSize)
    return String(buffer, Charsets.UTF_8)
}