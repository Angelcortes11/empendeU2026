package viewmodel

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import repository.FakeRepository
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch
import models.Metrics

class DashboardViewModel: ViewModel() {


    private val _metrics = MutableStateFlow(
        FakeRepository.getData()
    )

    val metrics: StateFlow<Metrics>
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