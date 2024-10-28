package com.vpavlov.ups.reversi.client.service.impl

import com.vpavlov.ups.reversi.client.config.ConnectionConfig
import com.vpavlov.ups.reversi.client.service.api.PingService
import com.vpavlov.ups.reversi.client.service.api.state.ClientStateService
import com.vpavlov.ups.reversi.client.service.impl.message.processors.GetLobbiesProcessor
import com.vpavlov.ups.reversi.client.service.impl.message.processors.PingProcessor
import com.vpavlov.ups.reversi.client.state.ClientFlowState
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach
import kotlinx.coroutines.launch
import org.apache.logging.log4j.kotlin.loggerOf
import java.util.concurrent.atomic.AtomicBoolean

class PingServiceImpl(
    private val config: ConnectionConfig,
    private val clientStateService: ClientStateService,
    private val pingProcessor: PingProcessor,
    private val getLobbiesProcessor: GetLobbiesProcessor
): PingService {

    companion object{
        private val LOGGER = loggerOf(PingServiceImpl::class.java)
    }

    private var isRunning = AtomicBoolean(false)
    private var clientFlowState: ClientFlowState? = null
    private var job: Job? = null

    init{
        clientStateService.getStateFlow().onEach { clientState ->
            clientFlowState = clientState?.flowState
            LOGGER.debug("Client state updated: $clientFlowState")
        }.launchIn(CoroutineScope(Dispatchers.Default))
    }

    @Synchronized
    override fun start() {
        LOGGER.debug("Starting ping service!")
        if (isRunning.get()){
            LOGGER.warn("The Ping Service is already running!")
            return
        }
        isRunning.set(true)
        job = CoroutineScope(Dispatchers.Default).launch {
            while (isRunning.get()) {
                action()
                delay(config.pingInterval)
            }
        }
    }

    @Synchronized
    private fun action(){
        if (!config.isPing){
            LOGGER.warn("Ping is disabled!")
            return
        }
        val clientFlowStateTmp = clientFlowState
        if (clientFlowStateTmp == null){
            pingProcessor()
        }else{
            when(clientFlowStateTmp){
                ClientFlowState.MENU -> getLobbiesProcessor()
                ClientFlowState.LOBBY -> {}
                ClientFlowState.GAME -> {}
            }
        }
    }

    @Synchronized
    override fun stop() {
        LOGGER.debug("Stopping ping service!")
        isRunning.set(false)
        job?.cancel()
        job = null
    }

    @Synchronized
    override fun resume() {
        LOGGER.debug("Resuming ping service!")
        if (job == null){
            LOGGER.error("Ping service was not started!")
            throw IllegalStateException("Ping service was not started!")
        }
        if (!isRunning.get()) {
            isRunning.set(true)
        } else {
            LOGGER.warn("The Ping Service is already running!")
        }
    }

    override fun pause() {
        LOGGER.debug("Pausing ping service!")
        isRunning.set(false)
    }

    override fun isRunning() = isRunning.get()

}