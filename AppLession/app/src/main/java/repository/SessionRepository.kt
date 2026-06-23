package com.example.applession.repository

import com.example.applession.database.SessionDao
import com.example.applession.models.Session

class SessionRepository(
    private val dao: SessionDao
) {

    suspend fun save(session: Session) {
        dao.insert(session)
    }

    fun getSessions() =
        dao.getSessions()
}