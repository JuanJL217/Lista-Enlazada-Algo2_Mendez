#include "lista.h"

typedef struct nodo {
    void* elemento;
    struct nodo* siguiente;
} nodo_lista;

struct lista {
    struct nodo* primer_elemento;
    struct nodo* ultimo_elemento;
    size_t cantidad_elementos;
};

nodo_lista* nodo_crear(void* cosa)
{
    nodo_lista* nodo = malloc(sizeof(nodo_lista));
    if (!nodo) {
        return NULL;
    }
    nodo->elemento = cosa;
    nodo->siguiente = NULL;
    return nodo;
}

// --------- FUNCIONES PRINCIPALES -------

Lista* lista_crear()
{
    Lista* lista = malloc(sizeof(Lista));
    lista->primer_elemento = NULL;
    lista->ultimo_elemento = NULL;
    lista->cantidad_elementos = 0;
    return lista;
}

size_t lista_cantidad_elementos(Lista* lista)
{
    return lista->cantidad_elementos;
}

bool lista_agregar_elemento(Lista* lista, size_t posicion, void* cosa)
{
    if (!lista || !cosa || posicion > lista->cantidad_elementos) {
        return false;
    }

    nodo_lista* nuevo_nodo = nodo_crear(cosa);
    if (!nuevo_nodo) {
        return false;
    }

    if (lista->cantidad_elementos == 0) {
        lista->primer_elemento = nuevo_nodo;
        lista->ultimo_elemento = nuevo_nodo;
    } else {
        if (posicion == 0) {
            nuevo_nodo->siguiente = lista->primer_elemento;
            lista->primer_elemento = nuevo_nodo;
        } else {
            nodo_lista* nodo_actual = lista->primer_elemento;
            for (size_t i = 0; i < posicion - 1; i++) {
                nodo_actual = nodo_actual->siguiente;
            }
            nuevo_nodo->siguiente = nodo_actual->siguiente;
            nodo_actual->siguiente = nuevo_nodo;
        }
    }
    lista->cantidad_elementos++;
    return true;
}

bool lista_agregar_al_final(Lista* lista, void* cosa)
{   
    nodo_lista* nuevo_nodo = nodo_crear(cosa);
    if (!nuevo_nodo) {
        return false;
    }

    if (lista->cantidad_elementos == 0) {
        lista->primer_elemento = nuevo_nodo;
        lista->ultimo_elemento = nuevo_nodo;
    } else {
        lista->ultimo_elemento = nuevo_nodo;
    }

    lista->cantidad_elementos++;
    return true;
}

bool lista_quitar_elemento(Lista* lista, size_t posicion, void** elemento_quitado)
{   
    if (!lista || posicion > lista->cantidad_elementos) {
        return false;
    }

    if (lista->cantidad_elementos == 0) {
        return false;
    }

    nodo_lista* nodo_actual = lista->primer_elemento;
    for(size_t i = 0; i < posicion; i++) {
        nodo_actual = nodo_actual->siguiente;
    }
    if (elemento_quitado){
        *elemento_quitado = nodo_actual->elemento;
    }
    free(nodo_actual->elemento);
    free(nodo_actual);
    lista->cantidad_elementos--;
    return true;
}

void* lista_buscar_elemento(Lista* lista, void* buscado, int(*comparador)(void*, void*))
{   
    if (!lista || !buscado) {
        return NULL;
    }

    nodo_lista* nodo_actual = lista->primer_elemento;
    while(nodo_actual) {
        if (comparador(buscado, nodo_actual) == 0) {
            return nodo_actual->elemento;
        } 
    }

    return NULL;
}

bool lista_obtener_elemento(Lista* lista, size_t posicon, void** elemento_encontrado)
{      
    if (!lista || !elemento_encontrado || posicon > lista->cantidad_elementos) {
        return false;
    }
    nodo_lista* nodo_actual = lista->primer_elemento;
    for (size_t i = 0; i < posicon; i++) {
        if (nodo_actual->elemento == *elemento_encontrado) {
            return true;
        }
        nodo_actual = nodo_actual->siguiente;
    }
    return false;
}

size_t lista_iterar_elementos(Lista* lista, bool (*f)(void*, void*), void* ctx)
{
    if (!lista || !f || lista->cantidad_elementos == 0) {
        return 0;
    }

    nodo_lista* nodo_actual = lista->primer_elemento;
    size_t contador_iteraciones = 0;

    while (!nodo_actual) {
        void* elemento_actual = nodo_actual->elemento;
        if (!f(elemento_actual, ctx)) {
            return contador_iteraciones + 1;
        }
        nodo_actual = nodo_actual->siguiente;
        contador_iteraciones++;
    }

    return lista->cantidad_elementos;
}
