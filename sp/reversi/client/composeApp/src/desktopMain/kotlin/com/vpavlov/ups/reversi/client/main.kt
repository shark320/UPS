package com.vpavlov.ups.reversi.client

import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Done
import androidx.compose.material.icons.filled.Face
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.darkColorScheme
import androidx.compose.ui.graphics.vector.rememberVectorPainter
import androidx.compose.ui.window.Window
import androidx.compose.ui.window.WindowState
import androidx.compose.ui.window.application
import com.vpavlov.ups.reversi.client.connection.message.LIST_PATTERN
import com.vpavlov.ups.reversi.client.presentation.navigation.NavigationBase
import com.vpavlov.ups.reversi.client.presentation.navigation.ScreenNavigation

fun main() = application {
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