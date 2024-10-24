package com.vpavlov.ups.reversi.client.di

import com.vpavlov.ups.reversi.client.config.ConfigProvider
import com.vpavlov.ups.reversi.client.presentation.connection.ConnectionViewModel
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.impl.ConnectionServiceImpl
import com.vpavlov.ups.reversi.client.service.impl.ConnectionStateServiceImpl
import com.vpavlov.ups.reversi.client.service.impl.offline.ConnectionServiceOfflineImpl
import com.vpavlov.ups.reversi.client.service.impl.offline.ConnectionStateServiceOfflineImpl
import org.koin.compose.viewmodel.dsl.viewModel
import org.koin.core.Koin
import org.koin.core.context.GlobalContext.get
import org.koin.core.context.startKoin
import org.koin.dsl.module

val koin: Koin
    get() = get()

fun initKoin(offline: Boolean) = startKoin {
    modules(sharedModules)
    if (offline){
        modules(offlineModules)
    } else{
        modules(onlineModules)
    }
}

val onlineModules = module {
    single<ConnectionService> {
        ConnectionServiceImpl(config = ConfigProvider.connectionConfig)
    }
    single<ConnectionStateService> {
        ConnectionStateServiceImpl()
    }

}

val offlineModules = module {
    single<ConnectionService> {
        ConnectionServiceOfflineImpl(config = ConfigProvider.connectionConfig)
    }
    single<ConnectionStateService> {
        ConnectionStateServiceOfflineImpl()
    }
}


val sharedModules = module {
    viewModel { ConnectionViewModel() }
}