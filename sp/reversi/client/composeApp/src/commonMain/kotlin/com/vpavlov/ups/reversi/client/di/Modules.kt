package com.vpavlov.ups.reversi.client.di

import com.vpavlov.ups.reversi.client.config.ConfigProvider
import com.vpavlov.ups.reversi.client.presentation.connection.ConnectionScreenViewModel
import com.vpavlov.ups.reversi.client.presentation.game.GameScreenViewModel
import com.vpavlov.ups.reversi.client.presentation.lobby.LobbyScreenViewModel
import com.vpavlov.ups.reversi.client.presentation.login.LoginScreenViewModel
import com.vpavlov.ups.reversi.client.presentation.menu.MenuScreenViewModel
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.PingService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.UserMessageStateService
import com.vpavlov.ups.reversi.client.service.api.state.GameStateService
import com.vpavlov.ups.reversi.client.service.impl.ConnectionServiceImpl
import com.vpavlov.ups.reversi.client.service.impl.PingServiceImpl
import com.vpavlov.ups.reversi.client.service.processor.GetLobbiesProcessor
import com.vpavlov.ups.reversi.client.service.processor.HandshakeProcessor
import com.vpavlov.ups.reversi.client.service.processor.LoginProcessor
import com.vpavlov.ups.reversi.client.service.processor.PingProcessor
import com.vpavlov.ups.reversi.client.service.impl.offline.ConnectionServiceOfflineImpl
import com.vpavlov.ups.reversi.client.service.impl.offline.state.ClientStateServiceOfflineImpl
import com.vpavlov.ups.reversi.client.service.impl.offline.state.ConnectionStateServiceOfflineImpl
import com.vpavlov.ups.reversi.client.service.impl.state.ClientStateServiceImpl
import com.vpavlov.ups.reversi.client.service.impl.state.ConnectionStateServiceImpl
import com.vpavlov.ups.reversi.client.service.impl.state.UserMessageStateServiceImpl
import com.vpavlov.ups.reversi.client.service.impl.state.GameStateServiceImpl
import com.vpavlov.ups.reversi.client.service.processor.ConnectToLobbyProcessor
import com.vpavlov.ups.reversi.client.service.processor.CreateLobbyProcessor
import com.vpavlov.ups.reversi.client.service.processor.ExitLobbyProcessor
import com.vpavlov.ups.reversi.client.service.processor.GameMoveProcessor
import com.vpavlov.ups.reversi.client.service.processor.GetGameStateProcessor
import com.vpavlov.ups.reversi.client.service.processor.GetLobbyStateProcessor
import com.vpavlov.ups.reversi.client.service.processor.StartGameProcessor
import org.koin.compose.viewmodel.dsl.viewModel
import org.koin.core.Koin
import org.koin.core.context.GlobalContext.get
import org.koin.core.context.startKoin
import org.koin.dsl.module

val koin: Koin
    get() = get()

fun initKoin(offline: Boolean) = startKoin {
    modules(sharedModules)
    if (offline) {
        modules(offlineModules)
    } else {
        modules(onlineModules, messageProcessorsModule)
    }
}

val onlineModules = module {
    single<ConnectionService> {
        ConnectionServiceImpl(
            config = ConfigProvider.connectionConfig,
            userMessageStateService = get(),
            connectionStateService = get(),
        )
    }
    single<ConnectionStateService> {
        ConnectionStateServiceImpl()
    }
    single<ClientStateService> {
        ClientStateServiceImpl()
    }
    single<PingService> {
        PingServiceImpl(
            config = ConfigProvider.connectionConfig,
            clientStateService = get(),
            pingProcessor = get(),
            getLobbiesProcessor = get(),
            connectionStateService = get(),
            getLobbyStateProcessor = get(),
            getGameStateProcessor = get()
        )
    }

    single<GameStateService> {
        GameStateServiceImpl()
    }

}

val messageProcessorsModule = module {
    single {
        LoginProcessor(
            config = ConfigProvider.connectionConfig,
            clientStateService = get(),
            connectionService = get(),
            userMessageStateService = get()
        )
    }

    single {
        HandshakeProcessor(
            config = ConfigProvider.connectionConfig,
            connectionService = get(),
            userMessageStateService = get()
        )
    }

    single {
        PingProcessor(
            config = ConfigProvider.connectionConfig,
            connectionService = get(),
            userMessageStateService = get()
        )
    }

    single {
        GetLobbiesProcessor(
            config = ConfigProvider.connectionConfig,
            connectionService = get(),
            userMessageStateService = get(),
            clientStateService = get()
        )
    }

    single {
        ConnectToLobbyProcessor(
            config = ConfigProvider.connectionConfig,
            clientStateService = get(),
            connectionService = get(),
            userMessageStateService = get(),
        )
    }

    single {
        GetLobbyStateProcessor(
            config = ConfigProvider.connectionConfig,
            clientStateService = get(),
            connectionService = get(),
            userMessageStateService = get(),
        )
    }

    single {
        CreateLobbyProcessor(
            config = ConfigProvider.connectionConfig,
            clientStateService = get(),
            connectionService = get(),
            userMessageStateService = get(),
        )
    }

    single {
        ExitLobbyProcessor(
            config = ConfigProvider.connectionConfig,
            clientStateService = get(),
            connectionService = get(),
            userMessageStateService = get(),
        )
    }

    single {
        StartGameProcessor(
            config = ConfigProvider.connectionConfig,
            clientStateService = get(),
            connectionService = get(),
            userMessageStateService = get(),
        )
    }

    single {
        GetGameStateProcessor(
            config = ConfigProvider.connectionConfig,
            clientStateService = get(),
            connectionService = get(),
            userMessageStateService = get(),
            gameStateService = get()
        )
    }

    single {
        GameMoveProcessor(
            config = ConfigProvider.connectionConfig,
            clientStateService = get(),
            connectionService = get(),
            userMessageStateService = get(),
            gameStateService = get(),
        )
    }


}


val offlineModules = module {
    single<ConnectionService> {
        ConnectionServiceOfflineImpl(
            config = ConfigProvider.connectionConfig,
            connectionStateService = get(),
            userMessageStateService = get(),
        )
    }
    single<ConnectionStateService> {
        ConnectionStateServiceOfflineImpl(
        )
    }
    single<ClientStateService> {
        ClientStateServiceOfflineImpl()
    }
}


val sharedModules = module {
    viewModel {
        ConnectionScreenViewModel(
            connectionService = get(),
            userMessageStateService = get(),
            connectionStateService = get(),
            pingService = get(),
            handshakeProcessor = get(),
            clientStateService = get()
        )
    }
    viewModel {
        LoginScreenViewModel(
            clientStateService = get(),
            userMessageStateService = get(),
            connectionStateService = get(),
            pingService = get(),
            loginProcessor = get()
        )
    }

    viewModel {
        MenuScreenViewModel(
            connectionStateService = get(),
            userMessageStateService = get(),
            pingService = get(),
            clientStateService = get(),
            connectToLobbyProcessor = get(),
            createLobbyProcessor = get()
        )
    }

    viewModel {
        LobbyScreenViewModel(
            clientStateService = get(),
            connectionStateService = get(),
            userMessageStateService = get(),
            pingService = get(),
            exitLobbyProcessor = get(),
            startGameProcessor = get()
        )
    }

    viewModel {
        GameScreenViewModel(
            clientStateService = get(),
            connectionStateService = get(),
            userMessageStateService = get(),
            pingService = get(),
            exitLobbyProcessor = get(),
            startGameProcessor = get(),
            gameStateService = get(),
            processGameMoveProcessor = get()
        )
    }


    single<UserMessageStateService> {
        UserMessageStateServiceImpl()
    }

}