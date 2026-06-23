package com.example.applession.viewmodel

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.applession.models.Session
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch

class HistoryViewModel : ViewModel() {

    private val _sessions =
        MutableStateFlow<List<Session>>(emptyList())

    val sessions: StateFlow<List<Session>> = _sessions

    init {
        loadFakeHistory()
    }

    private fun loadFakeHistory() {
        viewModelScope.launch {
            _sessions.value = List(15) {
                Session(
                    id = it,
                    date = "2026-06-${10 + it}",
                    risk = (20..90).random(),
                    fatigue = (10..100).random(),
                    stability = (300..900).random(),
                    heart = (80..170).random()
                )
            }
        }
    }
}