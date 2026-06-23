package com.example.applession.components

import androidx.compose.foundation.Canvas
import androidx.compose.foundation.layout.*
import androidx.compose.material3.*
import androidx.compose.runtime.Composable
import androidx.compose.ui.*
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp


@Composable
fun GaugeCard(

    title:String,
    value:Int,
    max:Int

){

    Card(

        modifier = Modifier
            .fillMaxWidth()

    ){

        Column(

            horizontalAlignment =
                Alignment.CenterHorizontally,

            modifier=Modifier.padding(16.dp)

        ){

            Text(title)


            Canvas(

                Modifier.size(120.dp)

            ){


                drawArc(

                    color = Color.DarkGray,

                    startAngle = 135f,

                    sweepAngle = 270f,

                    useCenter = false

                )



                drawArc(

                    color = Color.Cyan,

                    startAngle = 135f,

                    sweepAngle =
                        (value.toFloat()/max)*270f,

                    useCenter = false

                )



            }



            Text(

                "$value"

            )


        }

    }


}