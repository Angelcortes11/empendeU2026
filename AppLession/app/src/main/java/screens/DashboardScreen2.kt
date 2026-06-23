package com.example.applession.screens


import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.*
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp
import androidx.lifecycle.viewmodel.compose.viewModel
import com.example.applession.components.*
import viewmodel.DashboardViewModel



@Composable
fun DashboardScreen(){


    val vm:DashboardViewModel=
        viewModel()

    val data by vm.metrics.collectAsState()



    LazyColumn(

        modifier=Modifier

            .fillMaxSize()

            .background(

                Color(0xFF0D1321)

            )

            .padding(16.dp)

    ){



        item{


            GaugeCard(

                title="Stability",

                value=data.stability,

                max=1000

            )

        }



        item{

            Spacer(

                Modifier.height(12.dp)

            )

        }



        item{


            GaugeCard(

                title="Fatigue",

                value=data.fatigue,

                max=100

            )

        }


        item{

            Spacer(

                Modifier.height(12.dp)

            )

        }



        item{


            MuscleChart()

        }



        item{

            Spacer(

                Modifier.height(12.dp)

            )

        }



        item{


            RiskBar(

                data.injuryRisk

            )

        }



    }



}
