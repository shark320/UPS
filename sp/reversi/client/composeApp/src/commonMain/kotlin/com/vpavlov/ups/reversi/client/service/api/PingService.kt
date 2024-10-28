package com.vpavlov.ups.reversi.client.service.api

interface PingService {

    fun start()

    fun stop()

    fun resume()

    fun pause()

    fun isRunning(): Boolean
}