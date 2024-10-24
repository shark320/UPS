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
import org.apache.logging.log4j.kotlin.logger


fun main(args: Array<String>) = application {
    val argsSet = args.toSet()
    System.setProperty("log4j.configurationFile", "config/logging/log4j.xml")
    val logger = logger("main")
    logger.debug("Run arguments: $argsSet")
    initKoin(argsSet.contains("-offline"))
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
//private val ALLOWED_TYPES = setOf(
//    String::class,
//    Int::class,
//    Boolean::class,
//    List::class
//)
//
//fun main(){
//    val str = "sdfdsf"
//
//    println(str::class in ALLOWED_TYPES)
//}