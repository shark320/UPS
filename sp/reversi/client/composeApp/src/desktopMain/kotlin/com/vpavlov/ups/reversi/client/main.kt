package com.vpavlov.ups.reversi.client

import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Face
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.darkColorScheme
import androidx.compose.ui.graphics.vector.rememberVectorPainter
import androidx.compose.ui.window.Window
import androidx.compose.ui.window.application
import com.vpavlov.ups.reversi.client.di.initKoin
import com.vpavlov.ups.reversi.client.presentation.navigation.NavigationBase
import com.vpavlov.ups.reversi.client.service.api.ConnectionStateService
import jdk.jfr.internal.SecuritySupport.getResourceAsStream
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import org.koin.core.context.GlobalContext.get
import java.util.Properties
import kotlin.io.use


fun main() = application {
    initKoin()
    CoroutineScope(Dispatchers.Default).launch {
        delay(5000L)
        val connectionStateService: ConnectionStateService = get().get()
        connectionStateService.updateConnectionState(isAlive = true)
    }
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