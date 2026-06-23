package com.example.applession.components

import androidx.compose.foundation.layout.*
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp

@Composable
fun MuscleChart(
    value: Int
) {

    Card(
        modifier = Modifier.fillMaxWidth()
    ) {
        Column(
            Modifier.padding(16.dp)
        ) {

            Text("Muscle Activity")

            Spacer(Modifier.height(12.dp))

            LinearProgressIndicator(
                progress = value / 100f,
                modifier = Modifier.fillMaxWidth()
            )

            Spacer(Modifier.height(8.dp))

            Text("$value %")
        }
    }
}