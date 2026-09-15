# Chigui GD Mods

Repositorio de mods de **Geometry Dash** creados con **Geode**.

## Demon Tracker

**Version actual:** v1.0.0  
**Estado:** candidato de lanzamiento publico  
**Plataformas:** Windows y Android

Demon Tracker crea una clasificacion personal de demons basada en como se sintio cada nivel para el jugador al momento de completarlo.

### Funciones principales

- Detecta nuevos demons completados.
- Exige 100% y, cuando existen, todas las monedas verificadas.
- Muestra un cuestionario de seis factores despues de una completion valida.
- Calcula un Difficulty Score personal de 0 a 100.
- Inserta automaticamente el nivel en un ranking global permanente.
- Mantiene listas separadas para Easy, Medium, Hard, Insane y Extreme Demon.
- Guarda la informacion localmente con el sistema de saved data de Geode.
- Registra rebats sin volver a clasificar el nivel.
- Muestra Level ID, creador, intentos, rebats y score cuando estan disponibles.
- Permite abrir los niveles desde la propia lista.
- Usa las caras de dificultad y elementos visuales nativos de Geometry Dash.

### Cuestionario

| Factor | Peso |
| --- | ---: |
| Dificultad general | 30% |
| Consistencia | 20% |
| Precision | 15% |
| Control | 15% |
| Aprendizaje | 10% |
| Presion | 10% |

Una vez confirmado el cuestionario, la relacion del demon con los niveles ya registrados no se modifica. Nuevos demons pueden insertarse por encima, debajo o entre ellos.

## Otros mods planeados

- **Random 100:** reto de 100 Easy Demons aleatorios sin skips.
- **Level Roulette:** ruleta de niveles con filtros y modos de reto.

## Desarrollo

La rama `mod/demon-tracker` contiene el codigo de Demon Tracker. Las compilaciones multiplataforma se generan automaticamente con GitHub Actions.
