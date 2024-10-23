package com.vpavlov.ups.reversi.client.config

import java.util.Properties

open class Configuration(private val prop: Properties) {

    fun get(key: String): String? = prop.getProperty(key)
}