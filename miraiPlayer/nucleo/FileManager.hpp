#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include "../clases/Song.hpp"
#include "../estructuras/LinkedList.hpp"
#include "../estructuras/Node.hpp"
#include "../estructuras/Status.hpp"

class FileManager{
    public:
    static bool loadMusic(const std::string& filename, LinkedList<Song>& lista){
        lista.clear();

        std::ifstream file(resolveMusicFilePath(filename));
        if (!file.is_open()){
            return false;
        }

        std::string line;
        while (std::getline(file, line)){
            if (line.empty()){
                continue;
            }

            std::stringstream ss(line);
            std::string fields[7];
            std::string token;
            int fieldCount = 0;
            while (fieldCount < 7 && std::getline(ss, token, ',')){
                fields[fieldCount++] = token;
            }

            if (fieldCount != 7) {
                continue;
            }

            Song s;
            if (!parseInt(fields[0], s.id)){
                continue;
            }

            s.nombre = fields[1];
            s.artista = fields[2];
            s.album = fields[3];
            if (!parseInt(fields[4], s.año)){
                continue;
            }
            if (!parseInt(fields[5], s.duracion)){
                continue;
            }
            s.ubicacion = fields[6];

            lista.insertEnd(s);
        }

        file.close();
        return true;
    }

    static Status loadStatus(const std::string& filename){
        std::ifstream file(filename);

        Status s;
        s.currentSongId = -1;
        s.shuffle = false;
        s.repeatMode = 0;

        if (!file.is_open()){
            return s;
        }

        std::string line;
        while (std::getline(file, line)){
            std::stringstream ss(line);
            std::string key, value;
            std::getline(ss, key, '=');
            std::getline(ss, value);

            if (key == "CURRENT"){
                parseInt(value, s.currentSongId);
            }else if (key == "SHUFFLE"){
                s.shuffle = (value == "1");
            }else if (key == "REPEAT"){
                parseInt(value, s.repeatMode);
            }
        }

        file.close();
        return s;
    }

    static void saveStatus(const std::string& filename, const Status& s){
        std::ofstream file(filename);
        if (!file.is_open()){
            return;
        }

        file << "CURRENT=" << s.currentSongId << std::endl;
        file << "SHUFFLE=" << (s.shuffle ? "1" : "0") << std::endl;
        file << "REPEAT=" << s.repeatMode << std::endl;

        file.close();
    }

    static bool saveMusic(const std::string& filename, LinkedList<Song>& lista){
        std::ofstream file(resolveMusicFilePath(filename));
        if (!file.is_open()){
            return false;
        }

        Node<Song>* actual = lista.getHead();
        while(actual != nullptr){
            file << actual->data.toString(',') << std::endl;
            actual = actual->next;
        }

        file.close();
        return true;
    }

    private:
    static std::string resolveMusicFilePath(const std::string& filename){
        const std::string primary = filename.empty() ? "music_source.txt" : filename;

        std::ifstream primaryFile(primary);
        if (primaryFile.good()){
            return primary;
        }

        return primary;
    }

    static bool parseInt(const std::string& value, int& result){
        if (value.empty()){
            return false;
        }

        try {
            size_t parsedChars = 0;
            int parsedValue = std::stoi(value, &parsedChars);
            if (parsedChars != value.size()){
                return false;
            }
            result = parsedValue;
            return true;
        } catch (...) {
            return false;
        }
    }
};