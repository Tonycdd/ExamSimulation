#pragma once
#include <cstdint>
#include <exception>
#include <cassert>
#include <iostream>

template <class T>
class MyVector
{
private:
    T* data;
    std::size_t size;
    std::size_t capacity;

    void del(){
        delete[] data;
        data = nullptr;
        size = 0;
        capacity = 0;
    }
    

    void resize(){
        std::size_t newCapacity = this->capacity*2; // shte e vinagi polojitelno
        try
        {
            T* newVector = new T[newCapacity];
            for (int i = 0; i < this->size; i++)
            {
                newVector[i] = this->data[i];
            }
            delete[] this->data;
            this->data = newVector;
            this->capacity = newCapacity;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            throw;
        }
    }



public:
    //Imame * -> dynamichno zadelqne - golqmo 4;

    //default ctor
    MyVector() : size(0),capacity(1)
    {
        this->data = new T[1];
    };

    MyVector(const MyVector<T>& other) : data(nullptr), size(0), capacity(0) {
    try {
        T* newData = new T[other.capacity];
        for (std::size_t i = 0; i < other.size; ++i) {
            newData[i] = other.data[i];
        }
        this->data = newData;
        this->size = other.size;
        this->capacity = other.capacity;
    } catch (const std::exception& e) {
        std::cerr << "Copy constructor failed: " << e.what() << '\n';
        throw;
    }
}

    //operator =
    MyVector<T>& operator=(const MyVector<T>& other)
    {
       if (this != &other) {
        T* newData = new T[other.capacity];
        for (size_t i = 0; i < other.size; ++i) {
            newData[i] = other.data[i];
        }
        delete[] data;
        data = newData;
        size = other.size;
        capacity = other.capacity;
    }
    return *this;
    }
    //dtor
    ~MyVector(){
        this->del();
    }


    //getters
    size_t getSize() const
    {
        return this->size;
    }

    size_t getCapacity() const {
        return this->capacity;
    }

    //accessors
    bool isEmpty() const 
    {
        return this->size <= 0;
    }

    T& front() const 
    {
        assert(this->size > 0);
        return this->data[0];
    }
    
    T& back() const 
    {
        return this->data[this->size - 1];
    }

    T& at(int index) const{
        assert(index >= 0 && index < this->size);
        return this->data[index];
    }

    //Modifiers
    void push_front(const T& val)
    {
        //iskame da dobavim element
        //da vidim dali imame mqsto v masiva ot elementi
        std::size_t size = this->size + 1;
        if(size > this->capacity){
            //ako nqmame mqsto trqbva da uvelichim mqstoto;
            try
            {
                this->resize();
            }
            
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                throw;
            }
        }
        // tuk imame mqsto vinagi;
        for (int i = this->size; i > 0; i--)
        {
            this->data[i] = this->data[i - 1];
        }
        //nakraq samo data[0] ostava svobodna kutiika
            this->data[0] = val;
            this->size++;
    }

    void push_back(const T& val)
    {
        int copySize = this->size + 1;
        if(copySize > this->capacity)
        {

            //nqma mqsto
            try
            {
                this->resize();
            }
            
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                throw;
            }
        }
        //tuk vinagi imq mqsto
    
        this->data[this->size] = val;
        this->size++;
    }

    void pop_front()
    {
        assert(this->size > 0);
        for (int i = 0; i < this->size - 1; i++)
        {
            this->data[i] = this->data[i+1];
        }
        this->size--;
    }

    void pop_back()
    {
        assert(this->size > 0);
        --this->size;
    }

    void insert(int index, const T& val) {   
    assert(index >= 0 && index <= size); // Можем да вмъкнем и в края
    if (size >= capacity) {
        resize();
    }
    for (int i = size; i > index; --i) {
        data[i] = data[i - 1];
    }
    data[index] = val;
    ++size;
}
    //opreator
    T& operator[](int ind) const
    {
        return this->at(ind);
    }

    void print() const{
        for (int i = 0; i < this->size; i++)
        {
            std::cout << this->data[i];
        }
        std::cout << std::endl;
    }

    void shrink_to_fit() {
        if (capacity > size) {
            T* newVector = new T[size];
            for (size_t i = 0; i < size; ++i) {
                newVector[i] = data[i];
            }
            delete[] data;
            data = newVector;
            capacity = size;
        }
    }

    void erase(std::size_t index) {
    assert(index >= 0 && index < size); // Проверяваме дали индексът е валиден

    // Преместваме всички елементи след `index` с една позиция наляво
    for (std::size_t i = index; i < size - 1; ++i) {
        data[i] = data[i + 1];
    }
    
    // Намаляваме размера на масива
    --size;
}
};