package com.vpavlov.ups.reversi.client.config

import com.vpavlov.ups.reversi.client.utils.EnvironmentVariables
import java.io.File
import java.io.InputStream
import java.util.Properties

private const val CONNECTION_CONFIG = "config/connection.properties"

object ConfigProvider {

    val connectionConfig: ConnectionConfig

    init {
        connectionConfig = ConnectionConfig(loadProps(CONNECTION_CONFIG))
    }

    private fun loadProps(fileName: String): Properties{
        val props = Properties()
        val file = if (EnvironmentVariables.isIDEStart) this::class.java.classLoader.getResourceAsStream(fileName) else File(fileName).inputStream()
        props.load(file)
        return props
    }

}