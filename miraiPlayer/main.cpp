#include <iostream>
#include <limits>
#include <string>
#include "clases/Player.hpp"
#include "clases/Song.hpp"
#include "estructuras/LinkedList.hpp"
#include "estructuras/Status.hpp"
#include "estructuras/Utils.hpp"
#include "nucleo/FileManager.hpp"
#include "nucleo/PlaylistMenu.hpp"
#include "nucleo/SongMenu.hpp"
#include "nucleo/TopMenu.hpp"

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
    cout << "F - Buscar canciones" << endl;
    cout << "T - TOP 10 Artistas y Canciones" << endl;
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
    FileManager::loadPlayCount("song_ranking.txt", listaCanciones);

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
                clearScreen();
                player.playPause();
                cout << (player.isPlaying ? "Reproduciendo: " : "En pausa: ") << player.song.nombre << endl;
                break;
            case 'Q':
            case 'q':
                clearScreen();
                if(player.history.isEmpty()){
                    cout << "No hay pista anterior en el historial." << endl;
                } else {
                    player.prevTrack();
                    cout << "Pista anterior: " << player.song.nombre << endl;
                }
                break;
            case 'E':
            case 'e':
                clearScreen();
                if(player.queue.isEmpty() && player.repeatMode == 0) {
                    cout << "La cola de reproduccion esta vacia." << endl;
                } else {
                    player.nextTrack();
                    cout << "Siguiente pista: " << player.song.nombre << endl;
                }
                break;
            case 'S':
            case 's':
                clearScreen();
                player.toggleShuffle();
                cout << "Modo Aleatorio: " << (player.isShuffle ? "ON" : "OFF") << endl;
                break;
            case 'R':
            case 'r':
                clearScreen();
                player.toggleRepeat();
                cout << "Modo de repeticion: " << (player.repeatMode == 0 ? "OFF" : player.repeatMode == 1 ? "Repetir 1" : "Repetir todas") << endl;
                break;
            case 'A':
            case 'a':
                clearScreen();
                PlaylistMenu(player);
                break;
            case 'L':
            case 'l':
                clearScreen();
                SongMenu(listaCanciones, player);
                break;
            case 'T':
            case 't':
                clearScreen();
                TopMenu::showTopMenu(player, listaCanciones);
                break;
            case 'X':
            case 'x':
                clearScreen();
                ejecutando = false;
                status.shuffle = player.isShuffle;
                status.repeatMode = player.repeatMode;
                status.currentSongId = player.song.id;

                FileManager::saveStatus("status.cfg", status);
                FileManager::savePlayCount("song_ranking.txt", listaCanciones);
                
                cout << "Saliendo del reproductor..." << endl;
                break;
            default:
                cout << "Opcion no valida. Intente nuevamente" << endl;
        }
    }

    return 0;
}