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

    Heap<Song*> songHeap = getTopSongs(catalog, 10);

    if (songHeap.isEmpty()){
        clearScreen();
        cout << "No hay mas canciones en el catalogo." << endl;
        system("pause");
        return;
    }

    // Extraer los top 10 del heap
    Song** topSongs = new Song*[10];
    int topCount = 0;
    while (!songHeap.isEmpty() && topCount < 10){
        topSongs[topCount++] = songHeap.extractMax();
    }

    bool enTopSongs = true;

    while (enTopSongs){
        clearScreen();
        cout << "Ranking TOP " << topCount << " Canciones mas escuchadas:\n" << endl;

        for (int i=0; i< topCount; i++){
            cout << "[" << setw(2) << topSongs[i]->reproducciones << "] "
            << topSongs[i]->nombre << " - " << topSongs[i]->artista << endl;
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
                player.song = *(topSongs[num]);
                player.isPlaying = true;

                clearScreen();
                cout << "Reproduciendo: " << topSongs[num]->nombre << " - " << topSongs[num]->artista << endl;
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
                player.queue.enqueue(*(topSongs[num]));

                clearScreen();
                cout << "Cancion agregada a la lista de reproduccion: " << topSongs[num]->nombre << endl;
                system("pause");

            }
        }else  if ((comando == 'A' || comando == 'a') && input.length() == 1){
            enTopSongs = false;
            delete[] topSongs;
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

    delete[] topSongs;

}

void TopMenu::showTopArtists(Player& player, LinkedList<Song>& catalog){
    Heap<Artist*>* artistsHeap = getTopArtistsHeap(catalog, 10);

    if (artistsHeap->isEmpty()){
        clearScreen();
        cout << "No hay canciones en el catalogo." << endl;
        system("pause");
        delete artistsHeap;
        return;
    }

    // Extraer los top 10 del heap
    Artist** topArtists = new Artist*[10];
    int topCount = 0;
    while (!artistsHeap->isEmpty() && topCount < 10){
        topArtists[topCount++] = artistsHeap->extractMax();
    }

    bool enTopArtists = true;

    while (enTopArtists){
        clearScreen();
        cout << "Ranking TOP " << topCount << " Artistas mas escuchados:\\n" << endl;

        for (int i = 0; i < topCount; i++){
            cout << (i + 1) << ". [" << setw(2) << topArtists[i]->reproducciones << "] " << topArtists[i]->nombre << endl;
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

            if (num >= 0 && num < topCount){
                showSongsFromArtist(player, topArtists[num]->nombre, catalog);
            }else {
                clearScreen();
                cout << "Posicion invalida." << endl;
                system("pause");
            }
        }else if (comando == 'C' || comando == 'c'){
            enTopArtists = false;
            delete[] topArtists;
            delete artistsHeap;
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

    delete[] topArtists;
    delete artistsHeap;
}

void TopMenu::showSongsFromArtist(Player& player, const std::string& artistName, LinkedList<Song>& catalog){
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
