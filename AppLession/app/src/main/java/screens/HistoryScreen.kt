package com.example.applession.screens

import androidx.compose.foundation.lazy.*
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.lifecycle.viewmodel.compose.viewModel
import com.example.applession.viewmodel.HistoryViewModel

@Composable
fun HistoryScreen() {

    val vm: HistoryViewModel = viewModel()
    val sessions by vm.sessions.collectAsState()

    LazyColumn {

        items(sessions) { session ->

            Card {
                ListItem(
                    headlineContent = {
                        Text("Session ${session.id}")
                    },
                    supportingContent = {
                        Text(
                            "Risk ${session.risk}% | " +
                                    "Fatigue ${session.fatigue}%"
                        )
                    }
                )
            }
        }
    }
}