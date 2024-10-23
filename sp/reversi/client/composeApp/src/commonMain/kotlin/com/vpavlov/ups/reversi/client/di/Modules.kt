package com.vpavlov.ups.reversi.client.di
import com.vpavlov.ups.reversi.client.config.ConfigProvider
import com.vpavlov.ups.reversi.client.service.api.ConnectionService
import com.vpavlov.ups.reversi.client.service.api.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.impl.ConnectionServiceImpl
import com.vpavlov.ups.reversi.client.service.impl.ConnectionStateServiceImpl
import org.koin.core.Koin
import org.koin.core.context.GlobalContext.get
import org.koin.core.context.startKoin
import org.koin.dsl.module

val koin: Koin
    get() = get()

fun initKoin(){
    startKoin {
        modules(sharedModules)
    }
}

val sharedModules = module {

    single<ConnectionStateService>{
        ConnectionStateServiceImpl()
    }
    single<ConnectionService>{
        ConnectionServiceImpl(config = ConfigProvider.connectionConfig)
    }
}