#include <cctype>
#include <iostream>
#include <limits>
#include <string>
#include "../clases/Player.hpp"
#include "../clases/Song.hpp"
#include "../estructuras/LinkedList.hpp"
#include "../nucleo/FileManager.hpp"

using namespace std;

namespace {
bool parseIndex(const std::string& input, int& index){
    if (input.size() < 2){
        return false;
    }

    std::string numericPart = input.substr(1);
    if (numericPart.empty()){
        return false;
    }

    try {
        size_t parsedChars = 0;
        int parsedValue = std::stoi(numericPart, &parsedChars);
        if (parsedChars != numericPart.size()){
            return false;
        }
        index = parsedValue - 1;
        return true;
    } catch (...) {
        return false;
    }
}

bool parsePositiveInt(const std::string& value, int& result){
    if (value.empty()){
        return false;
    }

    try {
        size_t parsedChars = 0;
        int parsedValue = std::stoi(value, &parsedChars);
        if (parsedChars != value.size() || parsedValue <= 0){
            return false;
        }
        result = parsedValue;
        return true;
    } catch (...) {
        return false;
    }
}
}

void mostrarCanciones(LinkedList<Song>& lista){
    Node<Song>* actual = lista.getHead();
    int i = 1;

    cout << "\n=== CANCIONES REGISTRADAS ===" << endl;
    while(actual != nullptr){
        cout << i << ". " << actual->data.nombre << " - " << actual->data.artista << endl;
        actual = actual->next;
        i++;
    }
}

void menuCanciones(LinkedList<Song>& lista, Player& player){
    string input;
    bool enMenu = true;

    while(enMenu){
        system("cls");
        mostrarCanciones(lista);
        cout << "\n=== OPCIONES ===" << endl;
        cout << "R<num> - Reproducir cancion" << endl;
        cout << "A<num> - Agregar cancion a cola" << endl;
        cout << "N - Nueva cancion" << endl;
        cout << "D<num> - Eliminar cancion" << endl;
        cout << "V - Volver" << endl;
        cout << "Ingrese una opcion: ";

        cin >> input;
        if (input.empty()){
            continue;
        }

        char opcion = static_cast<char>(toupper(input[0]));

        switch (opcion){
            case 'R':{
                int index = -1;
                if (!parseIndex(input, index) || index < 0 || index >= lista.size()){
                    cout << "Indice no valido." << endl;
                    break;
                }

                try{
                    Song s = lista.getAt(index);
                    Song anterior = player.song;

                    player.song = s;
                    player.isPlaying = true;
                    cout << "Reproduciendo: " << s.nombre << " - " << s.artista << endl;

                    while(!player.queue.isEmpty()){
                        player.queue.dequeue();
                    }

                    while(!player.history.isEmpty()){
                        player.history.pop();
                    }

                    if (anterior.id != -1){
                        player.history.push(anterior);
                    }
                }catch (...){
                    cout << "Indice no valido." << endl;
                }
                break;
            }

            case 'A':{
                int index = -1;
                if (!parseIndex(input, index) || index < 0 || index >= lista.size()){
                    cout << "Indice no valido." << endl;
                    break;
                }

                try{
                    Song s = lista.getAt(index);
                    player.queue.enqueue(s);
                    cout << "Cancion agregada a la cola." << endl;
                }catch (...){
                    cout << "Indice no valido." << endl;
                }
                break;
            }

            case 'N':{
                Song nueva;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Ingrese nombre: ";
                getline(cin, nueva.nombre);

                cout << "Ingrese artista: ";
                getline(cin, nueva.artista);

                cout << "Ingrese album: ";
                getline(cin, nueva.album);

                cout << "Ingrese año: ";
                std::string yearInput;
                getline(cin, yearInput);
                if (!parsePositiveInt(yearInput, nueva.año)){
                    cout << "Año invalido. La cancion no fue registrada." << endl;
                    break;
                }

                cout << "Ingrese duracion (segundos): ";
                std::string durationInput;
                getline(cin, durationInput);
                if (!parsePositiveInt(durationInput, nueva.duracion)){
                    cout << "Duracion invalida. La cancion no fue registrada." << endl;
                    break;
                }

                cout << "Ingrese ubicacion: ";
                getline(cin, nueva.ubicacion);

                if (nueva.nombre.empty() || nueva.artista.empty() || nueva.album.empty() || nueva.ubicacion.empty()){
                    cout << "Los campos obligatorios no pueden estar vacios." << endl;
                    break;
                }

                nueva.id = lista.size() + 1;
                lista.insertEnd(nueva);
                FileManager::saveMusic("music_source.txt", lista);
                cout << "Cancion agregada." << endl;
                break;
            }

            case 'D':{
                int index = -1;
                if (!parseIndex(input, index) || index < 0 || index >= lista.size()){
                    cout << "Indice no valido." << endl;
                    break;
                }

                try{
                    lista.removeAt(index);
                    FileManager::saveMusic("music_source.txt", lista);
                    cout << "Cancion eliminada." << endl;
                }catch (...){
                    cout << "Indice no valido." << endl;
                }
                break;
            }

            case 'V':
                system("cls");
                enMenu = false;
                break;

            default:
                cout << "Opcion no valida, intente de nuevo." << endl;
        }
    }
}

void SongMenu(LinkedList<Song>& lista, Player& player) {
    menuCanciones(lista, player);
}