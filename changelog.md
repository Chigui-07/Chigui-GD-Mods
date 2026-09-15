# Changelog

## v1.0.1

- El ranking ahora se guarda por cuenta de Geometry Dash usando el Account ID.
- Cambiar de cuenta ya no arrastra la lista de demons de otra cuenta.
- Los datos compartidos de las builds de desarrollo anteriores dejan de cargarse en la version publica.
- Si se cambia de cuenta sin cerrar el juego, Demon Tracker recarga automaticamente la lista correspondiente.
- Actualizado el enlace de codigo fuente para apuntar a la rama estable `main`.

## v1.0.0

- Primera version preparada para publicacion.
- Los usuarios nuevos comienzan con una lista vacia; se elimino la lista personal usada durante las pruebas de desarrollo.
- Se mantiene intacta la informacion ya guardada por usuarios que actualizaron desde builds de prueba.
- Interfaz principal refinada con tarjetas de categoria, caras de Demon y contadores por dificultad.
- Listas refinadas con filas estilo Geometry Dash, rango, cara de dificultad, creador y Difficulty Score.
- Ficha individual refinada con ranking global, ranking por categoria, score, creador, ID, intentos y rebats.
- Cuestionario visualmente integrado con el estilo del juego y la cara de dificultad del nivel.
- Anadidos metadatos publicos, enlace al codigo fuente, reporte de problemas y tags para Geode.
- Anadido logo propio de Demon Tracker.

## v0.3.1

- Anadidas las caras oficiales de Demon a los selectores de dificultad.
- Las listas muestran la cara correspondiente a cada nivel y toda la fila funciona como boton.
- Rehecha la ficha del Demon para reducir paneles planos y usar botones visuales de Geometry Dash.

## v0.3.0

- Redisenada la pantalla principal con un panel de resumen mas limpio y botones por categoria con colores distintos.
- Redisenadas las listas de demons como tarjetas con rango, categoria, score y boton de acceso a la ficha.
- Redisenada la ficha individual con paneles de ranking global y por categoria, creador, Level ID, intentos, rebats y score personal.
- Redisenado el cuestionario con panel de pregunta, valor destacado, botones diferenciados y barra de progreso.
- Ajustados tamanos y espaciados para que la interfaz sea mas clara tanto con mouse como con pantalla tactil.

## v0.2.0

- Anadido guardado persistente de la clasificacion mediante el sistema de saved values de Geode.
- Anadida deteccion automatica al completar un Demon fuera de Practice Mode.
- Anadida comprobacion de monedas verificadas antes de permitir una nueva clasificacion.
- Los rebats de demons ya registrados conservan su posicion y actualizan sus estadisticas sin volver a preguntar.
- Anadido cuestionario personal de seis factores con valores del 1 al 10.
- Anadida confirmacion final sin mostrar previamente la posicion del nivel.
- Implementada la formula ponderada y normalizada de Difficulty Score de 0 a 100.
- Los nuevos demons se insertan en el ranking sin alterar el orden relativo de los demons ya clasificados.
- Anadido desempate por las seis respuestas; un empate total queda debajo del demon ya registrado.
- Las fichas muestran creador, Difficulty Score y usan el Level ID cuando ya esta disponible.

## v0.1.2

- Anadidas fichas individuales al tocar un demon.
- Anadidas opciones para abrir niveles oficiales y jugarlos directamente.
- Los niveles online pueden abrirse mediante la busqueda de Geometry Dash.

## v0.1.1

- Anadida navegacion real desde la interfaz principal hacia las listas por categoria.
- Anadida lista desplazable para mostrar demons registrados.
- Cargada la clasificacion inicial de 25 demons del jugador para las pruebas de desarrollo.
- El panel principal muestra el hardest y el total de demons registrados.

## v0.1.0

- Creada la estructura inicial del mod.
- Anadido el archivo de configuracion de Geode.
- Anadido el proyecto CMake.
- Anadido un hook inicial en el menu principal.
- Anadida la primera interfaz principal con botones de categorias.
- Primera prueba real completada correctamente en Android.
