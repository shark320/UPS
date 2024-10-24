package com.vpavlov.ups.reversi.client.presentation.login

import androidx.compose.runtime.State
import androidx.compose.runtime.mutableStateOf
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import androidx.navigation.NavHostController
import com.vpavlov.ups.reversi.client.di.koin
import com.vpavlov.ups.reversi.client.service.api.MessageService
import com.vpavlov.ups.reversi.client.utils.isValidUsername
import kotlinx.coroutines.launch

class LoginViewModel(private val navController: NavHostController): ViewModel() {

    private val _state = mutableStateOf(LoginState())
    val state: State<LoginState> = _state

    private val messageService: MessageService = koin.get()

    fun onEvent(event: LoginEvent){
        when(event){
            is LoginEvent.UsernameEntered -> usernameEntered(event.username)
            LoginEvent.ProcessLogin -> processLogin()
        }
    }

    private fun usernameEntered(username: String){
        var error = false
        if (username.isNotEmpty() && !isValidUsername(username)){
            error = true
        }
        _state.value = state.value.copy(username = username, usernameError = error)
    }

    private fun processLogin() {
        _state.value = state.value.copy(waitingResponse = true)
        viewModelScope.launch {

        }
    }
}