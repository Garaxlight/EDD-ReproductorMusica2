#include <cctype>
#include <iostream>
#include <string>
#include "PlaylistMenu.hpp"

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
}

void PlaylistMenu(Player& player){
    system("cls");
    bool enMenu = true;
    string input;

    while (enMenu){
        cout << "Actual: " << player.song.nombre << " - " << player.song.artista << endl;
        cout << "\nLista de reproduccion actual:" << endl;

        Node<Song>* actual = player.queue.getHead();
        int i = 1;

        if (player.queue.isEmpty()){
            cout << "Lista vacia." << endl;
        }else{
            while (actual != nullptr){
                cout << i << ". " << actual->data.nombre << " - " << actual->data.artista << endl;
                actual = actual->next;
                i++;
            }
        }

        cout << "\nOpciones:" << endl;
        cout << "S<num> - Saltar a la cancion" << endl;
        cout << "V - Volver" << endl;
        cout << "Ingrese una opcion: ";

        cin >> input;
        if (input.empty()){
            continue;
        }

        char opcion = static_cast<char>(toupper(input[0]));

        switch (opcion){
            case 'S':{
                system("cls");
                int index = -1;
                if (!parseIndex(input, index) || index < 0){
                    cout << "Indice invalido" << endl;
                    break;
                }

                try{
                    Node<Song>* temp = player.queue.getHead();
                    for (int j = 0; j < index && temp != nullptr; j++){
                        temp = temp->next;
                    }

                    if (temp == nullptr){
                        throw runtime_error("Indice invalido");
                    }

                    player.song = temp->data;
                    for (int j = 0; j <= index; j++){
                        player.queue.dequeue();
                    }

                    cout << "Saltando a: " << player.song.nombre << endl;
                    enMenu = false;
                }catch (...){
                    cout << "Indice invalido" << endl;
                }
                break;
            }

            case 'V':
                system("cls");
                enMenu = false;
                break;

            default:
                cout << "Opcion invalida" << endl;
        }
    }
}
