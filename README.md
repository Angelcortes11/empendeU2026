AppLession, Equipo +Deporte

AppLession es una aplicación Android desarrollada con **Kotlin** y **Jetpack Compose** orientada al monitoreo biomecánico y la prevención de lesiones deportivas.

Actualmente utiliza datos simulados, pero fue diseñada para integrarse en el futuro con sensores físicos como **ESP32, IMU, EMG y BPM**, permitiendo analizar el estado del deportista durante entrenamientos o actividades físicas.

Objetivo

El objetivo principal es ayudar a reducir el riesgo de lesiones mediante el monitoreo de variables como:

- Estabilidad corporal
- Fatiga muscular
- Esfuerzo muscular
- Frecuencia cardíaca
- Riesgo de lesión
- Historial de sesiones

 Funcionalidades

 Dashboard

Muestra métricas biomecánicas en tiempo real con actualización automática cada 2 segundos.

Métricas disponibles:

- Stability
- Fatigue
- Heart Rate
- Muscle Effort
- Injury Risk

Body Screen

Permite visualizar un modelo corporal simplificado con información sobre articulaciones y zonas musculares.

History Screen

Registra sesiones de entrenamiento y está preparada para almacenar información mediante Room Database.

Settings

Incluye una sección de configuración pensada para futuras integraciones con Bluetooth y sensores externos.

Arquitectura

La aplicación utiliza una arquitectura **MVVM** junto con:

- Jetpack Compose
- ViewModel
- StateFlow
- Coroutines
- Repository Pattern
- Room Database

Estructura del proyecto

## Estructura del proyecto

```text
app/
│
├── MainActivity.kt
│
├── ui/
│   ├── AppDestinations.kt
│   ├── DashboardScreen.kt
│   └── DashboardScreen2.kt
│
├── viewmodel/
│   └── DashboardViewModel.kt
│
├── repository/
│   └── FakeRepository.kt
│
├── models/
│   └── Metrics.kt
│
├── components/
│   ├── GaugeCard.kt
│   ├── MuscleChart.kt
│   └── RiskBar.kt
│
├── manifests/
│   └── AndroidManifest.xml
│
├── androidTest/
│   └── ExampleInstrumentedTest.kt
│
└── test/
    └── ExampleUnitTest.kt
```
  Estado del proyecto

Implementado:

- Dashboard funcional
- Simulación de métricas
- Pantalla corporal
- Historial de sesiones
- Configuración básica
- Estructura de Room
- Base para Bluetooth

  Pendiente:

- Persistencia completa con Room
- Comunicación BLE con ESP32
- Integración de sensores físicos
- Gráficos avanzados
- Predicción de lesiones mediante modelos de Machine Learning

  Tecnologías utilizadas

- Kotlin
- Jetpack Compose
- Material 3
- Room Database
- Coroutines
- StateFlow Bluetooth BLE (en desarrollo)

Autores

Proyecto desarrollado por estudiantes de la Escuela PROA La Falda.
