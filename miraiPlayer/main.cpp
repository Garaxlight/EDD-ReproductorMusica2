#include <iostream>
#include <limits>
#include <string>
#include "clases/Player.hpp"
#include "clases/Song.hpp"
#include "estructuras/Status.hpp"
#include "estructuras/Utils.hpp"
#include "nucleo/FileManager.hpp"
#include "nucleo/MusicCatalog.hpp"
#include "nucleo/PlaylistMenu.hpp"
#include "nucleo/RankingMenu.hpp"
#include "nucleo/SearchMenu.hpp"
#include "nucleo/SongMenu.hpp"

using namespace std;

void mostrarEstado(const Player& player){
    if (player.song.id == -1){
        cout << "Sin reproduccion activa" << endl;
        return;
    }

    cout << (player.isPlaying ? "Reproduciendo" : "En pausa") << ": " << player.song.nombre << endl;
    cout << "Artista: " << player.song.artista << endl;
    cout << "Album: " << player.song.album << " [" << player.song.año << "]" << endl;
}

void mostrarMenu(const Player& player){
    mostrarEstado(player);
    cout << endl << "Opciones:" << endl;
    cout << "W - Reproducir/Pausar" << endl;
    cout << "Q - Pista Anterior" << endl;
    cout << "E - Pista Siguiente" << endl;
    cout << "S - Modo Aleatorio" << endl;
    cout << "R - Repeticion" << endl;
    cout << "A - Ver lista de reproduccion actual" << endl;
    cout << "L - Listado de canciones" << endl;
    cout << "F - Buscar canciones" << endl;
    cout << "T - TOP 10 Artistas y Canciones" << endl;
    cout << "X - Salir" << endl;
    cout << "Ingrese una opcion: ";
}

int main(){
    Player player;
    MusicCatalog catalog;

    if (!FileManager::loadMusic("music_source.txt", catalog.canciones)){
        cout << "No se pudo cargar el catalogo de canciones. Se creara vacio al reiniciar." << endl;
    }

    FileManager::loadPlayCount("song_ranking.txt", catalog.canciones);
    rebuildMusicCatalog(catalog);

    Status status = FileManager::loadStatus("status.cfg");
    player.isShuffle = status.shuffle;
    player.repeatMode = status.repeatMode;

    if (!catalog.canciones.isEmpty()){
        player.initializeQueueFromCatalog(catalog.canciones, status.currentSongId);
    }

    bool ejecutando = true;
    while (ejecutando){
        clearScreen();
        mostrarMenu(player);

        char opcion = ' ';
        cin >> opcion;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcion){
            case 'W':
            case 'w':
                clearScreen();
                player.playPause();
                if (player.isPlaying){
                    incrementCurrentSongPlays(catalog, player);
                }
                mostrarEstado(player);
                break;
            case 'Q':
            case 'q':
                clearScreen();
                if(player.history.isEmpty()){
                    cout << "No hay pista anterior en el historial." << endl;
                } else {
                    player.prevTrack();
                    incrementCurrentSongPlays(catalog, player);
                    mostrarEstado(player);
                }
                break;
            case 'E':
            case 'e':
                clearScreen();
                if(player.queue.isEmpty() && player.repeatMode == 0) {
                    cout << "La cola de reproduccion esta vacia." << endl;
                } else {
                    player.nextTrack();
                    incrementCurrentSongPlays(catalog, player);
                    mostrarEstado(player);
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
                SongMenu(catalog, player);
                break;
            case 'F':
            case 'f':
                SearchMenu(catalog, player);
                break;
            case 'T':
            case 't':
                RankingMenu(catalog, player);
                break;
            case 'X':
            case 'x':
                clearScreen();
                ejecutando = false;
                status.shuffle = player.isShuffle;
                status.repeatMode = player.repeatMode;
                status.currentSongId = player.song.id;
                FileManager::saveStatus("status.cfg", status);
                FileManager::savePlayCount("song_ranking.txt", catalog.canciones);
                cout << "Saliendo del reproductor..." << endl;
                break;
            default:
                cout << "Opcion no valida. Intente nuevamente" << endl;
        }
    }

    return 0;
}
