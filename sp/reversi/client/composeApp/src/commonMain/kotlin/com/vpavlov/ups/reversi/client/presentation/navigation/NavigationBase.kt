package com.vpavlov.ups.reversi.client.presentation.navigation

import androidx.compose.animation.AnimatedContent
import androidx.compose.animation.fadeIn
import androidx.compose.animation.fadeOut
import androidx.compose.animation.togetherWith
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.verticalScroll
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.navigation.NavHostController
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import com.vpavlov.ups.reversi.client.presentation.connection.ConnectionScreen
import com.vpavlov.ups.reversi.client.presentation.game.GameScreen
import com.vpavlov.ups.reversi.client.presentation.lobby.LobbyScreen
import com.vpavlov.ups.reversi.client.presentation.login.LoginScreen
import com.vpavlov.ups.reversi.client.presentation.menu.MenuScreen

@Composable
fun NavigationBase(
    navController: NavHostController = rememberNavController()
){
    Scaffold{ innerPadding ->
        AnimatedContent(
            targetState = navController.currentBackStackEntry?.destination?.route,
            transitionSpec = {
                (fadeIn(initialAlpha = 0.9f))
                    .togetherWith(fadeOut(targetAlpha = 0.9f))
            }
        ) {
            NavHost(
                navController = navController,
                startDestination = ScreenNavigation.ConnectionScreen.toString(),
                modifier = Modifier
                    .fillMaxSize()
                    .verticalScroll(rememberScrollState())
                    .padding(innerPadding)
            ) {
                composable(route = ScreenNavigation.ConnectionScreen.toString()){
                    ConnectionScreen(navController = navController)
                }
                composable(route = ScreenNavigation.LoginScreen.toString()){
                    LoginScreen(navController = navController)
                }
                composable(route = ScreenNavigation.MenuScreen.toString()){
                    MenuScreen(navController = navController)
                }
                composable(route = ScreenNavigation.LobbyScreen.toString()){
                    LobbyScreen(navController = navController)
                }
                composable(route = ScreenNavigation.GameScreen.toString()){
                    GameScreen(navController = navController)
                }
            }
        }
    }

}