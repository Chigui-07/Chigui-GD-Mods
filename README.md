# Chigui GD Mods

Repositorio para crear, experimentar y mantener mods de **Geometry Dash** usando **Geode**.

El `README.md` funciona también como **bitácora principal del proyecto**: aquí se registran los mods planeados, su estado, las decisiones importantes y los avances realizados.

## Mods

### 1. Demon Tracker

**Estado:** Diseñando v0.1  
**Prioridad:** Principal

Objetivo: crear un sistema personal de clasificación de demons basado en la experiencia real del jugador al completar cada nivel.

Funciones planeadas:

- Detectar demons completados.
- Mostrar un cuestionario de dificultad después de completar un demon.
- Calcular una puntuación personal de dificultad.
- Insertar automáticamente el demon en una clasificación permanente.
- Mantener listas separadas para:
  - Easy Demon
  - Medium Demon
  - Hard Demon
  - Insane Demon
  - Extreme Demon
- Mantener una lista general con todos los demons mezclados.
- Mostrar el hardest personal y el hardest de cada categoría.
- Guardar estadísticas de cada completion.
- Soporte inicial planeado para Windows y Android.

#### Cuestionario v1

Cada apartado se califica del 1 al 10:

| Factor | Peso |
| --- | ---: |
| Dificultad general | 30% |
| Consistencia | 20% |
| Precisión | 15% |
| Control de modos de juego | 15% |
| Aprendizaje | 10% |
| Presión / nervios | 10% |

La puntuación final se convierte a una escala de 0 a 100.

La clasificación será **permanente**: una vez confirmado el cuestionario, el orden relativo del nivel respecto a los demons ya registrados no podrá modificarse. Los futuros demons podrán insertarse por encima, debajo o entre ellos.

Antes del cuestionario se mostrará una advertencia indicando que la clasificación será permanente, sin revelar previamente en qué posición quedará el nivel.

Estadísticas adicionales planeadas que no afectarán directamente la puntuación:

- Intentos.
- Intentos en práctica.
- Tiempo de juego.
- Fecha de completado.
- Monedas.
- Primer completion o rebeat.
- Dificultad oficial.
- ID del nivel.
- Creador.
- Modo o tipo de sección que más costó.

---

### 2. Random 100

**Estado:** Planeado

Reto de 100 Easy Demons completamente aleatorios.

Reglas base:

- No se puede saltar ningún nivel.
- No se puede avanzar hasta completar el nivel actual.
- Si ya estaba completado, será obligatorio hacer rebeat.
- Si el nivel tiene monedas, deberán conseguirse según la regla personal del reto.
- Integración futura con Demon Tracker para clasificar automáticamente cada nuevo completion.

---

### 3. Level Roulette

**Estado:** Planeado

Ruleta aleatoria para niveles normales de Geometry Dash, con filtros y posibles modos de reto.

---

## Bitácora

### 13/09/2026

- Repositorio creado: `Chigui-GD-Mods`.
- Se decidió usar este README como bitácora central de todos los mods.
- Se eligió **Demon Tracker** como primer proyecto.
- Se definió el sistema inicial de clasificación personal.
- Se definieron seis factores para el cuestionario de dificultad y sus pesos.
- Se decidió que la clasificación será permanente una vez confirmada.
- Se planificaron listas separadas por dificultad y una lista general.
- Se dejó **Random 100** como segundo proyecto principal.
- Se dejó **Level Roulette** como proyecto futuro.

## Plataformas objetivo

- Windows
- Android

Otras plataformas podrán estudiarse más adelante.
