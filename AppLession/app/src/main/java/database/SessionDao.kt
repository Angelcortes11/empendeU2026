package com.example.applession.database


import androidx.room.*
import com.example.applession.models.Session
import kotlinx.coroutines.flow.Flow


@Dao
interface SessionDao {


    @Insert
    suspend fun insert(
        session: Session
    )


    @Query(
        "SELECT * FROM sessions ORDER BY id DESC"
    )
    fun getSessions():Flow<List<Session>>


}