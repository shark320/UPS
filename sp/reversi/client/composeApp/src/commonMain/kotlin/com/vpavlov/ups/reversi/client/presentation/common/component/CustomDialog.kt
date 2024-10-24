package com.vpavlov.ups.reversi.client.presentation.common.component

import androidx.compose.material3.AlertDialog
import androidx.compose.material3.Button
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.navigation.NavController
import com.vpavlov.ups.reversi.client.presentation.common.viewModel.CommonEvent
import com.vpavlov.ups.reversi.client.presentation.common.viewModel.CustomViewModel

@Composable
fun ConfirmationDialog(
    title: String = "Confirmation",
    message: String,
    okButtonText: String = "OK",
    cancelButtonText: String = "Cancel",
    onOkClick: () -> Unit = {},
    onCancelClick: () -> Unit = {},
    onDismissRequest: () -> Unit = {},
) {
    val openAlertDialog = remember { mutableStateOf(true) }
    when {
        openAlertDialog.value -> {
            AlertDialog(
                onDismissRequest = onDismissRequest,
                title = { Text(title) },
                text = { Text(message) },
                confirmButton = {
                    Button(onClick = {
                        onOkClick()
                        openAlertDialog.value = false

                    }) {
                        Text(okButtonText)
                    }
                },
                dismissButton = {
                    Button(onClick = {
                        onCancelClick()
                        openAlertDialog.value = false
                    }) {
                        Text(cancelButtonText)
                    }
                }
            )
        }
    }
}

@Composable
fun ErrorDialog(
    title: String = "Error",
    message: String,
    okButtonText: String = "Ok",
    onOkClick: () -> Unit,
    onDismissRequest: () -> Unit = {},
) {
    val openAlertDialog = remember { mutableStateOf(true) }
    when {
        openAlertDialog.value -> {
            AlertDialog(
                onDismissRequest = onDismissRequest,
                title = { Text(title) },
                text = { Text(message) },
                confirmButton = {
                    Button(onClick = {
                        onOkClick()
                        openAlertDialog.value = false
                    }) {
                        Text(okButtonText)
                    }
                }
            )
        }
    }
}

@Composable
fun HandleErrors(viewModel: CustomViewModel){
    val errorMessage = viewModel.commonState.value.errorMessage
    if (errorMessage != null){
        ErrorDialog(
            message = errorMessage,
            onOkClick = {viewModel.onEvent(CommonEvent.ClearError)}
        )
    }
}