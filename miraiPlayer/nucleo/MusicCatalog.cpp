#include "MusicCatalog.hpp"

namespace {
void clearArtistEntries(MusicCatalog& catalog) {
    while (!catalog.artistas.isEmpty()) {
        ArtistEntry* entry = catalog.artistas.getAt(0);
        catalog.artistas.removeFirst();
        delete entry;
    }
}
}

ArtistEntry* findArtistEntry(MusicCatalog& catalog, const std::string& artista) {
    Node<ArtistEntry*>* actual = catalog.artistas.getHead();
    while (actual != nullptr) {
        if (actual->data->nombre == artista) {
            return actual->data;
        }
        actual = actual->next;
    }
    return nullptr;
}

void rebuildMusicCatalog(MusicCatalog& catalog) {
    catalog.trie.clear();
    clearArtistEntries(catalog);

    Node<Song>* actual = catalog.canciones.getHead();
    while (actual != nullptr) {
        Song* songPtr = &actual->data;
        catalog.trie.insertSong(songPtr);

        ArtistEntry* entry = findArtistEntry(catalog, actual->data.artista);
        if (entry == nullptr) {
            entry = new ArtistEntry(actual->data.artista);
            catalog.artistas.insertEnd(entry);
        }
        entry->arbol.insert(songPtr);

        actual = actual->next;
    }
}

void incrementCurrentSongPlays(MusicCatalog& catalog, Player& player) {
    if (player.song.id == -1 || !player.isPlaying) {
        return;
    }

    Node<Song>* actual = catalog.canciones.getHead();
    while (actual != nullptr) {
        if (actual->data.id == player.song.id) {
            actual->data.reproducciones++;
            player.song.reproducciones = actual->data.reproducciones;
            return;
        }
        actual = actual->next;
    }
}
