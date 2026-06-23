package com.example.applession.database


import androidx.room.Database
import androidx.room.RoomDatabase
import com.example.applession.models.Session


@Database(

    entities = [

        Session::class

    ],

    version = 1

)

abstract class AppDatabase : RoomDatabase(){


    abstract fun dao():SessionDao


}