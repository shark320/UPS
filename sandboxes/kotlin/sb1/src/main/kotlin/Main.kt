package com.vpavlov.ups.snadbox

import kotlinx.coroutines.*

import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.filter
import kotlinx.coroutines.flow.first

class JobController {
    private val controlState = MutableStateFlow(true) // true = running, false = paused

    fun startPrintingJob() = CoroutineScope(Dispatchers.Default).launch {
        controlState.collect { isRunning ->
            if (isRunning) {
                while (isActive && controlState.value) {
                    println("Message every second")
                    delay(1000)
                }
            } else {
                controlState.filter { it }.first() // Wait until resumed
            }
        }
    }

    fun pause() {
        println("Pausing...")
        controlState.value = false
    }

    fun resume() {
        println("Resuming...")
        controlState.value = true
    }

    fun stop(job: Job) {
        println("Stopping...")
        controlState.value = false
        job.cancel()
    }
}

fun main() = runBlocking {
    val jobController = JobController()
    val printingJob = jobController.startPrintingJob()

    // Simulate pause and resume events
    delay(5000) // Let it print for 5 seconds
    jobController.pause() // Pause the coroutine
    delay(3000) // Wait for 3 seconds
    jobController.resume() // Resume the coroutine

    delay(5000) // Let it print for another 5 seconds
    jobController.stop(printingJob) // Stop the coroutine

    printingJob.join() // Wait for coroutine to complete
}