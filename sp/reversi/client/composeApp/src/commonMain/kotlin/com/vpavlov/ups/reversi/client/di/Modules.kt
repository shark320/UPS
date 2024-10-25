package com.vpavlov.ups.reversi.client.di

import androidx.navigation.NavHostController
import com.vpavlov.ups.reversi.client.config.ConfigProvider
import com.vpavlov.ups.reversi.client.presentation.connection.ConnectionViewModel
import com.vpavlov.ups.reversi.client.presentation.login.LoginViewModel
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.state.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.api.MessageService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.api.state.ErrorStateService
import com.vpavlov.ups.reversi.client.service.impl.ConnectionServiceImpl
import com.vpavlov.ups.reversi.client.service.impl.state.ConnectionStateServiceImpl
import com.vpavlov.ups.reversi.client.service.impl.message.MessageServiceImpl
import com.vpavlov.ups.reversi.client.service.impl.message.processors.HandshakeProcessor
import com.vpavlov.ups.reversi.client.service.impl.message.processors.LoginProcessor
import com.vpavlov.ups.reversi.client.service.impl.offline.ConnectionServiceOfflineImpl
import com.vpavlov.ups.reversi.client.service.impl.offline.state.ConnectionStateServiceOfflineImpl
import com.vpavlov.ups.reversi.client.service.impl.offline.MessageServiceOfflineImpl
import com.vpavlov.ups.reversi.client.service.impl.offline.state.ClientStateServiceOfflineImpl
import com.vpavlov.ups.reversi.client.service.impl.state.ClientStateServiceImpl
import com.vpavlov.ups.reversi.client.service.impl.state.ErrorStateServiceImpl
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
        ConnectionServiceImpl(config = ConfigProvider.connectionConfig)
    }
    single<ConnectionStateService> {
        ConnectionStateServiceImpl()
    }
    single<MessageService> {
        MessageServiceImpl(
            loginProcessor = get(),
            handshakeProcessor = get()
        )
    }
    single<ClientStateService> {
        ClientStateServiceImpl()
    }


}

val messageProcessorsModule = module {
    single {
        LoginProcessor(
            config = ConfigProvider.connectionConfig,
            clientStateService = get(),
            connectionService = get(),
            errorStateService = get(),
        )
    }

    single {
        HandshakeProcessor(
            config = ConfigProvider.connectionConfig,
            connectionStateService = get(),
            connectionService = get(),
            errorStateService = get()
        )
    }
}


val offlineModules = module {
    single<ConnectionService> {
        ConnectionServiceOfflineImpl(config = ConfigProvider.connectionConfig)
    }
    single<ConnectionStateService> {
        ConnectionStateServiceOfflineImpl()
    }
    single<MessageService> {
        MessageServiceOfflineImpl(
            connectionStateService = get(),
            clientStateService = get(),
            errorStateService = get()
        )
    }
    single<ClientStateService> {
        ClientStateServiceOfflineImpl()
    }
}


val sharedModules = module {
    viewModel {
        ConnectionViewModel(
            connectionService = get(),
            errorStateService = get(),
            connectionStateService = get(),
            messageService = get(),
        )
    }
    viewModel { (navController: NavHostController) ->
        LoginViewModel(
            navController = navController,
            messageService = get(),
            clientStateService = get(),
            errorStateService = get()
        )
    }

    single<ErrorStateService> {
        ErrorStateServiceImpl()
    }
}