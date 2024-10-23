package com.vpavlov.ups.reversi.client

import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Face
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.darkColorScheme
import androidx.compose.ui.graphics.vector.rememberVectorPainter
import androidx.compose.ui.window.Window
import androidx.compose.ui.window.application
import com.vpavlov.ups.reversi.client.di.initKoin
import com.vpavlov.ups.reversi.client.di.koin
import com.vpavlov.ups.reversi.client.presentation.navigation.NavigationBase
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import org.apache.logging.log4j.kotlin.logger


fun main() = application {
    System.setProperty("log4j.configurationFile", "config/logging/log4j.xml")
    val logger = logger("main")
    logger.debug("dfdfd")
    initKoin()
    val connectionService: ConnectionService = koin.get()
    connectionService.connect()
//    CoroutineScope(Dispatchers.Default).launch {
//        delay(5000L)
//        val connectionStateService: ConnectionStateService = get().get()
//        connectionStateService.updateConnectionState(isAlive = true)
//    }
    Window(
        onCloseRequest = ::exitApplication,
        title = "client",
        icon = rememberVectorPainter(Icons.Filled.Face),
        visible = true
    ) {
        MaterialTheme(
            colorScheme = darkColorScheme()
        ) {
            NavigationBase()
        }

    }
}

//fun main(){
//    println(ScreenNavigation.ConnectionScreen.toString())
//}