#include "lista.h"

const size_t UNA_POSICION_ANTES = 1;
const size_t PRIMER_NODO = 0;

typedef struct nodo {
    void* elemento;
    struct nodo* siguiente;
} nodo_lista;

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

struct lista {
    struct nodo* primer_nodo;
    struct nodo* ultimo_nodo;
    size_t cantidad_elementos;
};

bool lista_esta_vacia(Lista* lista)
{
    return lista->cantidad_elementos == 0;
}

Lista* lista_crear()
{
    Lista* lista = malloc(sizeof(Lista));
    lista->primer_nodo = NULL;
    lista->ultimo_nodo = NULL;
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

    if (posicion == lista->cantidad_elementos) {
        return lista_agregar_al_final(lista, cosa);
    }

    nodo_lista* nuevo_nodo = nodo_crear(cosa);
    if (!nuevo_nodo) {
        return false;
    }

    if (insertar_al_inicio(posicion)) {
        nuevo_nodo->siguiente = lista->primer_nodo;
        lista->primer_nodo = nuevo_nodo;
    } else {
        nodo_lista* nodo_encontrado = buscar_nodo(lista->primer_nodo, posicion, UNA_POSICION_ANTES);
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
        lista->primer_nodo = nuevo_nodo;
    } else {
        lista->ultimo_nodo->siguiente = nuevo_nodo;
    }
    lista->ultimo_nodo = nuevo_nodo;
    lista->cantidad_elementos++;
    return true;
}

size_t posicion_ultimo_nodo(Lista* lista)
{
    return lista->cantidad_elementos;
}

bool lista_quitar_elemento(Lista* lista, size_t posicion, void** elemento_quitado)
{   
    if (!lista || posicion > lista->cantidad_elementos || lista_esta_vacia(lista)) {
        return false;
    }

    nodo_lista* nodo_encontrado;
    nodo_lista* nodo_anterior = NULL;

    if (posicion == PRIMER_NODO) {
        nodo_encontrado = lista->primer_nodo;
        lista->primer_nodo = lista->primer_nodo->siguiente;
    } else {
        nodo_anterior = buscar_nodo(lista->primer_nodo, posicion, UNA_POSICION_ANTES);
        } if (posicion == posicion_ultimo_nodo(lista)) {
            nodo_encontrado = lista->ultimo_nodo;
            lista->ultimo_nodo = nodo_anterior;
        } else {
            nodo_encontrado = nodo_anterior->siguiente;
            nodo_anterior->siguiente = nodo_encontrado->siguiente;
        }

    if (elemento_quitado){
        *elemento_quitado = nodo_encontrado->elemento;
    }

    free(nodo_encontrado);
    lista->cantidad_elementos--;
    return true;
}

void* lista_buscar_elemento(Lista* lista, void* buscado, int(*comparador)(void*, void*))
{   
    if (!lista || !comparador || lista_esta_vacia(lista)) {
        return NULL;
    }

    nodo_lista* nodo_actual = lista->primer_nodo;
    while(nodo_actual) {
        if (comparador(buscado, nodo_actual->elemento) == 0) {
            return nodo_actual->elemento;
        }
        nodo_actual = nodo_actual->siguiente;
    }

    return NULL;
}

bool lista_obtener_elemento(Lista* lista, size_t posicion, void** elemento_encontrado)
{      
    if (!lista || posicion > lista->cantidad_elementos || lista_esta_vacia(lista)) {
        return false;
    }
    nodo_lista* nodo_encontrado = buscar_nodo(lista->primer_nodo, posicion, 0);
    *elemento_encontrado = nodo_encontrado->elemento;

    return true;
}

size_t lista_iterar_elementos(Lista* lista, bool (*f)(void*, void*), void* ctx)
{
    if (!lista || !f || lista->cantidad_elementos == 0 || lista_esta_vacia(lista)) {
        return 0;
    }

    nodo_lista* nodo_actual = lista->primer_nodo;
    size_t contador_iteraciones = 0;

    while (nodo_actual) {
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
    if (!lista || !destructor) {
        return;
    }
    nodo_lista* nodo_actual = lista->primer_nodo;
    while (nodo_actual) {
        nodo_lista* nodo_siguiente = nodo_actual->siguiente;
        destructor(nodo_actual->elemento);
        free(nodo_actual);
        nodo_actual = nodo_siguiente;
    }
    free(lista);
}

void lista_destruir(Lista* lista)
{
    if (!lista) { return; }
    nodo_lista* nodo_actual = lista->primer_nodo;
    while(nodo_actual) {
        nodo_lista* nodo_siguiente = nodo_actual->siguiente;
        free(nodo_actual);
        nodo_actual = nodo_siguiente;
    }
    free(lista);
}

// ---------- Iterador Externo ------------

struct lista_iterador {
    Lista* lista;
    nodo_lista* nodo_actual;
};

Lista_iterador* lista_iterador_crear(Lista *lista)
{
    Lista_iterador* iterador = malloc(sizeof(Lista_iterador));
    if (!iterador) {
        return NULL;
    }
    iterador->lista = lista;
    iterador->nodo_actual = lista->primer_nodo;
    return iterador;
}

bool lista_iterador_hay_siguiente(Lista_iterador* iterador)
{   
    if (!iterador) {
        return false;
    }
    return iterador->nodo_actual != NULL;
}

void lista_iterador_avanzar(Lista_iterador* iterador)
{   
    if (!iterador) {
        return;
    }
    iterador->nodo_actual = iterador->nodo_actual->siguiente;
}

void* lista_iterador_obtener_elemento_actual(Lista_iterador* iterador)
{
    return iterador->nodo_actual->elemento;
}

void lista_iterador_destruir_todo(Lista_iterador* iterador)
{
    if (!iterador) {
        return;
    }
    free(iterador);
}