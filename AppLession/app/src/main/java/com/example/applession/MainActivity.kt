package com.example.applession

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.ui.res.painterResource
import com.example.applession.screens.DashboardScreen
import com.example.applession.ui.theme.AppLessionTheme



class MainActivity : ComponentActivity() {


    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)

        enableEdgeToEdge()


        setContent {


            AppLessionTheme {

                AppLessionApp()

            }

        }

    }


}



@Composable
fun AppLessionApp(){



    var destination by rememberSaveable{


        mutableStateOf(

            AppDestinations.DASHBOARD

        )

    }




    NavigationSuiteScaffold(


        navigationSuiteItems = {


            AppDestinations.entries.forEach{


                item(

                    icon={


                        Icon(

                            painterResource(it.icon),

                            contentDescription = it.label

                        )

                    },

                    label={

                        Text(it.label)

                    },

                    selected = destination==it,


                    onClick = {


                        destination=it

                    }


                )


            }


        }


    ) {





        when(destination){



            AppDestinations.DASHBOARD->{

                DashboardScreen()

            }



            AppDestinations.BODY->{

                Text(

                    "BODY SCREEN"

                )

            }



            AppDestinations.HISTORY->{

                Text(

                    "HISTORY SCREEN"

                )

            }



        }



    }




}