#ifndef _LIB_HASHMAP
#define _LIB_HASHMAP

template <typename T, typename U>
class hash_map{
public:
    typedef size_t size_type;
private:
    struct hash_entry{
        T key;
        U value;
        hash_entry(){}
        hash_entry(const T& _key, const U& _value) : key(_key), value(_value){}
    };
    hash_entry* internal_data;
    int* array_positions;
    size_type array_size;
    size_type generate_hash(const T&);
public:
    explicit hash_map(size_type);
    ~hash_map();
    void add(const T&, const U&);
    U& operator[](const T&);
};

template <typename T, typename U>
hash_map<T,U>::hash_map(size_type _size){
    internal_data = new hash_entry[_size];
    array_positions = new int[_size]();
    array_size = _size;
}

template <typename T, typename U>
hash_map<T,U>::~hash_map(){
    delete[] array_positions;
    delete[] internal_data;
}

template <typename T, typename U>
typename hash_map<T,U>::size_type hash_map<T,U>::generate_hash(const T& _key){
    size_type position = 0;
    for (char c : _key){
        position += c;
    }
    position = position%array_size;
    return position;
}

template <typename T, typename U>
void hash_map<T,U>::add(const T& _key, const U& _value){
    size_type position = generate_hash(_key);
    while (array_positions[position]){
        position = (position+1)%array_size;
    }
    array_positions[position] = 1;
    internal_data[position] = hash_entry(_key,_value);
}

template <typename T, typename U>
U& hash_map<T,U>::operator[](const T& _key){
    size_type position = generate_hash(_key);
    while (internal_data[position].key != _key){
        position = (position+1)%array_size;
    }
    return internal_data[position].value;
}

#endif // _LIB_HASHMAP
