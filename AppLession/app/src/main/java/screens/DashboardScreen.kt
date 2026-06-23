package com.example.applession.screens

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp
import androidx.lifecycle.viewmodel.compose.viewModel
import com.example.applession.components.GaugeCard
import com.example.applession.components.MuscleChart
import com.example.applession.components.RiskBar
import com.example.applession.viewmodel.DashboardViewModel

@Composable
fun DashboardScreen() {

    val vm: DashboardViewModel = viewModel()
    val data by vm.metrics.collectAsState()

    LazyColumn(
        modifier = Modifier
            .fillMaxSize()
            .background(Color(0xFF0D1321))
            .padding(16.dp),
        verticalArrangement = Arrangement.spacedBy(16.dp)
    ) {

        item {
            Text(
                text = "AppLession Dashboard",
                style = MaterialTheme.typography.headlineMedium,
                color = Color.White
            )
        }

        item {
            GaugeCard(
                title = "Stability",
                value = data.stability,
                max = 1000
            )
        }

        item {
            GaugeCard(
                title = "Fatigue",
                value = data.fatigue,
                max = 100
            )
        }

        item {
            Card(
                colors = CardDefaults.cardColors(
                    containerColor = Color(0xFF1B2433)
                ),
                modifier = Modifier.fillMaxWidth()
            ) {
                Column(
                    modifier = Modifier.padding(16.dp)
                ) {

                    Text(
                        text = "Heart Rate",
                        color = Color.White
                    )

                    Spacer(Modifier.height(8.dp))

                    Text(
                        text = "${data.heartRate} bpm",
                        style = MaterialTheme.typography.headlineMedium,
                        color = Color.Red
                    )
                }
            }
        }

        item {
            MuscleChart(
                value = data.muscleEffort
            )
        }

        item {
            RiskBar(
                risk = data.injuryRisk
            )
        }

        item {
            Card(
                colors = CardDefaults.cardColors(
                    containerColor = Color(0xFF1B2433)
                ),
                modifier = Modifier.fillMaxWidth()
            ) {
                Column(
                    modifier = Modifier.padding(16.dp)
                ) {

                    Text(
                        "Status",
                        color = Color.White
                    )

                    Spacer(Modifier.height(8.dp))

                    Text(
                        text = when {
                            data.injuryRisk > 75 -> "High Injury Risk"
                            data.fatigue > 70 -> "High Fatigue"
                            else -> "Stable"
                        },
                        color = when {
                            data.injuryRisk > 75 -> Color.Red
                            data.fatigue > 70 -> Color.Yellow
                            else -> Color.Green
                        }
                    )
                }
            }
        }
    }
}