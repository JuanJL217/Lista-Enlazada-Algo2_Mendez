#include "pila.h"
#include "lista.h"

#define INICIO 0

struct pila {
	Lista *elementos;
};

Pila *pila_crear()
{
	Pila *pila = malloc(sizeof(Pila));
	if (!pila) {
		return NULL;
	}
	Lista *lista = lista_crear();
	if (!lista) {
		free(pila);
		return NULL;
	}
	pila->elementos = lista;
	return pila;
}

void pila_destruir(Pila *pila)
{
	lista_destruir(pila->elementos);
}

void pila_destruir_todo(Pila *pila, void (*f)(void *))
{
	lista_destruir_todo(pila->elementos, f);
}

size_t pila_cantidad(Pila *pila)
{
	return lista_cantidad_elementos(pila->elementos);
}

void *pila_tope(Pila *pila)
{
	void *tope;
	return lista_obtener_elemento(pila->elementos, INICIO, &tope) ? tope :
									NULL;
}

bool pila_apilar(Pila *pila, void *cosa)
{
	return lista_agregar_elemento(pila->elementos, INICIO, cosa);
}

void *pila_desapilar(Pila *pila)
{
	void *elemento_desapilado;
	return lista_quitar_elemento(pila->elementos, INICIO,
				     &elemento_desapilado) ?
		       elemento_desapilado :
		       NULL;
}

bool pila_esta_vacia(Pila *pila)
{
	return pila_cantidad(pila) == 0;
}