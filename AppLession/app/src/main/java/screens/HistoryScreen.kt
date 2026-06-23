package com.example.applession.screens


import androidx.compose.foundation.lazy.*
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Modifier



@Composable
fun HistoryScreen(){



    LazyColumn{


        items(20){



            Card{


                ListItem(


                    headlineContent={


                        Text(

                            "Session $it"

                        )


                    },


                    supportingContent={

                        Text(

                            "Risk ${(20..90).random()}%"

                        )

                    }



                )



            }



        }



    }



}