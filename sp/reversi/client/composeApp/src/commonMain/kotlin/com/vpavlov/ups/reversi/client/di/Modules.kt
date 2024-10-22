package com.vpavlov.ups.reversi.client.di
import org.koin.core.context.startKoin
import org.koin.dsl.module

fun initKoin(){
    startKoin {
        modules(sharedModules)
    }
}

val sharedModules = module {

}