#pragma once
#include <string>

class Song {
    public:

        int id;
        std::string nombre;
        std::string artista;
        std::string album;
        int año;
        int duracion;
        std::string ubicacion;

        Song();
        
        Song(int id, std::string nombre, std::string artista, 
            std::string album, int año, int duracion, std::string ubicacion);

        void mostrar()const;


        std::string toString(char separador) const;
};