package com.vpavlov.ups.reversi.client

import androidx.compose.ui.window.Window
import androidx.compose.ui.window.application

fun main() = application {
    Window(
        onCloseRequest = ::exitApplication,
        title = "client",
    ) {
        App()
    }
}