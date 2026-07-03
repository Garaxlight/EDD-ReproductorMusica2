#pragma once
#include "../clases/Song.hpp"
#include "../estructuras/LinkedList.hpp"
#include "../estructuras/Queue.hpp"
#include "../estructuras/Stack.hpp"
#include "../estructuras/Heap.hpp"

class Player {
    public:
        Song song;
        bool isPlaying;
        bool isShuffle;
        int repeatMode; // 0 para off, 1 para repetir una, 2 para repetir todas
        Queue<Song> queue;
        Stack<Song> history;
        LinkedList<Song>* catalog;
        Heap<Song*> songHeap;

        void playPause();
        void prevTrack();
        void nextTrack();
        void toggleShuffle();
        void toggleRepeat();
        void initializeQueueFromCatalog(LinkedList<Song>& catalog, int currentSongId = -1);
        void playFromSelection(const Song& selected, LinkedList<Song>& catalog, bool randomizeQueue);
        void shuffleQueue();
        void incrementPlayCount();
        Heap<Song*> getSongHeap() const;

        Player(){
            song = Song();
            song.id = -1;
            isPlaying = false;
            isShuffle = false;
            repeatMode = 0;
            catalog = nullptr;
        }
};