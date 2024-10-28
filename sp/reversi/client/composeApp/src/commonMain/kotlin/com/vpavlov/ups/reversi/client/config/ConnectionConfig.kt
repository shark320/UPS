package com.vpavlov.ups.reversi.client.config

import java.util.Properties

class ConnectionConfig(prop: Properties): Configuration(prop = prop) {
    val port: Int

    val ip: String

    val identifier: String

    val pingInterval: Long

    val isPing: Boolean

    init{
        port = get("port")!!.toInt()
        ip = get("ip")!!
        identifier = get("identifier")!!
        pingInterval = get("ping_interval")!!.toLong()
        isPing = get("ping")!!.toBoolean()
    }
}