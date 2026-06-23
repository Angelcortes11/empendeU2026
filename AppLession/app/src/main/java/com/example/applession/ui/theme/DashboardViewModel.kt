package com.example.applession.viewmodel

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.applession.models.Metrics
import com.example.applession.repository.FakeRepository
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch

class DashboardViewModel: ViewModel() {


    private val _metrics = MutableStateFlow(
        FakeRepository.getData()
    )

    val metrics:StateFlow<Metrics>
        get() = _metrics


    init {

        startSimulation()

    }


    private fun startSimulation(){


        viewModelScope.launch {


            while(true){

                delay(2000)

                _metrics.value =
                    FakeRepository.getData()

            }

        }


    }


}