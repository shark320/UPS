package com.vpavlov.ups.reversi.client.presentation.common.component

import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.material3.AlertDialog
import androidx.compose.material3.Button
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.ui.Modifier

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
fun InputDialog(
    inputText: String = "",
    onInputStringChange: (String) -> Unit,
    onOkClick: (String) -> Unit,
    onCancelClick: () -> Unit = {},
    title: String,
    inputLabel: String,
    isError: Boolean = false,
    errorMessage: String? = null,
    okButtonEnabled: Boolean = true
) {
    val openAlertDialog = remember { mutableStateOf(true) }
    AlertDialog(
        onDismissRequest = onCancelClick,
        title = { Text(title) },
        text = {
            CustomOutlinedTextField(
                modifier = Modifier
                    .fillMaxWidth(),
                label = { Text(inputLabel) },
                value = inputText,
                onValueChange = { onInputStringChange(it) },
                isError = isError,
                errorMessage = errorMessage
            )
        },
        confirmButton = {
            Button(
                onClick = {
                    onOkClick(inputText)
                    openAlertDialog.value = false
                },
                enabled = okButtonEnabled
            ) {
                Text("OK")
            }
        },
        dismissButton = {
            Button(
                onClick = {
                    onCancelClick()
                    openAlertDialog.value = false
                }
            ) {
                Text("Cancel")
            }
        }
    )
}

