package com.vpavlov.ups.reversi.client.di

import com.vpavlov.ups.reversi.client.config.ConfigProvider
import com.vpavlov.ups.reversi.client.presentation.connection.ConnectionScreenViewModel
import com.vpavlov.ups.reversi.client.presentation.lobby.LobbyScreenViewModel
import com.vpavlov.ups.reversi.client.presentation.login.LoginScreenViewModel
import com.vpavlov.ups.reversi.client.presentation.menu.MenuScreenViewModel
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.PingService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
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
import com.vpavlov.ups.reversi.client.service.impl.state.ErrorStateServiceImpl
import com.vpavlov.ups.reversi.client.service.processor.ConnectToLobbyProcessor
import com.vpavlov.ups.reversi.client.service.processor.CreateLobbyProcessor
import com.vpavlov.ups.reversi.client.service.processor.GetLobbyStateProcessor
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
            errorStateService = get(),
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
            getLobbyStateProcessor = get()
        )
    }

}

val messageProcessorsModule = module {
    single {
        LoginProcessor(
            config = ConfigProvider.connectionConfig,
            clientStateService = get(),
            connectionService = get(),
            errorStateService = get()
        )
    }

    single {
        HandshakeProcessor(
            config = ConfigProvider.connectionConfig,
            connectionService = get(),
            errorStateService = get()
        )
    }

    single {
        PingProcessor(
            config = ConfigProvider.connectionConfig,
            connectionService = get(),
            errorStateService = get()
        )
    }

    single {
        GetLobbiesProcessor(
            config = ConfigProvider.connectionConfig,
            connectionService = get(),
            errorStateService = get(),
            clientStateService = get()
        )
    }

    single {
        ConnectToLobbyProcessor(
            config = ConfigProvider.connectionConfig,
            clientStateService = get(),
            connectionService = get(),
            errorStateService = get(),
        )
    }

    single {
        GetLobbyStateProcessor(
            config = ConfigProvider.connectionConfig,
            clientStateService = get(),
            connectionService = get(),
            errorStateService = get(),
        )
    }

    single {
        CreateLobbyProcessor(
            config = ConfigProvider.connectionConfig,
            clientStateService = get(),
            connectionService = get(),
            errorStateService = get(),
        )
    }

}


val offlineModules = module {
    single<ConnectionService> {
        ConnectionServiceOfflineImpl(
            config = ConfigProvider.connectionConfig,
            connectionStateService = get(),
            errorStateService = get(),
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
            errorStateService = get(),
            connectionStateService = get(),
            pingService = get(),
            handshakeProcessor = get(),
            clientStateService = get()
        )
    }
    viewModel {
        LoginScreenViewModel(
            clientStateService = get(),
            errorStateService = get(),
            connectionStateService = get(),
            pingService = get(),
            loginProcessor = get()
        )
    }

    viewModel {
        MenuScreenViewModel(
            connectionStateService = get(),
            errorStateService = get(),
            pingService = get(),
            clientStateService = get(),
            connectToLobbyProcessor = get()
        )
    }

    viewModel {
        LobbyScreenViewModel(
            clientStateService = get(),
            connectionStateService = get(),
            errorStateService = get(),
            pingService = get()
        )
    }

    single<ErrorStateService> {
        ErrorStateServiceImpl()
    }
}