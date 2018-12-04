#ifndef _LIB_HASHMAP
#define _LIB_HASHMAP
#include <exception>
#include <string>

namespace lib{

    namespace details{
        
        const unsigned int NUM_HASHER = 1367; 

        unsigned int generate_hash(const int& key, const size_t& hash_map_size){
            return (key*NUM_HASHER)%hash_map_size;
        }

        unsigned int generate_hash(const std::string& key, const size_t& hash_map_size){
            size_t position = 0;
            for (size_t i=0; i<key.size(); i++){
                position += key[i]*i;
            }
            return position%hash_map_size;
        }

        unsigned int generate_hash(const char* key, const size_t& hash_map_size){
            size_t position = 0;
            for (size_t i=0; key[i]!='\0'; i++){
                position += key[i]*i;
            }
            return position%hash_map_size;
        }

    }


    template <typename T, typename U>
    class hash_map{

    private:
        struct hash_entry{
            T key;
            U value;
            bool is_filled;
            hash_entry() : is_filled(false){}
            hash_entry(const T& _key, const U& _value)
                : key(_key), value(_value), is_filled(true){}
        };
        hash_entry* hash_entries;
        unsigned int hash_map_size;
        unsigned int num_entries;

    public:
        typedef const hash_entry* const_iterator;
        typedef hash_entry* iterator;
        explicit hash_map(unsigned int);
        ~hash_map();
        iterator begin();
        iterator end();
        const_iterator cbegin();
        const_iterator cend();
        unsigned int size();
        void add(const T&, const U&);
        U& operator[](const T&);
    };

    template <typename T, typename U>
    hash_map<T,U>::hash_map(unsigned int _size):
        hash_entries(new hash_entry[_size]),
        hash_map_size(_size),
        num_entries(0){
    }

    template <typename T, typename U>
    hash_map<T,U>::~hash_map(){
        delete[] hash_entries;
    }

    template <typename T, typename U>
    typename hash_map<T,U>::const_iterator hash_map<T,U>::cbegin(){
        return hash_entries;
    }

    template <typename T, typename U>
    typename hash_map<T,U>::const_iterator hash_map<T,U>::cend(){
        return hash_entries+hash_map_size;
    }

    template <typename T, typename U>
    typename hash_map<T,U>::iterator hash_map<T,U>::begin(){
        return hash_entries;
    }

    template <typename T, typename U>
    typename hash_map<T,U>::iterator hash_map<T,U>::end(){
        return hash_entries+hash_map_size;
    }

    template <typename T, typename U>
    unsigned int hash_map<T,U>::size(){
        return hash_map_size;
    }

    template <typename T, typename U>
    void hash_map<T,U>::add(const T& _key, const U& _value){
        if (num_entries >= hash_map_size){
            throw std::out_of_range("std::out_of_range : hash_map full");
        }
        unsigned int position = details::generate_hash(_key,hash_map_size);
        while (hash_entries[position].is_filled){
            position = (position+1)%hash_map_size;
        }
        hash_entries[position] = hash_entry(_key,_value);
        num_entries += 1;
    }

    template <typename T, typename U>
    U& hash_map<T,U>::operator[](const T& _key){
        unsigned int position = details::generate_hash(_key,hash_map_size);
        unsigned int count = 0;
        while (hash_entries[position].key != _key){
            if (count > hash_map_size){
                throw std::invalid_argument("std::invalid_argument : key not in hash_map");
            }
            position = (position+1)%hash_map_size;
            count += 1;
        }
        return hash_entries[position].value;
    }

}

#endif // _LIB_HASHMAP
