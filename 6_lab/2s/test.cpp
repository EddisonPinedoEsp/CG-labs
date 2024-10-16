#include <iostream>

using namespace std;

// Estructura de un nodo en la lista enlazada
struct Nodo {
    int dato;
    Nodo* siguiente;
};

// Clase para representar la lista enlazada
class ListaEnlazada {
public:
    Nodo* cabeza;

    ListaEnlazada() {
        cabeza = nullptr;
    }

    // Función para insertar un nodo al inicio de la lista
    void insertarAlInicio(int dato) {
        Nodo* nuevoNodo = new Nodo;
        nuevoNodo->dato = dato;
        nuevoNodo->siguiente = cabeza;
        cabeza = nuevoNodo;
    }

    // Función para insertar un nodo al final de la lista
    void insertarAlFinal(int dato) {
        Nodo* nuevoNodo = new Nodo;
        nuevoNodo->dato = dato;
        nuevoNodo->siguiente = nullptr;

        if (cabeza == nullptr) {
            cabeza = nuevoNodo;
        } else {
            Nodo* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevoNodo;
        }
    }

    // Función para eliminar el primer nodo de la lista
    void eliminarPrimero() {
        if (cabeza == nullptr) {
            cout << "La lista está vacía." << endl;
            return;
        }
        Nodo* temp = cabeza;
        cabeza = cabeza->siguiente;
        delete temp;
    }

    // Función para imprimir los elementos de la lista
    void imprimir() {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            cout << actual->dato << " ";
            actual = actual->siguiente;
        }
        cout << endl;
    }
};

// Función para probar la inserción al inicio y al final
void test1() {
    ListaEnlazada lista;

    // Insertar al inicio
    lista.insertarAlInicio(5);
    lista.insertarAlInicio(3);
    lista.insertarAlInicio(1);

    // Insertar al final
    lista.insertarAlFinal(7);
    lista.insertarAlFinal(9);

    cout << "Test 1: Inserciones al inicio y al final: ";
    lista.imprimir();

    // Verificaciones adicionales (por ejemplo, verificar el tamaño de la lista, el valor del primer y último nodo)
    // ...
}

// Función para probar la eliminación y casos límite
void test2() {
    ListaEnlazada lista;

    // Insertar algunos elementos
    lista.insertarAlInicio(5);
    lista.insertarAlFinal(7);

    // Eliminar el primer elemento
    lista.eliminarPrimero();

    // Intentar eliminar de una lista vacía
    lista.eliminarPrimero();

    // Insertar más elementos y eliminar el último
    lista.insertarAlInicio(3);
    lista.insertarAlFinal(9);
    // ... (implementar la función para eliminar el último elemento)

    cout << "Test 2: Eliminaciones y casos límite: ";
    lista.imprimir();

    // Verificaciones adicionales (por ejemplo, verificar si la lista está vacía después de eliminar todos los elementos)
    // ...
}

int main() {
    // ... (resto del código del main)
    test1();
    test2();
    return 0;
}