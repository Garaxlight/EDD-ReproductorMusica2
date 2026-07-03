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
            std::string nombre;
            std::string artista;  // Campo para compatibilidad con Heap compare()
            int reproducciones;

            Artist() : reproducciones(0) {
            }

            Artist(const char* n, int r) : nombre(n), artista(n), reproducciones(r){
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
                if (idx >= 0 && idx < cantidad){
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


        //Extraer artistas unicos con reproducciones usando Heap
        static Heap<Artist*>* getTopArtistsHeap(LinkedList<Song>& catalog, int topN = 10){
            Heap<Artist*>* heap = new Heap<Artist*>();
            Artist* tempArtists = new Artist[100];
            int artistCount = 0;

            Node<Song>* actual = catalog.getHead();

            while (actual != nullptr){
                bool encontrado = false;

                for (int i = 0; i < artistCount; ++i){
                    if (tempArtists[i].nombre == actual->data.artista){
                        tempArtists[i].reproducciones += actual->data.reproducciones;
                        encontrado = true;
                        break;
                    }
                }

                if (!encontrado && artistCount < 100){
                    tempArtists[artistCount] = Artist(actual->data.artista.c_str(), actual->data.reproducciones);
                    artistCount++;
                }
                actual = actual->next;
            }

            // Insertar todos los artistas en el Heap (se ordena automáticamente)
            for (int i = 0; i < artistCount; ++i){
                heap->insert(&tempArtists[i]);
            }

            delete[] tempArtists;
            return heap;
        }

        // Obtener canciones de un artista especifico

        static LinkedList<Song>* getSongsFromArtist(const std::string& artistName, LinkedList<Song>& catalog){
            LinkedList<Song>* artistSongs = new LinkedList<Song>();
            Node<Song>* actual = catalog.getHead();

            while (actual != nullptr){
                if (actual->data.artista == artistName){
                    artistSongs->insertEnd(actual->data);
                }
                actual = actual->next;
            }
            return artistSongs;
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
        static void showSongsFromArtist(Player& player, const std::string& artistName, LinkedList<Song>& catalog);
};

extern void clearScreen();