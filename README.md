# Pac-Man - Analisis de Algoritmos

Proyecto del curso **Analisis de Algoritmos** basado en Pac-Man. El juego esta implementado en C++ para Windows y permite practicar el analisis y la implementacion de recorridos y decisiones sobre una grilla.

El objetivo es completar los niveles comiendo toda la comida. El trabajo principal consiste en completar tres funciones dentro de `fantasma.cpp`; `juego.h` ya contiene la interfaz del juego y no debe modificarse.

## Reglas principales

- `#`: pared. Pac-Man y los fantasmas no pueden atravesarla.
- `.`: comida normal. Otorga 10 puntos.
- `o`: pildora de poder. Otorga 50 puntos y hace que los fantasmas se vuelvan asustados temporalmente.
- `~`: lodo. Cuando un fantasma entra en una casilla de lodo, debe esperar 2 turnos adicionales antes de volver a moverse.
- ` `: casilla vacia.
- `P`: posicion inicial de Pac-Man.
- `G`: posicion inicial de un fantasma.

Los fantasmas normales no se mueven en todos los turnos y los fantasmas asustados se mueven con una frecuencia menor. Si Pac-Man pierde todas sus vidas, termina el juego. Al comer una pildora puede perseguir y comer a los fantasmas asustados temporalmente.

Los niveles se encuentran en `mapas/nivel1.txt` hasta `mapas/nivel6.txt`. Un nivel se considera roto si existe comida que Pac-Man no puede alcanzar desde su posicion inicial. Los niveles rotos se detectan al cargar el juego y se omiten.

## Archivos del proyecto

- `fantasma.cpp`: archivo que deben modificar los integrantes del proyecto. Contiene las declaraciones pendientes de las tres funciones solicitadas y el `main`.
- `juego.h`: logica completa del juego. Define el mapa, las posiciones, el movimiento, las colisiones y el menu. No debe modificarse.
- `mapas/`: contiene los niveles disponibles. No debe modificarse. Para agregar un nivel se crea `nivelN.txt` y se actualiza la cantidad de niveles configurada en `juego.h`.
- `Contexto/Proyecto grafos.pdf`: material de apoyo del enunciado.

Mantener todos los archivos y carpetas juntos es necesario para encontrar `juego.h` y los mapas al ejecutar el programa.

## Funciones por implementar

No modifique `juego.h`: implemente unicamente el contenido pendiente de las siguientes funciones en `fantasma.cpp`.

### `mapaValido`

Debe determinar si Pac-Man puede llegar desde su posicion inicial a toda la comida del mapa. La comida incluye tanto los puntos normales (`.`) como las pildoras de poder (`o`).

El resultado permite distinguir niveles jugables de niveles rotos. Si alguna casilla con comida es inaccesible por paredes o por la configuracion de regiones del mapa, la funcion debe indicar que el nivel no es valido.

La implementacion propuesta se basa en **BFS (Busqueda en Anchura)** desde la posicion de Pac-Man. La cola permite procesar las casillas por capas de distancia y el registro de visitados evita procesar una misma casilla varias veces o quedar atrapado en recorridos ciclicos.

Cuando termina el recorrido, se revisa toda la grilla; si alguna casilla que contiene comida no fue visitada, significa que pertenece a una componente inaccesible y el mapa no es valido.

**Complejidad:**

- **Tiempo:** `O(F * C)`, siendo `F` y `C` la cantidad de filas y columnas. Cada casilla se agrega y procesa una vez, y se hacen cuatro intentos de movimiento por casilla. La revision final tambien recorre las `F * C` casillas.
- **Espacio:** `O(F * C)`. Se almacenan la matriz de visitados y, en el peor caso, una cola con casillas de la grilla.

### `pasoPerseguir`

Dada la posicion actual de un fantasma y la posicion de Pac-Man, debe elegir una casilla vecina que permita atrapar a Pac-Man lo antes posible.

La decision debe considerar que entrar al lodo aumenta el tiempo efectivo de movimiento del fantasma. Si desde la posicion actual no existe un camino util hacia Pac-Man, la funcion debe indicar que el fantasma debe quedarse.

### `pasoEscapar`

Dada la posicion actual de un fantasma asustado y la posicion de Pac-Man, debe elegir una casilla vecina que aleje al fantasma lo mas posible de Pac-Man.

Si moverse no proporciona una ventaja o no existe una casilla vecina disponible que pueda recomendarse, la funcion debe indicar que el fantasma debe quedarse.

## Preparacion en Windows

### Requisito

Necesita un compilador de C++ compatible con Windows. Se recomienda **MinGW-w64 g++** o el compilador incluido en una instalacion de Visual Studio con soporte para C++.

Para verificar g++, abra **PowerShell** o **Símbolo del sistema** y ejecutelo:

```bat
g++ --version
```

Si Windows no reconoce `g++`, agregue al `PATH` la carpeta `bin` de MinGW-w64 o inicie el proyecto desde el Developer Command Prompt de Visual Studio.

### Organizacion previa

Coloque estos elementos en la misma carpeta principal:

```text
ProyectoAnalisis/
|-- fantasma.cpp
|-- juego.h
|-- README.md
`-- mapas/
    |-- nivel1.txt
    |-- nivel2.txt
    |-- nivel3.txt
    |-- nivel4.txt
    |-- nivel5.txt
    `-- nivel6.txt
```

Ejecute la compilacion desde esa carpeta principal. Si la consola esta en otra ubicacion, use `cd` para entrar a la carpeta del proyecto.

## Compilar y ejecutar

### Con MinGW-w64

1. Abra PowerShell en la carpeta del proyecto.
2. Compile:

```bat
g++ -std=c++17 fantasma.cpp -o pacman.exe
```

3. Ejecute desde la misma carpeta:

```bat
.\pacman.exe
```

Mantenga abierta la ventana de consola mientras juega.

### Con Visual Studio

1. Abra Visual Studio y seleccione **Abrir un proyecto o solución desde carpeta** o cree un proyecto vacío de consola.
2. Agregue `fantasma.cpp` y `juego.h` al proyecto sin eliminarlos de su carpeta actual.
3. Configure el proyecto como aplicación de consola de Windows.
4. Compile con `Ctrl+Shift+B`.
5. Ejecute con `Ctrl+F5`.

Al ejecutarlo desde Visual Studio, asegure que el directorio de trabajo sea la carpeta principal del proyecto. Así `juego.h` puede incluirse correctamente y `mapas/nivelN.txt` puede encontrarse al iniciar.

## Controles

En el menu:

- `1`: jugar todos los niveles en orden. Los niveles rotos se omiten.
- `2`: elegir directamente un nivel del 1 al 6.
- `3`: salir.
- Para elegir el numero en la opcion 2, presione la tecla del nivel deseado.

Durante el juego:

- Flecha arriba o `W`: mover hacia arriba.
- Flecha abajo o `S`: mover hacia abajo.
- Flecha izquierda o `A`: mover hacia la izquierda.
- Flecha derecha o `D`: mover hacia la derecha.
- `Q`: salir del juego.

La direccion se cambia al pulsar una tecla de movimiento. El proyecto ya incluye el control de entrada dentro de `juego.h`; no es necesario implementarlo en `fantasma.cpp`.
