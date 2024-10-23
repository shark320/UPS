package com.vpavlov.ups.reversi.client.di
import com.vpavlov.ups.reversi.client.service.api.ConnectionStateService
import com.vpavlov.ups.reversi.client.service.impl.ConnectionServiceImpl
import com.vpavlov.ups.reversi.client.service.impl.ConnectionStateServiceImpl
import org.koin.core.context.startKoin
import org.koin.dsl.module

fun initKoin(){
    startKoin {
        modules(sharedModules)
    }
}

val sharedModules = module {
    single<ConnectionStateService>{
        ConnectionStateServiceImpl()
    }
}