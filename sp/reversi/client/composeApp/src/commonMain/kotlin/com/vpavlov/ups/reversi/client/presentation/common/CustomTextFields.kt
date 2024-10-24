package com.vpavlov.ups.reversi.client.presentation.common

import androidx.compose.animation.AnimatedVisibility
import androidx.compose.animation.expandVertically
import androidx.compose.animation.fadeIn
import androidx.compose.animation.fadeOut
import androidx.compose.animation.shrinkVertically
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.foundation.text.KeyboardActions
import androidx.compose.foundation.text.KeyboardOptions
import androidx.compose.material3.MaterialTheme.colorScheme
import androidx.compose.material3.MaterialTheme.typography
import androidx.compose.material3.OutlinedTextField
import androidx.compose.material3.Text
import androidx.compose.material3.TextFieldColors
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.focus.FocusManager
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.Shape
import androidx.compose.ui.platform.LocalFocusManager
import androidx.compose.ui.text.TextStyle
import androidx.compose.ui.text.input.ImeAction
import androidx.compose.ui.unit.dp
import com.vpavlov.ups.reversi.client.ui.theme.components.CustomOutlineTextFieldColors
import com.vpavlov.ups.reversi.client.ui.theme.defaultCornerRadius

@Composable
fun CustomOutlinedTextField(
    modifier: Modifier = Modifier,
    readOnly: Boolean = false,
    value: String = "",
    label: @Composable() (() -> Unit)? = null,
    onValueChange: (String) -> Unit = {},
    trailingIcon: @Composable() (() -> Unit)? = null,

    shape: Shape = RoundedCornerShape(defaultCornerRadius),
    enabled: Boolean = true,
    focusManager: FocusManager = LocalFocusManager.current,
    keyboardOptions: KeyboardOptions = KeyboardOptions(imeAction = ImeAction.Done),
    keyboardActions: KeyboardActions = KeyboardActions(
        onDone = {
            focusManager.clearFocus() // Unfocus the OutlinedTextField
        }
    ),
    leadingIcon: @Composable (() -> Unit)? = null,
    isError: Boolean = false,
    errorMessage: String? = null,
    errorColor: Color = colorScheme.error,
    singleLine: Boolean = true,
    textStyle: TextStyle = typography.bodyLarge,
    colors: TextFieldColors = CustomOutlineTextFieldColors(),
) {
    Column {
        OutlinedTextField(
            readOnly = readOnly,
            value = value,
            onValueChange = onValueChange,
            label = label,
            trailingIcon = trailingIcon,
            colors = colors,
            modifier = modifier,
            shape = shape,
            enabled = enabled,
            keyboardOptions = keyboardOptions,
            keyboardActions = keyboardActions,
            leadingIcon = leadingIcon,
            singleLine = singleLine,
            textStyle = textStyle,
            isError = isError
        )
        AnimatedVisibility(
            visible = isError,
            modifier = Modifier.padding(0.dp),
            enter = fadeIn() + expandVertically(),
            exit = fadeOut() + shrinkVertically(),
        ) {
            Text(
                text = errorMessage ?: "Error",
                color = errorColor,
                fontSize = typography.bodySmall.fontSize
            )
        }
    }

}