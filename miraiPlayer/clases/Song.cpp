#include "Song.hpp"
#include <iostream>
using namespace std;

Song::Song() {
    id = 0;
    nombre = "";
    artista = "";
    album = "";
    año = 0;
    duracion = 0;
    ubicacion = "";
}

Song::Song(int id, string nombre, string artista, 
    string album, int año, int duracion, string ubicacion) {
    this->id = id;
    this->nombre = nombre;
    this->artista = artista;
    this->album = album;
    this->año = año;
    this->duracion = duracion;
    this->ubicacion = ubicacion;
}

void Song::mostrar() const {
    cout << nombre << " - " << artista << endl;
}

string Song::toString(char separador) const {
    return to_string(id) + separador + nombre + separador + artista +
    separador + album + separador + to_string(año) + separador + to_string(duracion)
    + separador + ubicacion;
}