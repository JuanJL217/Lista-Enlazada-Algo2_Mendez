#include "pila.h"
#include "lista.h"

struct pila {
    Lista* lista_enlazada;
};

Pila* pila_crear()
{
    Pila* pila = malloc(sizeof(Pila));
    if (!pila) {
        return NULL;
    }
    Lista* lista = lista_crear();
    if (!lista) {
        free(pila);
        return NULL;
    }
    pila->lista_enlazada = lista;
    return pila;
}

size_t pila_cantidad(Pila* pila)
{   
    return lista_cantidad_elementos(pila->lista_enlazada);;
}

void* pila_tope(Pila* pila)
{   
    void* tope = NULL;
    void** buscar = &tope;
    if(!lista_obtener_elemento(pila->lista_enlazada, 0, buscar)) {
        return NULL;
    }
    return tope;
}

bool pila_apilar(Pila* pila, void* cosa)
{
    if (!lista_agregar_elemento(pila->lista_enlazada, 0, cosa)) {
        return false;
    }
    return true;
}

void* pila_desapilar(Pila* pila)
{
    void* tope_quitar = NULL;
    void** buscar = &tope_quitar;
    if (!lista_quitar_elemento(pila->lista_enlazada, 0, buscar)) {
        return NULL;
    }
    return tope_quitar;
}

