package com.example.applession.components


import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.material3.*
import androidx.compose.runtime.Composable
import androidx.compose.ui.*
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp



@Composable
fun RiskBar(


    risk:Int

){


    Card{


        Column(

            Modifier.padding(16.dp)

        ){

            Text(

                "Risk Traffic"

            )


            Spacer(

                Modifier.height(10.dp)

            )


            Row{


                repeat(15){


                    Box(

                        Modifier
                            .padding(2.dp)
                            .width(12.dp)
                            .height(25.dp)
                            .background(


                                if(it<risk/7)
                                    Color.Yellow
                                else
                                    Color.DarkGray


                            )

                    )


                }


            }



            Spacer(

                Modifier.height(8.dp)

            )



            Text(

                "$risk %"

            )



        }


    }



}