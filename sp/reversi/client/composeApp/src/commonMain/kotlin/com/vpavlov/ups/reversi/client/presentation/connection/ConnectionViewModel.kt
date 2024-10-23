package com.vpavlov.ups.reversi.client.presentation.connection

import androidx.lifecycle.ViewModel
import com.vpavlov.ups.reversi.client.di.koin
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.ConnectionStateService

class ConnectionViewModel: ViewModel() {

    val connectionStateService: ConnectionStateService = koin.get()

    private val connectionService: ConnectionService = koin.get()

    val connectionState = connectionStateService.getConnectionState()

    init {
        connectionService.connect()
    }
}