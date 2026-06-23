package com.example.applession.components


import androidx.compose.foundation.Canvas
import androidx.compose.foundation.layout.*
import androidx.compose.runtime.*
import androidx.compose.ui.*
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.unit.dp




@Composable
fun MuscleChart(){



    val values = remember {


        List(20){

            (30..100).random()

        }

    }



    Canvas(

        Modifier

            .fillMaxWidth()

            .height(200.dp)

    ){


        for(i in 0 until values.size-1){



            val x1 =
                i*size.width/20


            val y1 =
                size.height-values[i]*2f


            val x2 =
                (i+1)*size.width/20


            val y2 =
                size.height-values[i+1]*2f




            drawLine(

                color=Color.Cyan,

                start=Offset(x1,y1),

                end=Offset(x2,y2),

                strokeWidth=5f

            )



        }



    }



}