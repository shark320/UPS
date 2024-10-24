package com.vpavlov.ups.reversi.client.config

import java.util.Properties

class ConnectionConfig(prop: Properties): Configuration(prop = prop) {
    val port: Int

    val ip: String

    val identifier: String

    init{
        port = get("port")!!.toInt()
        ip = get("ip")!!
        identifier = get("identifier")!!
    }
}