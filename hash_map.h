#ifndef LIB_HASHMAP
#define LIB_HASHMAP
#include <stdexcept>
#include <string>

namespace lib{

    namespace details{

        const unsigned int NUM_HASHER = 31337;
        const unsigned int DEFAULT_SIZE = 4;

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
        void rehash(unsigned int);

    public:
        typedef const hash_entry* const_iterator;
        typedef hash_entry* iterator;

        hash_map();
        explicit hash_map(unsigned int);
        hash_map(const hash_map<K,V>&);
        ~hash_map();
        inline iterator begin();
        inline iterator end();
        inline const_iterator cbegin() const;
        inline const_iterator cend() const;
        inline unsigned int size() const;
        inline unsigned int entries() const;
        bool find(const_iterator&,const_iterator&,const K&) const;
        void add(const K&, const V&);
        void remove(const K&);
        V& operator[](const K&);
    };


    template <typename K, typename V>
    hash_map<K,V>::hash_map() :
        hash_entries(new hash_entry[details::DEFAULT_SIZE]),
        hash_map_size(details::DEFAULT_SIZE),
        num_entries(0){
    }

    template <typename K, typename V>
    hash_map<K,V>::hash_map(unsigned int _size) :
        hash_entries(new hash_entry[_size]),
        hash_map_size(_size),
        num_entries(0){
    }

    template <typename K, typename V>
    hash_map<K,V>::hash_map(const hash_map<K,V>& old_map) :
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
    unsigned int hash_map<K,V>::entries() const{
        return num_entries;
    }

    template <typename K, typename V>
    bool hash_map<K,V>::find(const_iterator& _begin, const_iterator& _end, const K& target_key) const{
        for (const_iterator beg=_begin; beg!=_end; ++beg){
            if (beg->key == target_key){
                return true;
            }
        }
        return false;
    }

    template <typename K, typename V>
    void hash_map<K,V>::rehash(unsigned int new_size){
        hash_entry* new_hash_map = new hash_entry[new_size];
        unsigned int old_hash_map_size = hash_map_size;
        hash_map_size = new_size;
        num_entries = 0;
        for (unsigned int i=0; i<old_hash_map_size; ++i){
            if (hash_entries[i].is_filled){
                unsigned int position = details::generate_hash(hash_entries[i].key,hash_map_size);
                while (new_hash_map[position].is_filled){
                    position = (position+1)%hash_map_size;
                }
                new_hash_map[position] = hash_entry(hash_entries[i].key,hash_entries[i].value);
                num_entries += 1;
            }
        }
        delete[] hash_entries;
        hash_entries = new_hash_map;
    }

    template <typename K, typename V>
    void hash_map<K,V>::add(const K& key, const V& value){
        if (num_entries >= hash_map_size){
            rehash(hash_map_size*2);
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
        unsigned int original_position(position);
        for (unsigned int i=0; i<hash_map_size; ++i){
            position = (position+1)%hash_map_size;
            if (hash_entries[position].key == key){
                return hash_entries[position].value;
            }
        }
        if (num_entries >= hash_map_size){
            rehash(hash_map_size*2);
        }
        while (hash_entries[original_position].is_filled){
            original_position = (original_position+1)%hash_map_size;
        }
        hash_entries[original_position] = hash_entry(key,V());
        ++num_entries;
        return hash_entries[original_position].value;
    }

}

#endif // LIB_HASHMAP
