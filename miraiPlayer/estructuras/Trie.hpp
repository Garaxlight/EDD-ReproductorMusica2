#pragma once
#include <string>
#include "LinkedList.hpp"

template <typename T>
struct TrieNode {
    TrieNode* children[256];
    LinkedList<T> songs;

    TrieNode() {
        for (int i = 0; i < 256; i++) {
            children[i] = nullptr;
        }
    }
};

template <typename T>
class Trie {
private:
    TrieNode<T>* root;

    void toLower(std::string& str) const {
        for (int i = 0; i < (int)str.length(); i++) {
            char c = str[i];
            if (c >= 'A' && c <= 'Z') {
                str[i] = c - 'A' + 'a';
            }
        }
    }

    void deleteTree(TrieNode<T>* node) {
        if (node == nullptr) return;
        for (int i = 0; i < 256; i++) {
            if (node->children[i] != nullptr) {
                deleteTree(node->children[i]);
            }
        }
        delete node;
    }

    void addSongToNode(TrieNode<T>* node, T song) {
        int size = node->songs.size();
        for (int j = 0; j < size; j++) {
            if (node->songs.getAt(j) == song) {
                return;
            }
        }
        node->songs.insertEnd(song);
    }

    void insert(const std::string& text, T song) {
        if (text.empty() || song == nullptr) return;

        std::string lowercase = text;
        toLower(lowercase);

        TrieNode<T>* current = root;
        for (int i = 0; i < (int)lowercase.length(); i++) {
            unsigned char index = static_cast<unsigned char>(lowercase[i]);
            if (current->children[index] == nullptr) {
                current->children[index] = new TrieNode<T>();
            }
            current = current->children[index];
            addSongToNode(current, song);
        }
    }

    void insertSuffixes(const std::string& text, T song) {
        if (text.empty() || song == nullptr) return;

        std::string lowercase = text;
        toLower(lowercase);

        for (int start = 0; start < (int)lowercase.length(); start++) {
            insert(lowercase.substr(start), song);
        }
    }

public:
    Trie() {
        root = new TrieNode<T>();
    }

    ~Trie() {
        deleteTree(root);
    }

    void insertSong(T song) {
        if (song == nullptr) return;
        insertSuffixes(song->nombre, song);
        insertSuffixes(song->artista, song);
    }

    void search(const std::string& query, LinkedList<T>& results) {
        while (!results.isEmpty()) {
            results.removeFirst();
        }

        if (query.empty()) return;

        std::string lowercase = query;
        toLower(lowercase);

        TrieNode<T>* current = root;
        for (int i = 0; i < (int)lowercase.length(); i++) {
            unsigned char index = static_cast<unsigned char>(lowercase[i]);
            if (current->children[index] == nullptr) {
                return;
            }
            current = current->children[index];
        }

        int size = current->songs.size();
        for (int i = 0; i < size; i++) {
            T song = current->songs.getAt(i);
            int resSize = results.size();
            bool found = false;
            for (int j = 0; j < resSize; j++) {
                if (results.getAt(j) == song) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                results.insertEnd(song);
            }
        }
    }

    void clear() {
        deleteTree(root);
        root = new TrieNode<T>();
    }
};
