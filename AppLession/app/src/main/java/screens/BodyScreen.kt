package com.example.applession.screens


import androidx.compose.foundation.layout.*
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.*
import androidx.compose.ui.unit.dp
import com.example.applession.components.BodyCanvas



@Composable
fun BodyScreen(){



    Column(


        modifier=Modifier
            .fillMaxSize()
            .padding(16.dp)

    ){


        Text(

            "Body Analysis",

            style=MaterialTheme.typography.headlineMedium

        )



        Spacer(

            Modifier.height(20.dp)

        )



        BodyCanvas()



        Spacer(

            Modifier.height(20.dp)

        )



        Card{


            Column(

                Modifier.padding(16.dp)

            ){

                Text(

                    "Right Knee"

                )

                Text(

                    "65°"

                )

            }


        }



    }


}