#ifndef _LIB_HASHMAP
#define _LIB_HASHMAP
#include <string>

namespace lib{

    namespace details{

        unsigned int generate_hash(const int& key, const size_t& hash_map_size){
            return key%hash_map_size;
        }

        unsigned int generate_hash(const std::string& key, const size_t& hash_map_size){
            return key.size()%hash_map_size;
        }

        unsigned int generate_hash(const char* key, const size_t& hash_map_size){
            size_t position = 0;
            for (size_t i=0; key[i]!='\0'; i++){
                position += key[i];
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
            hash_entry(){}
            hash_entry(const T& _key, const U& _value)
                : key(_key), value(_value){}
        };
        hash_entry* hash_entries;
        int* filled_positions;
        unsigned int hash_map_size;

    public:
        typedef hash_entry* const_iterator;
        explicit hash_map(unsigned int);
        ~hash_map();
        const_iterator cbegin();
        const_iterator cend();
        void add(const T&, const U&);
        U& operator[](const T&);
    };

    template <typename T, typename U>
    hash_map<T,U>::hash_map(unsigned int _size){
        hash_entries = new hash_entry[_size];
        filled_positions = new int[_size]();
        hash_map_size = _size;
    }

    template <typename T, typename U>
    hash_map<T,U>::~hash_map(){
        delete[] filled_positions;
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
    void hash_map<T,U>::add(const T& _key, const U& _value){
        unsigned int position = details::generate_hash(_key,hash_map_size);
        while (filled_positions[position]){
            position = (position+1)%hash_map_size;
        }
        filled_positions[position] = 1;
        hash_entries[position] = hash_entry(_key,_value);
    }

    template <typename T, typename U>
    U& hash_map<T,U>::operator[](const T& _key){
        unsigned int position = details::generate_hash(_key,hash_map_size);
        while (hash_entries[position].key != _key){
            position = (position+1)%hash_map_size;
        }
        return hash_entries[position].value;
    }

}

#endif // _LIB_HASHMAP
