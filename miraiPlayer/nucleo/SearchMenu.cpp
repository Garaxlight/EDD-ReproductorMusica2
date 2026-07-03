#include <cctype>
#include <iostream>
#include <limits>
#include <string>
#include "../estructuras/Utils.hpp"
#include "SearchMenu.hpp"

using namespace std;

namespace {
bool parseIndex(const std::string& input, int& index) {
    if (input.size() < 2) {
        return false;
    }

    std::string numericPart = input.substr(1);
    if (numericPart.empty()) {
        return false;
    }

    try {
        size_t parsedChars = 0;
        int parsedValue = std::stoi(numericPart, &parsedChars);
        if (parsedChars != numericPart.size()) {
            return false;
        }
        index = parsedValue - 1;
        return true;
    } catch (...) {
        return false;
    }
}

void mostrarResultados(const LinkedList<Song*>& resultados, const std::string& query) {
    cout << "Busqueda de canciones" << endl;
    cout << "Canciones que contienen \"" << query << "\":" << endl;

    if (resultados.isEmpty()) {
        cout << "No se encontraron canciones." << endl;
        return;
    }

    Node<Song*>* actual = resultados.getHead();
    int i = 1;
    while (actual != nullptr) {
        Song* song = actual->data;
        cout << i << ". " << song->nombre << " - " << song->artista << endl;
        actual = actual->next;
        i++;
    }
}

void mostrarOpcionesBusqueda() {
    cout << endl << "Opciones:" << endl;
    cout << "R<num> - Reproducir cancion seleccionada" << endl;
    cout << "A<num> - Agregar cancion seleccionada al final de la lista de reproduccion actual" << endl;
    cout << "F - Repetir busqueda con un texto diferente" << endl;
    cout << "V - Volver al menu principal" << endl;
    cout << "Ingrese Opcion: ";
}
}

void SearchMenu(MusicCatalog& catalog, Player& player) {
    bool enMenu = true;

    while (enMenu) {
        clearScreen();
        cout << "Buscar canciones que contengan:" << endl;
        cout << "Ingrese texto (Enter vacio para volver): ";

        string query;
        getline(cin, query);
        if (query.empty()) {
            return;
        }

        LinkedList<Song*> resultados;
        catalog.trie.search(query, resultados);

        bool revisandoResultados = true;
        while (revisandoResultados) {
            clearScreen();
            mostrarResultados(resultados, query);
            mostrarOpcionesBusqueda();

            string input;
            getline(cin, input);
            if (input.empty()) {
                continue;
            }

            char opcion = static_cast<char>(toupper(input[0]));

            switch (opcion) {
                case 'R': {
                    int index = -1;
                    if (!parseIndex(input, index) || index < 0 || index >= resultados.size()) {
                        cout << "Indice no valido." << endl;
                        break;
                    }

                    Song* selected = resultados.getAt(index);
                    player.playFromSelection(*selected, catalog.canciones, true);
                    incrementCurrentSongPlays(catalog, player);
                    cout << "Reproduciendo: " << selected->nombre << " - " << selected->artista << endl;
                    revisandoResultados = false;
                    enMenu = false;
                    break;
                }

                case 'A': {
                    int index = -1;
                    if (!parseIndex(input, index) || index < 0 || index >= resultados.size()) {
                        cout << "Indice no valido." << endl;
                        break;
                    }

                    Song* selected = resultados.getAt(index);
                    player.queue.enqueue(*selected);
                    cout << "Cancion agregada al final de la lista de reproduccion." << endl;
                    break;
                }

                case 'F':
                    revisandoResultados = false;
                    break;

                case 'V':
                    clearScreen();
                    return;

                default:
                    cout << "Opcion no valida." << endl;
            }
        }
    }
}
