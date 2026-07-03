#include "Player.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

void Player::playPause(){
    if (song.id == -1 && queue.isEmpty()){
        cout << "No hay canciones en la cola" << endl;
        return;
    }
    isPlaying = !isPlaying;
}

void Player::toggleShuffle(){
    isShuffle = !isShuffle;
    if (isShuffle) {
        shuffleQueue();
    }
}

void Player::toggleRepeat(){
    switch(repeatMode){
        case 0:
            repeatMode = 1;
            break;
        case 1:
            repeatMode = 2;
            break;
        case 2:
            repeatMode = 0;
            break;
    }
}

void Player::initializeQueueFromCatalog(LinkedList<Song>& catalog, int currentSongId){
    this->catalog = &catalog;
    songHeap.clear();


    while(!queue.isEmpty()){
        queue.dequeue();
    }
    while(!history.isEmpty()){
        history.pop();
    }

    const Node<Song>* actual = catalog.getHead();
    if (actual == nullptr){
        song = Song();
        song.id = -1;
        isPlaying = false;
        return;
    }

    bool foundCurrent = false;
    while(actual != nullptr){
        if (actual->data.id == currentSongId){
            song = actual->data;
            foundCurrent = true;
            break;
        }
        actual = actual->next;
    }

    if (!foundCurrent){
        song = catalog.getHead()->data;
    }

    actual = catalog.getHead();
    while(actual != nullptr){

        songHeap.insert(const_cast<Song*>(&actual->data));
        
        if (actual->data.id != song.id){
            queue.enqueue(actual->data);
        }
        actual = actual->next;
    }

    isPlaying = false;
}

void Player::playFromSelection(const Song& selected, LinkedList<Song>& catalog, bool randomizeQueue){
    song = selected;
    isPlaying = true;

    while(!queue.isEmpty()){
        queue.dequeue();
    }
    while(!history.isEmpty()){
        history.pop();
    }

    const Node<Song>* actual = catalog.getHead();
    while(actual != nullptr){
        if (actual->data.id != song.id){
            queue.enqueue(actual->data);
        }
        actual = actual->next;
    }

    if (randomizeQueue && !queue.isEmpty()){
        shuffleQueue();
    }
}

void Player::shuffleQueue(){
    if (queue.isEmpty()){
        return;
    }

    int count = 0;
    Node<Song>* actual = queue.getHead();
    while(actual != nullptr){
        count++;
        actual = actual->next;
    }

    Song* items = new Song[count];
    actual = queue.getHead();
    for (int i = 0; i < count; ++i){
        items[i] = actual->data;
        actual = actual->next;
    }

    while(!queue.isEmpty()){
        queue.dequeue();
    }

    static bool seeded = false;
    if (!seeded){
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        seeded = true;
    }

    for (int i = count - 1; i > 0; --i){
        int j = std::rand() % (i + 1);
        Song temp = items[i];
        items[i] = items[j];
        items[j] = temp;
    }

    for (int i = 0; i < count; ++i){
        queue.enqueue(items[i]);
    }

    delete[] items;
}

void Player::nextTrack(){
    if (repeatMode == 1){
        isPlaying = true;
        return;
    }

    if (queue.isEmpty()){
        if (repeatMode == 2){
            if (song.id != -1){
                history.push(song);
            }

            while(!history.isEmpty()){
                queue.pushFront(history.top());
                history.pop();
            }

            if (isShuffle){
                shuffleQueue();
            }
        }else{
            cout << "La cola de reproduccion esta vacia" << endl;
            return;
        }
    }

    if(song.id != -1){
        history.push(song);
    }

    if (isShuffle){
        if (queue.isEmpty()){
            isPlaying = true;
            return;
        }

        int count = 0;
        Node<Song>* actual = queue.getHead();
        while(actual != nullptr){
            count++;
            actual = actual->next;
        }

        Song* items = new Song[count];
        actual = queue.getHead();
        for (int i = 0; i < count; ++i){
            items[i] = actual->data;
            actual = actual->next;
        }

        while(!queue.isEmpty()){
            queue.dequeue();
        }

        static bool seeded = false;
        if (!seeded){
            std::srand(static_cast<unsigned>(std::time(nullptr)));
            seeded = true;
        }

        for (int i = count - 1; i > 0; --i){
            int j = std::rand() % (i + 1);
            Song temp = items[i];
            items[i] = items[j];
            items[j] = temp;
        }

        song = items[0];
        for (int i = 1; i < count; ++i){
            queue.enqueue(items[i]);
        }

        delete[] items;
    }else{
        song = queue.front();
        queue.dequeue();
    }
    incrementPlayCount();
    isPlaying = true;
}

void Player::prevTrack() {
    if (history.isEmpty()){
        cout << "No hay historial." << endl;
        return;
    }

    if (song.id != -1){
        queue.pushFront(song);
    }

    song = history.top();
    history.pop();

    incrementPlayCount();
    isPlaying = true;
}
void Player::incrementPlayCount(){
    if (catalog == nullptr || song.id == -1){
        return;
    }

    Node<Song>* actual = catalog->getHead();

    while (actual != nullptr){
        if (actual-> data.id == song.id){
            actual-> data.reproducciones++;
            song.reproducciones = actual->data.reproducciones;

            songHeap.rebuild();

            return;
        }
        actual = actual->next;
    }
}
Heap<Song*> Player::getSongHeap() const {
    return songHeap;
}