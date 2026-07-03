#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include "../clases/Player.hpp"
#include "../clases/Song.hpp"
#include "../estructuras/Heap.hpp"
#include "../estructuras/LinkedList.hpp"
#include "../estructuras/Node.hpp"




class TopMenu {
    private:
        struct Artist{
            char name[100];
            int reproducciones;

            Artist() : reproducciones(0) {
                name[0] = '\0';
            }

            Artist(const char* n, int r) : reproducciones(r){
                strcpy(name,n);
            }
        };

        // Almacenar Top N elementos
        template <typename T>
        struct TopList{
            T* elementos;
            int cantidad;
            int capacidad;

            TopList(int cap = 10) : cantidad(0), capacidad(cap){
                elementos = new T[capacidad];
            }

            ~TopList(){
                delete[] elementos;
            }

            void agregar(T elem){
                if (cantidad < capacidad){
                    elementos[cantidad++] = elem;
                }
            }
            
            int getSize() const {
                return cantidad;
            }

            T getAt(int idx) const {
                if (idx >= = && idx < cantidad){
                    return elementos[idx];
                }
                throw std::runtime_error("Indice fuera de rango");
            }
        };


        // Obtener canciones ordenadas por reproducciones
        static Heap<Song*> getTopSongs(LinkedList<Song>& catalog, int topN = 10){
            Heap<Song*> heap;
            Node<Song>* actual = catalog.getHead();

            while (actual != nullptr){
                heap.insert(&actual->data);
                actual = actual->next;
            }
            return heap;
        }


        //Extraer artistas unicos con reproducciones
        static TopList<Artist> getTopArtists(LinkedList<Song>& catalog, int topN = 10){
            TopList<Artist>* artists = new TopList<Artist>(topN);
            Artist* tempArtists = new Artist[100]; // Array temporal para guardar artistas unicos
            int artistCount = 0;

            Node<Song>* actual = catalog.getHead();

            while (actual != nullptr){
                bool encontrado = false;

                for (int i = 0; i < artistCount; ++i){
                    if (strcmp(tempArtists[i].name, actual->data.artista.c_str()) == 0){
                        tempArtists[i].reproducciones += actual->data.reproducciones;
                        encontrado = true;
                        break;
                    }
                }

                if (!encontrado && artistCount < 100){
                    tempArtists[artistCount++] = Artist(actual->data.artista.c_str(), actual->data.reproducciones);
                    artistCount++;
                }
                actual = actual->next;
            }

            // 1. Ordenar artistas por reproducciones
            // 2. Ordenar artistas alfabeticamente

            for (int i = 0; i < artistCount - 1; ++i){
                for (int j = 0; j < artistCount - i - 1; j++){
                    int cmp = 0;
                    if (tempArtists[j].reproducciones != tempArtists[j+1].reproducciones){
                        cmp = tempArtists[j+1].reproducciones - tempArtists[j].reproducciones;
                    } else {
                        cmp = strcmp(tempArtists[j].name, tempArtists[j+1].name);
                    }

                    if (cmp < 0){
                        Artist temp = tempArtists[j];
                        tempArtists[j] = tempArtists[j+1];
                        tempArtists[j+1] = temp;
                    }
                }
            }

            //Agregar Top N
            int limite = (artistCount < topN) ? artistCount : topN;
            for (int i = 0; i < limite; i++){
                artists->agregar(tempArtists[i]);
            }

            delete[] tempArtists;
            return *artists;
        }

        // Obtener canciones de un artista especifico

        static LinkedList<Song> getSongsFromArtist(const char* artistName, LinkedList<Song>& catalog){
            LinkedList<Song>* artistSongs = new LinkedList<Song>();
            Node<Song>* actual = catalog.getHead();

            while (actual != nullptr){
                if (strcmp(actual->data.artista.c_str(), artistName) == 0){
                    artistSongs->insertEnd(actual->data);
                }
                actual = actual->next;
            }
            return *artistSongs;
        }

        //Bubble sort para canciones

        static void sortSongsAlphabetically(LinkedList<Song>* lista){
            Node<Song>* actual = lista->getHead();

            if (actual == nullptr) return;

            bool swapped;
            do {
                swapped = false;
                actual = lista->getHead();

                while (actual != nullptr && actual->next != nullptr){
                    if (actual->data.nombre > actual->next->data.nombre){
                        Song temp = actual->data;
                        actual->data = actual->next->data;
                        actual->next->data = temp;
                        swapped = true;
                    }
                    actual = actual->next;
                }
            }while (swapped);
        }
    public:
        static void showTopMenu(Player& player, LinkedList<Song>& catalog);
        static void showTopSongs(Player& player, LinkedList<Song>& catalog);
        static void showTopArtists(Player& player, LinkedList<Song>& catalog);
        static void showSongsFromArtist(Player& player, LinkedList<Song>& catalog);
};

extern void clearScreen();