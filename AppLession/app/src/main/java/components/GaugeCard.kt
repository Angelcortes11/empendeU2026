package com.example.applession.components

import androidx.compose.animation.core.animateFloatAsState
import androidx.compose.foundation.Canvas
import androidx.compose.foundation.layout.*
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.*
import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.graphics.*
import androidx.compose.ui.graphics.drawscope.Stroke
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp

@Composable
fun GaugeCard(
    title: String,
    value: Int,
    max: Int
) {

    val animated by animateFloatAsState(
        targetValue = value.toFloat()
    )

    Card(
        colors = CardDefaults.cardColors(
            containerColor = Color(0xFF1B2433)
        ),
        modifier = Modifier.fillMaxWidth()
    ) {
        Column(
            horizontalAlignment = Alignment.CenterHorizontally,
            modifier = Modifier.padding(16.dp)
        ) {

            Text(
                title,
                color = Color.White
            )

            Spacer(Modifier.height(16.dp))

            Box(contentAlignment = Alignment.Center) {

                Canvas(
                    modifier = Modifier.size(180.dp)
                ) {

                    drawArc(
                        color = Color.DarkGray,
                        startAngle = 135f,
                        sweepAngle = 270f,
                        useCenter = false,
                        style = Stroke(18f)
                    )

                    drawArc(
                        brush = Brush.linearGradient(
                            listOf(
                                Color.Cyan,
                                Color.Blue
                            )
                        ),
                        startAngle = 135f,
                        sweepAngle = (animated / max) * 270f,
                        useCenter = false,
                        style = Stroke(18f)
                    )
                }

                Text(
                    "$value",
                    color = Color.White,
                    fontSize = 32.sp
                )
            }
        }
    }
}