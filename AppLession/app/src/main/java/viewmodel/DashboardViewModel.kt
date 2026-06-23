package com.example.applession.viewmodel

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.applession.models.Metrics
import com.example.applession.repository.FakeRepository
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch

class DashboardViewModel : ViewModel() {

    // Estado interno mutable
    private val _metrics = MutableStateFlow(
        Metrics(
            stability = 426,
            fatigue = 10,
            heartRate = 100,
            muscleEffort = 70,
            injuryRisk = 48
        )
    )

    // Estado público observable
    val metrics: StateFlow<Metrics> = _metrics

    // Contador para futuras sesiones en Room
    private var counter = 0

    init {
        startSimulation()
    }

    private fun startSimulation() {
        viewModelScope.launch {

            while (true) {

                delay(2000)

                val newData = FakeRepository.getData()

                _metrics.value = newData

                counter++

                if (counter % 5 == 0) {
                    saveSession(newData)
                }
            }
        }
    }

    private fun saveSession(metrics: Metrics) {
        // En Parte 5 conectaremos esto con Room Database
        println(
            "Session Saved -> " +
                    "Stability: ${metrics.stability}, " +
                    "Fatigue: ${metrics.fatigue}, " +
                    "Heart: ${metrics.heartRate}, " +
                    "Risk: ${metrics.injuryRisk}"
        )
    }

    fun refreshManually() {
        _metrics.value = FakeRepository.getData()
    }

    fun stopSimulation() {
        // Lo dejamos preparado para futuro control manual
    }
}