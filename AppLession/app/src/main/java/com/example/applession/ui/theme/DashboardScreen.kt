package com.example.applession.screens


import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.*
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp
import kotlinx.coroutines.delay



@Composable
fun DashboardScreen(){


    var stability by remember {

        mutableStateOf(426)

    }


    var fatigue by remember {

        mutableStateOf(10)

    }


    var heartRate by remember {

        mutableStateOf(100)

    }


    var muscle by remember {

        mutableStateOf(70)

    }



    var risk by remember {

        mutableStateOf(48)

    }




    LaunchedEffect(Unit){


        while(true){

            delay(2000)


            stability=(300..900).random()

            fatigue=(0..100).random()

            heartRate=(80..170).random()

            muscle=(40..100).random()

            risk=(0..100).random()

        }


    }




    LazyColumn(

        modifier = Modifier
            .fillMaxSize()
            .background(Color(0xFF0D1321))
            .padding(16.dp)

    ) {


        item{


            Card(

                modifier=Modifier.fillMaxWidth()

            ){

                Column(

                    Modifier.padding(16.dp)

                ){


                    Text(

                        "Stability",

                        color=Color.White

                    )


                    Text(

                        "$stability",

                        style=MaterialTheme.typography.headlineMedium

                    )

                }


            }

        }



        item{


            Spacer(Modifier.height(12.dp))

        }



        item{

            Card{


                Column(

                    Modifier.padding(16.dp)

                ){

                    Text(

                        "Fatigue"

                    )

                    Text(

                        "$fatigue %"

                    )

                }

            }

        }



        item{


            Spacer(Modifier.height(12.dp))

        }



        item{


            Card{


                Column(

                    Modifier.padding(16.dp)

                ){

                    Text(

                        "Heart Rate"

                    )

                    Text(

                        "$heartRate bpm"

                    )

                }


            }


        }







        item{


            Spacer(Modifier.height(12.dp))

        }



        item{


            Card{


                Column(

                    Modifier.padding(16.dp)

                ){

                    Text(

                        "Muscle Effort"

                    )

                    Text(

                        "$muscle %"

                    )

                }


            }


        }



        item{


            Spacer(Modifier.height(12.dp))

        }




        item{


            Card{


                Column(

                    Modifier.padding(16.dp)

                ){

                    Text(

                        "Injury Risk"

                    )

                    Text(

                        "$risk %"

                    )


                    LinearProgressIndicator(

                        progress = risk/100f,

                        modifier=Modifier.fillMaxWidth()

                    )

                }


            }


        }


    }



}



}