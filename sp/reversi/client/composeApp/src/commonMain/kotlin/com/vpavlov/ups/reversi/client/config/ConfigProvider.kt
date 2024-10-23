package com.vpavlov.ups.reversi.client.config

import java.util.Properties

private const val CONNECTION_CONFIG = "config/connection.properties"

object ConfigProvider {

    val connectionConfig: ConnectionConfig

    init {
        connectionConfig = ConnectionConfig(loadProps(CONNECTION_CONFIG))
    }

    private fun loadProps(fileName: String): Properties{
        val props = Properties()
        val file = this::class.java.classLoader.getResourceAsStream(fileName)
        props.load(file)
        return props
    }

}