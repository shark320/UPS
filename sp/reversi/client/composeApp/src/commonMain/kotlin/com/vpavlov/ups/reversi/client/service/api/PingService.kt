package com.vpavlov.ups.reversi.client.service.api

interface PingService {

    fun start()

    fun stop()

    fun isRunning(): Boolean
}