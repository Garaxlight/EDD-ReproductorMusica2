#include <iostream>
#include <limits>
#include <string>
#include "clases/Player.hpp"
#include "clases/Song.hpp"
#include "estructuras/LinkedList.hpp"
#include "estructuras/Status.hpp"
#include "nucleo/FileManager.hpp"
#include "nucleo/PlaylistMenu.hpp"
#include "nucleo/SongMenu.hpp"

using namespace std;

void mostrarMenu(){
    cout << "=== Reproductor de Musica 2 ===" << endl;
    cout << "W - Reproducir/Pausar" << endl;
    cout << "Q - Pista Anterior" << endl;
    cout << "E - Pista Siguiente" << endl;
    cout << "S - Modo Aleatorio" << endl;
    cout << "R - Repeticion" << endl;
    cout << "A - Ver lista actual" << endl;
    cout << "L - Lista de Canciones" << endl;
    cout << "X - Salir" << endl;
    cout << "============================" << endl;
    cout << "Ingrese una opcion: ";
}

int main(){
    Player player;
    LinkedList<Song> listaCanciones;

    if (!FileManager::loadMusic("music_source.txt", listaCanciones)){
        cout << "No se pudo cargar el catalogo de canciones. Se creara vacio al reiniciar." << endl;
    }

    Status status = FileManager::loadStatus("status.cfg");
    player.isShuffle = status.shuffle;
    player.repeatMode = status.repeatMode;

    if (!listaCanciones.isEmpty()){
        player.initializeQueueFromCatalog(listaCanciones, status.currentSongId);
    }

    bool ejecutando = true;
    while (ejecutando){
        mostrarMenu();

        char opcion = ' ';
        cin >> opcion;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcion){
            case 'W':
            case 'w':
                system("cls");
                player.playPause();
                cout << (player.isPlaying ? "Reproduciendo: " : "En pausa: ") << player.song.nombre << endl;
                break;
            case 'Q':
            case 'q':
                system("cls");
                if(player.history.isEmpty()){
                    cout << "No hay pista anterior en el historial." << endl;
                } else {
                    player.prevTrack();
                    cout << "Pista anterior: " << player.song.nombre << endl;
                }
                break;
            case 'E':
            case 'e':
                system("cls");
                if(player.queue.isEmpty()) {
                    cout << "La cola de reproduccion esta vacia." << endl;
                } else {
                    player.nextTrack();
                    cout << "Siguiente pista: " << player.song.nombre << endl;
                }
                break;
            case 'S':
            case 's':
                system("cls");
                player.toggleShuffle();
                cout << "Modo Aleatorio: " << (player.isShuffle ? "ON" : "OFF") << endl;
                break;
            case 'R':
            case 'r':
                system("cls");
                player.toggleRepeat();
                cout << "Modo de repeticion: " << (player.repeatMode == 0 ? "OFF" : player.repeatMode == 1 ? "Repetir 1" : "Repetir todas") << endl;
                break;
            case 'A':
            case 'a':
                system("cls");
                PlaylistMenu(player);
                break;
            case 'L':
            case 'l':
                system("cls");
                SongMenu(listaCanciones, player);
                break;
            case 'X':
            case 'x':
                system("cls");
                ejecutando = false;
                status.shuffle = player.isShuffle;
                status.repeatMode = player.repeatMode;
                status.currentSongId = player.song.id;
                FileManager::saveStatus("status.cfg", status);
                cout << "Saliendo del reproductor..." << endl;
                break;
            default:
                cout << "Opcion no valida. Intente nuevamente" << endl;
        }
    }

    return 0;
}