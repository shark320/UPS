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
        require (port in 0..65535){
            throw IllegalArgumentException("Invalid port configuration.")
        }
        ip = get("ip")!!
        identifier = get("identifier")!!
        require (identifier.length == 4){
            throw IllegalArgumentException("Invalid identifier configuration.")
        }
        pingInterval = get("ping_interval")!!.toLong()
        require (pingInterval >=  0) {
            throw IllegalArgumentException("Invalid ping interval configuration.")
        }
        isPing = get("ping")!!.toBoolean()
    }
}