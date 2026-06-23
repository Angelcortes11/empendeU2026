# empendeU2026

# 🦵 AppLession

AppLession es una aplicación Android desarrollada con **Kotlin** y **Jetpack Compose** que permite simular el monitoreo biomecánico de un deportista en tiempo real, mostrando métricas relacionadas con estabilidad, esfuerzo muscular, frecuencia cardíaca y riesgo de lesión.

El proyecto está inspirado en soluciones de análisis corporal y prevención de lesiones mediante sensores inteligentes.

---

## 📱 Características

### Dashboard Interactivo
- Indicadores de estabilidad
- Fatiga muscular
- Frecuencia cardíaca simulada
- Riesgo de lesión
- Actualización automática de métricas

### Análisis Corporal
- Modelo corporal simplificado
- Representación de articulaciones
- Indicadores de postura y movimiento

### Historial de Sesiones
- Registro local de entrenamientos
- Persistencia mediante Room Database
- Consulta de sesiones anteriores

### Arquitectura Moderna
- Jetpack Compose
- MVVM
- Repository Pattern
- Room Database
- StateFlow
- ViewModel

---

## 🏗 Arquitectura

La aplicación sigue el patrón MVVM recomendado por Google.

```text
UI (Compose)
        │
        ▼
ViewModel
        │
        ▼
Repository
        │
        ▼
Room Database
        │
        ▼
DAO
```

---

## 📂 Estructura del Proyecto

```text
app/
│
├── components/
│   ├── BodyCanvas.kt
│   ├── GaugeCard.kt
│   ├── MuscleChart.kt
│   └── RiskBar.kt
│
├── database/
│   ├── AppDatabase.kt
│   └── SessionDao.kt
│
├── models/
│   ├── Metrics.kt
│   └── Session.kt
│
├── repository/
│   ├── FakeRepository.kt
│   └── SessionRepository.kt
│
├── screens/
│   ├── DashboardScreen.kt
│   ├── BodyScreen.kt
│   └── HistoryScreen.kt
│
├── viewmodel/
│   └── DashboardViewModel.kt
│
├── AppDestinations.kt
└── MainActivity.kt
```

---

## ⚙ Tecnologías Utilizadas

- Kotlin
- Android Studio
- Jetpack Compose
- Material 3
- Room Database
- StateFlow
- Coroutines
- NavigationSuiteScaffold
- Canvas API

---

## 🚀 Instalación

Clonar el repositorio

```bash
git clone https://github.com/usuario/AppLession.git
```

Abrir el proyecto en Android Studio.

Sincronizar Gradle.

Ejecutar la aplicación en un emulador o dispositivo físico.

---

## 📈 Simulación de Datos

Actualmente las métricas se generan de manera simulada.

Ejemplos:

- Estabilidad: 300 - 900
- Fatiga: 0 - 100%
- Esfuerzo muscular: 30 - 100%
- Frecuencia cardíaca: 70 - 180 BPM
- Riesgo de lesión: 0 - 100%

La actualización ocurre cada 2 segundos.

---

## 🎯 Objetivo

AppLession busca demostrar cómo las tecnologías móviles pueden utilizarse para:

- Reducir lesiones deportivas.
- Mejorar el rendimiento físico.
- Monitorear el estado biomecánico del atleta.
- Facilitar el análisis de sesiones de entrenamiento.

---

## 🔮 Mejoras Futuras

- Integración con sensores BLE.
- Modelo corporal 3D.
- Machine Learning para predicción de lesiones.
- Exportación de reportes PDF.
- Sincronización en la nube.
- Compatibilidad con wearables.

---

## 👨‍💻 Autores

Proyecto desarrollado por estudiantes de **Escuela PROA La Falda**.

Desarrollado con ❤️ utilizando Kotlin y Jetpack Compose.
