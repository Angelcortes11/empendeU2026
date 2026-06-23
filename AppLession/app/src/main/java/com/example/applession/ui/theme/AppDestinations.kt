package com.example.applession


enum class AppDestinations(

    val label:String,
    val icon:Int

) {

    DASHBOARD(
        "Dashboard",
        R.drawable.ic_home
    ),

    BODY(

        "Body",
        R.drawable.ic_body

    ),

    HISTORY(

        "History",
        R.drawable.ic_history

    )

}