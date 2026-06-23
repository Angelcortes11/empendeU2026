package com.example.applession.models

import androidx.room.Entity
import androidx.room.PrimaryKey


@Entity(tableName = "sessions")
data class Session(

    @PrimaryKey(autoGenerate = true)
    val id:Int = 0,

    val date:String,

    val risk:Int,

    val fatigue:Int,

    val stability:Int,

    val heart:Int

)