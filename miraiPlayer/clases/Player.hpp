#pragma once
#include "../clases/Song.hpp"
#include "../estructuras/LinkedList.hpp"
#include "../estructuras/Queue.hpp"
#include "../estructuras/Stack.hpp"

class Player {
    public:
        Song song;
        bool isPlaying;
        bool isShuffle;
        int repeatMode; // 0 para off, 1 para repetir una, 2 para repetir todas
        Queue<Song> queue;
        Stack<Song> history;

        void playPause();
        void prevTrack();
        void nextTrack();
        void toggleShuffle();
        void toggleRepeat();
        void initializeQueueFromCatalog(const LinkedList<Song>& catalog, int currentSongId = -1);
        void shuffleQueue();

        Player(){
            song = Song();
            song.id = -1;
            isPlaying = false;
            isShuffle = false;
            repeatMode = 0;
        }
};