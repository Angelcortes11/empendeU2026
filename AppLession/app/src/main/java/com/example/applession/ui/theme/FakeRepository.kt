package com.example.applession.repository

import com.example.applession.models.Metrics



object FakeRepository {


    fun getData():Metrics{


        return Metrics(

            stability = (300..900).random(),

            fatigue=(0..100).random(),

            heartRate=(70..180).random(),

            muscleEffort=(30..100).random(),

            injuryRisk=(0..100).random()

        )


    }


}
