#pragma once
#include "../clases/Player.hpp"
#include "../clases/Song.hpp"
#include "../estructuras/AVL.hpp"
#include "../estructuras/LinkedList.hpp"
#include "../estructuras/Trie.hpp"

struct ArtistEntry {
    std::string nombre;
    AVL<Song*> arbol;

    explicit ArtistEntry(const std::string& name) : nombre(name) {}
};

struct MusicCatalog {
    LinkedList<Song> canciones;
    Trie<Song*> trie;
    LinkedList<ArtistEntry*> artistas;
};

void rebuildMusicCatalog(MusicCatalog& catalog);
void incrementCurrentSongPlays(MusicCatalog& catalog, Player& player);
ArtistEntry* findArtistEntry(MusicCatalog& catalog, const std::string& artista);
