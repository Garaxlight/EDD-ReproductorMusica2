#include "TopMenu.hpp"
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <cstring>

using namespace std;

void TopMenu::showTopMenu(Player& player, LinkedList<Song>& catalog){
    bool enMenu = true;

    while (enMenu){
        clearScreen();
        cout << "Ranking TOP" << endl;
        cout << "C - Top 10 canciones más escuchadas" << endl;
        cout << "A - Top 10 artistas más escuchados" << endl;
        cout << "X - Salir" << endl;
        cout << "\nIngrese una opcion: ";
        char opcion;
        cin >> opcion;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcion){
            case 'C':
            case 'c':
                clearScreen();
                showTopSongs(player, catalog);
                break;
            case 'A':
            case 'a':
                clearScreen();
                showTopArtists(player, catalog);
                break;
            case 'X':
            case 'x':
                enMenu = false;
                break;
            default:
                cout << "Opcion no valida. Intente nuevamente" << endl;
                system("pause");
                break;
        }
    }
}

void TopMenu::showTopSongs(Player& player, LinkedList<Song>& catalog){

    int songCount = 0;
    Node<Song>* temp = catalog.getHead();

    while (temp != nullptr){
        songCount++;
        temp = temp->next;
    }

    if (songCount == 0){
        clearScreen();
        cout << "No hay mas canciones en el catalogo." << endl;
        system("pause");
        return;
    }

    Song** songArray = new Song*[songCount];
    Node<Song>* actual = catalog.getHead();
    int idx = 0;
    while (actual !=nullptr){
        songArray[idx++] = &(actual->data);
        actual = actual->next;
    }

    /*Bubble sort por:
        1. Reproducciones (descendente)
        2. Nombre (Ascendente)
        3. Artista (Ascendente)
    */

    for (int i = 0; i < songCount - 1; i++){
        for (int j = 0; j < songCount -i -1; j++){
            bool swapped = false;

            if (songArray[j]->reproducciones < songArray[j+1]->reproducciones){
                swapped = true;
            }else if(songArray[j]->reproducciones == songArray[j+1]->reproducciones){
                if(songArray[j]->nombre > songArray[j+1]->nombre){
                    swapped = true;
                }else if (songArray[j]->nombre == songArray[j+1]->nombre){
                    if(songArray[j]->artista > songArray[j+1]->artista){
                        swapped = true;
                    }
                }
            }
            if (swapped) {
                Song* temp = songArray[j];
                songArray[j] = songArray[j+1];
                songArray[j+1] = temp;
            }
        }
    }

    int topCount = (songCount > 10) ? 10 : songCount;

    bool enTopSongs = true;

    while (enTopSongs){
        clearScreen();
        cout << "Ranking TOP" << topCount << "Canciones mas escuchadas:\n" << endl;

        for (int i=0; i< topCount; i++){
            cout << "[" << setw(2) << songArray[i]->reproducciones << "] "
            << songArray[i]->nombre << " - " << songArray[i]->artista << endl;
        }

        cout << "\nOpciones: " << endl;
        cout << "R<num> - Reproducir cancion seleccionada" << endl;
        cout << "A<num> - Agregar canción seleccionada al final de la lista de reproducción actual" << endl;
        cout << "A - Top 10 artistas mas escuchados" << endl;
        cout << "V - Volver al menu principal" << endl;
        cout << "\nIngrese Opcion:";

        string input;
        getline(cin, input);

        if (input.empty()){
            continue;
        }

        char comando = input[0];

        if ((comando == 'R' || comando == 'r') && input.length() > 1){
            string numStr = input.substr(1);
            int num = -1;
            try{
                num = stoi(numStr) - 1;
            }catch(...){
                clearScreen();
                cout << "Numero invalido" << endl;
                system("pause");
                continue;
            }
            if (num >= 0 && num < topCount){
                player.queue.dequeue();
                player.initializeQueueFromCatalog(catalog, -1);
                player.shuffleQueue();
                player.song = *(songArray[num]);
                player.isPlaying = true;

                clearScreen();
                cout << "Reproduciendo: " << songArray[num]->nombre << " - " << songArray[num]->artista << endl;
                system("pause");

            } else {
                clearScreen();
                cout << "Posicion invalida." << endl;
                system("pause");
            }
            
        }else  if ((comando == 'A' || comando == 'a') && input.length() > 1){
            string numStr = input.substr(1);
            int num = -1;

            try{
                num = stoi(numStr) - 1;
            }catch(...){
                clearScreen();
                cout << "Numero invalido." << endl;
                system ("pause");
                continue;
            }
            if (num >=0 && num < topCount){
                player.queue.enqueue(*(songArray[num]));

                clearScreen();
                cout << "Cancion agregada a la lista de reproduccion: " << songArray[num]->nombre << endl;
                system("pause");

            }
        }else  if ((comando == 'A' || comando == 'a') && input.length() == 1){
            enTopSongs = false;
            delete[] songArray;
            showTopArtists(player, catalog);
            return;
        }else if (comando == 'V' || comando == 'v'){
            enTopSongs = false;
        }else{
            clearScreen();
            cout << "Opcion invalida." << endl;
            system("pause");
        }
    }

    delete[] songArray;

}

void TopMenu::showTopArtists(Player& player, LinkedList<Song>& catalog){
    TopList<Artist>* topArtists = getTopArtists(catalog, 10);

    if (topArtists->getSize() == 0){
        clearScreen();
        cout << "No hay canciones en el catalogo." << endl;
        system("pause");
        delete topArtists;
        return;
    }

    bool enTopArtists = true;

    while (enTopArtists){
        clearScreen();
        cout << "Ranking TOP" << topArtists->getSize() << "Artistas mas escuchados:\n" << endl;

        for (int i = 0; i < topArtists->getSize(); i++){
            Artist artist = topArtists->getAt(i);
            cout << (i + 1) << ". [" << setw(2) << artist.reproducciones << "] " << artist.name << endl;
        }

        cout << "\nOpciones:" << endl;
        cout << "S<num> - Mostrar canciones del artista" << endl;
        cout << "C - Top 10 canciones mas escuchadas" << endl;
        cout << "V - Volver al menu principal" << endl;
        cout << "\nIngrese opcion: ";

        string input;
        getline(cin, input);

        if (input.empty()){
            continue;
        }
        char comando = input[0];

        if ((comando == 'S' || comando == 's') && input.length() > 1){
            string numStr = input.substr(1);
            int num = -1;

            try{
                num = stoi(numStr) - 1;
            }catch(...){
                clearScreen();
                cout << "Numero invalido." << endl;
                system("pause");
                continue;
            }

            if (num >= 0 && num < topArtists->getSize()){
                Artist artist = topArtists->getAt(num);
                showSongsFromArtist(player, artist.name, catalog);
            }else {
                clearScreen();
                cout << "Posicion invalida." << endl;
                system("pause");
            }
        }else if (comando == 'C' || comando == 'c'){
            enTopArtists = false;
            delete topArtists;
            showTopSongs(player, catalog);
            return;
        }else if (comando == 'V' || comando == 'v'){
            enTopArtists = false;
        } else{
            clearScreen();
            cout << "Opcion invalida." << endl;
            system("pause");
        }
    }

    delete topArtists;
}

void TopMenu::showSongsFromArtist(Player& player, const char* artistName, LinkedList<Song>& catalog){
    LinkedList<Song>* artistSongs = getSongsFromArtist(artistName, catalog);

    //Contar canciones
    int songCount = 0;
    Node<Song>* temp = artistSongs->getHead();

    while (temp != nullptr){
        songCount++;
        temp = temp->next;
    }

    if (songCount == 0){
        clearScreen();
        cout << "No hay canciones para este artista." << endl;
        system("pause");
        delete artistSongs;
        return;
    }

    //Crear arreglo y ordenar alfabeticamente

    Song** songs = new Song*[songCount];
    Node<Song>* actual = artistSongs->getHead();
    int idx = 0;
    while (actual != nullptr){
        songs[idx++] = &(actual->data);
        actual = actual->next;
    }

    //Bubble sort por nombre

    for (int i = 0 ; i < songCount - 1 ; i++){
        for (int j = 0 ; j < songCount - i -1 ; j++){
            if (songs[j]->nombre > songs[j+1]->nombre){
                Song* temp = songs[j];
                songs[j] = songs[j+1];
                songs[j+1] = temp;
            }
        }
    }

    bool enArtistSongs = true;

    while (enArtistSongs){
        clearScreen();
        cout << "Ranking TOP Artistas mas escuchados:" << endl;
        cout << "Artista: \n" << artistName <<  endl;
        
        for (int i = 0; i < songCount; i++){
            cout << (i+1) << ". " << songs[i]->nombre << endl;
        }

        cout << "\nOpciones: " << endl;
        cout << "R<num> - Reproducir cancion seleccionada" << endl;
        cout << "A<num> - Agregar canción seleccionada al final de la lista de reproducción actual" << endl;
        cout << "V - Volver al listado de TOP 10 Artistas" << endl;
        cout << "X - Volver al menu principal" << endl;
        cout << "\nIngrese Opcion:";

        string input;
        getline(cin, input);

        if (input.empty()){
            continue;
        }

        char comando = input[0];

        if ((comando == 'R' || comando == 'r') && input.length() > 1){
            string numStr = input.substr(1);
            int num = -1;

            try{
                num = stoi(numStr) - 1;
            }catch(...){
                clearScreen();
                cout << "Numero invalido." << endl;
                system("pause");
                continue;
            }

            if (num >= 0 && num < songCount){
                player.queue.dequeue();
                player.initializeQueueFromCatalog(catalog, -1);
                player.shuffleQueue();
                player.song = *(songs[num]);
                player.isPlaying = true;

                clearScreen();
                cout << "Reproduciendo: " << songs[num]->nombre << " - "
                << songs[num]->artista << endl;
                system("pause");
            }else{
                clearScreen();
                cout << "Numero invalido." << endl;
                system("pause");
            }
        }else if ((comando == 'A' || comando == 'a') && input.length() > 1){
            string numStr = input.substr(1);
            int num = -1;

            try{
                num = stoi(numStr) - 1;
            }catch(...){
                clearScreen();
                cout << "Numero invalido." << endl;
                system("pause");
                continue;
            }

            if (num >= 0 && num < songCount){
                player.queue.enqueue(*(songs[num]));

                clearScreen();
                cout << "Cancion agregada a la lista de reproduccion: "
                << songs[num]->nombre << endl;
                system("pause");
            }
        }else if (comando == 'V' || comando == 'v'){
            enArtistSongs = false;
        }else if (comando == 'X' || comando == 'x'){
            enArtistSongs = false;
            delete[] songs;
            delete artistSongs;
            return;

        }else {
            clearScreen();
            cout << "Opcion invalida." << endl;
            system("pause");

        }
    }

    delete[] songs;
    delete artistSongs;
}
