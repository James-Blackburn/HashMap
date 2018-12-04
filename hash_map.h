#ifndef _LIB_HASHMAP
#define _LIB_HASHMAP
#include <stdexcept>
#include <string>

namespace lib{

    namespace details{

        const unsigned int NUM_HASHER = 31337;

        unsigned int generate_hash(const int& key, const unsigned int& hash_map_size){
            return (key*NUM_HASHER)%hash_map_size;
        }

        unsigned int generate_hash(const std::string& key, const unsigned int& hash_map_size){
            unsigned int position = 0;
            for (unsigned int i=0; i<key.size(); i++){
                position += key[i]*i;
            }
            return position%hash_map_size;
        }

        unsigned int generate_hash(const char* key, const unsigned int& hash_map_size){
            unsigned int position = 0;
            for (unsigned int i=0; key[i]!='\0'; i++){
                position += key[i]*i;
            }
            return position%hash_map_size;
        }

    }


    template <typename K, typename V>
    class hash_map{

    private:
        struct hash_entry{
            K key;
            V value;
            bool is_filled;
            hash_entry() : is_filled(false){}
            hash_entry(const K& _key, const V& _value)
                : key(_key), value(_value), is_filled(true){}
        };
        hash_entry* hash_entries;
        unsigned int hash_map_size;
        unsigned int num_entries;
    public:
        typedef const hash_entry* const_iterator;
        typedef hash_entry* iterator;
        explicit hash_map(unsigned int);
        hash_map(const hash_map<K,V>&);
        ~hash_map();
        iterator begin();
        iterator end();
        const_iterator cbegin() const;
        const_iterator cend() const;
        unsigned int size() const;
        void add(const K&, const V&);
        void remove(const K&);
        V& operator[](const K&);
    };

    template <typename K, typename V>
    hash_map<K,V>::hash_map(unsigned int _size):
        hash_entries(new hash_entry[_size]),
        hash_map_size(_size),
        num_entries(0){
    }

    template <typename K, typename V>
    hash_map<K,V>::hash_map(const hash_map<K,V>& old_map):
        hash_map_size(old_map.size()),
        hash_entries(new hash_entry[old_map.size()]),
        num_entries(0){
        for (hash_map<K,V>::const_iterator entry=old_map.cbegin(); entry!=old_map.cend(); entry++){
            if (entry->is_filled){
                add(entry->key,entry->value);
            }
        }
    }

    template <typename K, typename V>
    hash_map<K,V>::~hash_map(){
        delete[] hash_entries;
    }

    template <typename K, typename V>
    typename hash_map<K,V>::const_iterator hash_map<K,V>::cbegin() const{
        return hash_entries;
    }

    template <typename K, typename V>
    typename hash_map<K,V>::const_iterator hash_map<K,V>::cend() const{
        return hash_entries+hash_map_size;
    }

    template <typename K, typename V>
    typename hash_map<K,V>::iterator hash_map<K,V>::begin(){
        return hash_entries;
    }

    template <typename K, typename V>
    typename hash_map<K,V>::iterator hash_map<K,V>::end(){
        return hash_entries+hash_map_size;
    }

    template <typename K, typename V>
    unsigned int hash_map<K,V>::size() const{
        return hash_map_size;
    }

    template <typename K, typename V>
    void hash_map<K,V>::add(const K& key, const V& value){
        if (num_entries >= hash_map_size){
            throw std::out_of_range("std::out_of_range : hash_map full");
        }
        unsigned int position = details::generate_hash(key,hash_map_size);
        while (hash_entries[position].is_filled){
            position = (position+1)%hash_map_size;
        }
        hash_entries[position] = hash_entry(key,value);
        num_entries += 1;
    }

    template <typename K, typename V>
    void hash_map<K,V>::remove(const K& key){
        unsigned int position = details::generate_hash(key,hash_map_size);
        for (unsigned int i=0; i<hash_map_size; ++i){
            position = (position+1)%hash_map_size;
            if (hash_entries[position].key == key){
                hash_entries[position] = hash_entry();
                num_entries--;
                return;
            }
        }
        throw std::invalid_argument("std::invalid_argument : key not in hash_map");
    }

    template <typename K, typename V>
    V& hash_map<K,V>::operator[](const K& key){
        unsigned int position = details::generate_hash(key,hash_map_size);
        for (unsigned int i=0; i<hash_map_size; ++i){
            position = (position+1)%hash_map_size;
            if (hash_entries[position].key == key){
                return hash_entries[position].value;
            }
        }
        throw std::invalid_argument("std::invalid_argument : key not in hash_map");
    }

}

#endif // _LIB_HASHMAP

