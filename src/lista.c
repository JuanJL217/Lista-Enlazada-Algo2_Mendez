#include "lista.h"

const size_t UNA_POSICION_MENOS = 1;

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

bool lista_esta_vacia(Lista* lista)
{
    return lista->cantidad_elementos == 0;
}

bool insertar_al_inicio(size_t posicion)
{
    return posicion == 0;
}

nodo_lista* buscar_nodo(nodo_lista* nodo_actual, size_t posicion, size_t ajuste)
{
    for (size_t i = 0; i < posicion - ajuste; i++) {
        nodo_actual = nodo_actual->siguiente;
    }
    return nodo_actual;
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
    if (!lista || posicion > lista->cantidad_elementos) {
        return false;
    }

    if (lista_esta_vacia(lista) || posicion == lista->cantidad_elementos) {
        return (lista_agregar_al_final(lista, cosa))?true:false;
    }

    nodo_lista* nuevo_nodo = nodo_crear(cosa);
    if (!nuevo_nodo) {
        return false;
    }

    if (insertar_al_inicio(posicion)) {
        nuevo_nodo->siguiente = lista->primer_elemento;
        lista->primer_elemento = nuevo_nodo;

    } else {
        nodo_lista* nodo_encontrado = buscar_nodo(lista->primer_elemento, posicion, UNA_POSICION_MENOS);
        nuevo_nodo->siguiente = nodo_encontrado->siguiente;
        nodo_encontrado->siguiente = nuevo_nodo;
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

    if (lista_esta_vacia(lista)) {
        lista->primer_elemento = nuevo_nodo;
        lista->ultimo_elemento = nuevo_nodo;
    } else {
        lista->ultimo_elemento->siguiente = nuevo_nodo;
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
    if (lista_esta_vacia(lista)) {
        return false;
    }
    nodo_lista* nodo_encontrado = buscar_nodo(lista->primer_elemento, posicion, 0);
    if (elemento_quitado){
        *elemento_quitado = nodo_encontrado->elemento;
    }
    free(nodo_encontrado);
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
    nodo_lista* nodo_encontrado = buscar_nodo(lista->primer_elemento, posicon, 0);
    *elemento_encontrado = nodo_encontrado->elemento;

    return true;
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

void lista_destruir_todo(Lista* lista, void (*destructor)(void*))
{
    nodo_lista* nodo_actual = lista->primer_elemento;
    while (nodo_actual) {
        nodo_lista* nodo_siguiente = nodo_actual->siguiente;
        destructor(nodo_actual->elemento);
        free(nodo_actual);
        nodo_actual = nodo_siguiente;
    }
}

void lista_destruir(Lista* lista)
{
    nodo_lista* nodo_actual = lista->primer_elemento;
    while(nodo_actual) {
        nodo_lista* nodo_siguiente = nodo_actual->siguiente;
        free(nodo_actual);
        nodo_actual = nodo_siguiente;
    }
    free(lista);
}