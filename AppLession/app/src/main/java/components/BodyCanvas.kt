package com.example.applession.components


import androidx.compose.foundation.Canvas
import androidx.compose.foundation.layout.*
import androidx.compose.runtime.Composable
import androidx.compose.ui.*
import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp




@Composable
fun BodyCanvas(){



    Canvas(

        Modifier
            .fillMaxWidth()
            .height(400.dp)

    ){



        drawCircle(

            Color.White,
            20f,

            Offset(

                size.width/2,
                60f

            )

        )




        drawLine(

            Color.White,

            Offset(

                size.width/2,
                80f

            ),

            Offset(

                size.width/2,
                200f

            ),

            5f

        )






        drawLine(

            Color.Cyan,

            Offset(

                size.width/2,
                100f

            ),

            Offset(

                size.width/2-80,
                150f

            ),

            5f

        )






        drawLine(

            Color.Cyan,

            Offset(

                size.width/2,
                100f

            ),

            Offset(

                size.width/2+80,
                150f

            ),

            5f

        )







        drawLine(

            Color.Yellow,

            Offset(

                size.width/2,
                200f

            ),

            Offset(

                size.width/2-50,
                320f

            ),

            5f

        )






        drawLine(

            Color.Red,

            Offset(

                size.width/2,
                200f

            ),

            Offset(

                size.width/2+50,
                320f

            ),

            5f

        )





    }



}