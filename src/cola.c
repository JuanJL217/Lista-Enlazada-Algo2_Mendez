#include "cola.h"
#include "lista.h"

#define INICIO 0

struct cola {
    Lista* elementos;
};

Cola* cola_crear()
{
    Cola* cola = malloc(sizeof(Cola));
    if (!cola) {
        return NULL;
    }
    Lista* lista = lista_crear();
    if (!lista) {
        free(cola);
        return NULL;
    }
    cola->elementos = lista;
    return cola;
}

void cola_destruir(Cola* cola)
{
    lista_destruir(cola->elementos);
}

void cola_destruir_todo(Cola* cola, void (*f)(void*))
{
    lista_destruir_todo(cola->elementos, f);
}

size_t cola_cantidad(Cola* cola)
{
    return lista_cantidad_elementos(cola->elementos);
}

void* cola_frente(Cola* cola)
{
    void* frente;
    return lista_obtener_elemento(cola->elementos, INICIO, &frente)?frente:NULL;
}

bool cola_encolar(Cola* cola, void* cosa)
{
    return lista_agregar_al_final(cola->elementos, cosa);
}

void* cola_desencolar(Cola* cola)
{   
    void* elemento_desencolado;
    return lista_obtener_elemento(cola->elementos, INICIO, &elemento_desencolado)?elemento_desencolado:NULL;
}

bool cola_esta_vacia(Cola* cola)
{
    return cola_cantidad(cola) == 0;
}