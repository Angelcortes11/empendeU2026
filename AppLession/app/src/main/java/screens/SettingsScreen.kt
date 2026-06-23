package com.example.applession.screens

import androidx.compose.foundation.layout.*
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp

@Composable
fun SettingsScreen() {

    var bluetoothEnabled by remember {
        mutableStateOf(false)
    }

    Column(
        modifier = Modifier.padding(16.dp)
    ) {

        Text(
            "Settings",
            style = MaterialTheme.typography.headlineMedium
        )

        Spacer(Modifier.height(20.dp))

        Row {
            Text("Bluetooth Sensors")

            Switch(
                checked = bluetoothEnabled,
                onCheckedChange = {
                    bluetoothEnabled = it
                }
            )
        }
    }
}
