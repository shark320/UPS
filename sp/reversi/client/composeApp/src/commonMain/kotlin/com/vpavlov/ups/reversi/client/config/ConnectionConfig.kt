package com.vpavlov.ups.reversi.client.config

import java.util.Properties

private const val CONFIG = "connection.properties"

object ConnectionConfig {
    private val properties = Properties()

    val ip: String

    val port: Int

    init {
        val file = this::class.java.classLoader.getResourceAsStream(CONFIG)
        properties.load(file)
        ip = get("ip")
        port = get("port").toInt()
    }
    
    fun get(key: String): String = properties.getProperty(key)
}