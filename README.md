# MIRAI-PLAYER 2

> Reproductor de música por consola desarrollado en C++ con estructuras de datos personalizadas.

---

## Integrantes

Ramiro Alvarado Durán ||
Patricio Alvarado Durán

---

## Descripción

MIRAI-PLAYER es un reproductor de música por consola desarrollado en C++.
Simula la reproducción y gestión de canciones desde un archivo local, implementando estructuras de datos como linked lists, pilas y colas. Incluye reproducción/pausa, shuffle, repetición y gestión de listas de reproducción. Las canciones se cargan desde el archivo music_source.txt, mientras que el estado de la sesión se guarda en status.cfg para retomar la reproducción al abrir el programa.

---

## Diagrama de Clases

```mermaid
classDiagram
    class Song {
        -int id
        -string nombre
        -string artista
        -string album
        -int año
        -int duracion
        -string ubicacion
        +Song()
        +mostrar()
        +toString(separador) string
    }

    class Player {
        -Song song
        -bool isPlaying
        -bool isShuffle
        -int repeatMode
        -Queue~Song~ queue
        -Stack~Song~ history
        +Player()
        +playPause()
        +prevTrack()
        +nextTrack()
        +toggleShuffle()
        +toggleRepeat()
    }

    class Node~T~ {
        -T data
        -Node~T~* next
        +Node(data)
    }

    class LinkedList~T~ {
        -Node~T~* head
        +LinkedList()
        +insertEnd(data)
        +insertFront(data)
        +removeFirst()
        +removeAt(index)
        +getAt(index) T
        +size() int
        +getHead() Node~T~*
    }

    class Queue~T~ {
        -Node~T~* head
        -Node~T~* back
        +Queue()
        +enqueue(value)
        +dequeue()
        +front() T
        +isEmpty() bool
        +pushFront(value)
        +getHead() Node~T~*
    }

    class Stack~T~ {
        -Node~T~* topNode
        +Stack()
        +push(value)
        +pop()
        +top() T
        +isEmpty() bool
    }

    class Status {
        +int currentSongId
        +bool shuffle
        +int repeatMode
    }

    class FileManager {
        +static loadMusic(filename, lista)
        +static loadStatus(filename) Status
        +static saveStatus(filename, status)
    }

    Song --> Player : contiene
    Song --> LinkedList~T~ : almacena en
    Player --> Queue~Song~ : contiene
    Player --> Stack~Song~ : contiene
    LinkedList~T~ --> Node~T~ : usa
    Queue~T~ --> Node~T~ : usa
    Stack~T~ --> Node~T~ : usa
    LinkedList~T~ --> FileManager : carga datos
    FileManager --> Status : carga/guarda
```

---

### Requisitos
- Compilador compatible con C++14 o superior (GCC, MinGW, Cygwin)

---

## Instrucciones de compilación y ejecución

### Windows con MinGW, GCC o Cygwin



### Controles

| Tecla | Acción |
|---|---|
| W | Reproducir / Pausar |
| Q | Pista anterior |
| E | Pista siguiente |
| S | Activar / Desactivar modo shuffle |
| R | Repetición (Desactivado / Repetir una / Repetir todas) |
| A | Ver lista de reproducción actual |
| L | Listado de canciones |
| X | Salir |

---

## Estructura del Proyecto

```
├── miraiPlayer/
│   ├── clases/
│   │   ├── Player.hpp / .cpp
│   │   ├── Playlist.hpp
│   │   └── Song.hpp / .cpp
│   ├── estructuras/
│   │   ├── LinkedList.hpp
│   │   ├── Node.hpp
│   │   ├── Queue.hpp
│   │   ├── Stack.hpp
│   │   └── Status.hpp
│   └── nucleo/
│       ├── FileManager.hpp
│       └── SongMenu.cpp
├── main.cpp
├── README.md
└── .gitignore
```

---

*Taller 2 – Estructuras de Datos, Primer Semestre 2026 | Universidad Católica del Norte*
