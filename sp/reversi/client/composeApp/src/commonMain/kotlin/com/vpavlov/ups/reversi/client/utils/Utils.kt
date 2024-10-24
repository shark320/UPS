package com.vpavlov.ups.reversi.client.utils

val VALID_USERNAME = Regex("^[a-zA-Z][a-zA-Z0-9_]*$");

fun isValidUsername(username: String) = VALID_USERNAME.matches(username)