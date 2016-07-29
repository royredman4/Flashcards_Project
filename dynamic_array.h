#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <iostream>
#include <cstdlib>

using namespace std;

template<typename e>
struct Array_Node{

   e value;
   Array_Node<e>* next;
   Array_Node(e val): value(val), next(NULL)
   {}
};


template<typename T>
class Dynamic_Array
{

public:

    Dynamic_Array(){
        _Head = _Tail = NULL;
        Arry_size = 0;
    }

    void push_back(T value)
    {
        Arry_size++;

        //cout << "Array Size is now " << Arry_size << endl;

        if (_Head == NULL)
        {
            _Head = new Array_Node<T>(value);
            _Tail = _Head;
        }

        else
        {
            Array_Node<T>* Temp = new Array_Node<T>(value);
            _Tail->next = Temp;
            _Tail= _Tail->next;
            Temp = NULL;
            delete Temp;
        }


    }

    void pop_back()
    {

        if (_Head == NULL)
           {
                return;
            }
        Array_Node<T>* Temp = _Head;

       if (_Head == _Tail)
        {
            _Head = _Tail = NULL;
        }

        else
        {
             while (Temp->next != _Tail)
             {
                Temp= Temp->next;
             }

            _Tail = Temp;
            Temp = Temp->next;
            _Tail->next = NULL;

        }
        delete Temp;
        Arry_size--;
    }

    void Empty_Array()
    {
        if (_Head == NULL)
            return;

        _Tail = NULL;
        while(_Head != NULL)
        {
            Array_Node<T>* Temp = _Head;
            _Head = _Head->next;

            delete Temp;
        }
        Arry_size = 0;
    }

    int size()
    {
        return Arry_size;
    }

    void Display()
    {

        Array_Node<T>* Temp = _Head;
        while(Temp != NULL)
         {
            cout << Temp->value << endl;
            Temp = Temp->next;
         }

        Temp = NULL;
        delete Temp;
    }

    T& operator[](int D)
    {
        return(getItem(D));
    }


    T& getItem(int number){

        Array_Node<T>* Temp = _Head;
        for (int i = 0; i < number; i++)
        {
            Temp = Temp->next;
        }
        return Temp->value;
    }

    void setItem(Array_Node<T>* Temp, T value)
    {
        Temp->value = value;
    }

    ~Dynamic_Array()
    {
        Empty_Array();
        delete _Head;
        delete _Tail;
    }


private:
    Array_Node<T>* _Head;
    Array_Node<T>* _Tail;
    int Arry_size;
};


#endif // DYNAMIC_ARRAY_H
