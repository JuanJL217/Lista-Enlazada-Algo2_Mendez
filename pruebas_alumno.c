#include "pa2m.h"
#include "src/lista.h"

void crearLista()
{
	Lista* lista = lista_crear();
	if(!lista) {
		return;
	}
	pa2m_afirmar(lista_cantidad_elementos(lista) == 0, "Crear una lista tiene 0 elementos");
	lista_destruir(lista);
}

void agregarElementosSimplesAlComienzo()
{
	Lista* lista = lista_crear();
	size_t numero[] = {123, 456, 789};
	size_t resultado_esperado[] = {789, 456, 123};
	pa2m_afirmar(lista_agregar_elemento(NULL, 0, &numero[0]) == false, "Pasar NULL como parametro en lugar de una lista, devuelve false");
	pa2m_afirmar(lista_agregar_elemento(lista, 1, &numero[0]) == false, "Agregar un elemento, estando vacia, por fuera de la cantidad 0, no se puede agregar");
	for (size_t i = 0;  i < 3; i++) {
		if (!lista_agregar_elemento(lista, 0, &numero[i])) {
			lista_destruir(lista);
			return;
		}
	}
	pa2m_afirmar(lista_cantidad_elementos(lista) == 3, "Hay 3 elementos en la lista");

	void* elemento;
	for (size_t i = 0; i < 3; i++) {
		if (!lista_obtener_elemento(lista, i, &elemento)) {
			lista_destruir(lista);
			return;
		}
		pa2m_afirmar(*(size_t*)elemento == resultado_esperado[i], "Posicion %li: %li <-> %li", i, *(size_t*)elemento, resultado_esperado[i]);
	}

	lista_destruir(lista);
}

void agregarElementosSimplesAlFinal()
{
	Lista* lista = lista_crear();
	double decimales[] = {123.5, 456.7, 98.9, 18.3};
	for (size_t i = 0; i < 4; i++) {
		if (!lista_agregar_al_final(lista, &decimales[i])) {
			lista_destruir(lista);
			return;
		}
	}
	pa2m_afirmar(lista_cantidad_elementos(lista) == 4, "Hay 4 elementos en la lista");
	void *elemento;
	for (size_t i = 0; i < 4; i++) {
		if (!lista_obtener_elemento(lista, i, &elemento)) {
			lista_destruir(lista);
			return;
		}
		pa2m_afirmar(*(double*)elemento == decimales[i], "Posicion %li: %.1f <-> %.1f", i, *(double*)elemento, decimales[i]);
	}

	lista_destruir(lista);
}

void agregarElementosSimplesEnElMedio() // La prueba de NULL
{
	Lista* lista = lista_crear();
	int numeros[] = {-5, -3, 12, 434, 8, -95, 55, -898, 10, 42};
	int lista_esperada[] = {-95, -5, -898, -3, 55, 42, 12, 434, 8, 10, };
	for (size_t i = 0; i < 5; i++) {
		lista_agregar_elemento(lista, i, &numeros[i]);
	}
	lista_agregar_elemento(lista, 0, &numeros[5]);
	lista_agregar_elemento(lista, 3, &numeros[6]);
	lista_agregar_elemento(lista, 2, &numeros[7]);
	lista_agregar_al_final(lista, &numeros[8]);
	lista_agregar_elemento(lista, 5, &numeros[9]);
	pa2m_afirmar(lista_cantidad_elementos(lista) == 10, "Se agregaron 10 elementos");
	int* elemento;
	for (size_t i = 0; i < 10; i ++) {
		if (!lista_obtener_elemento(lista, i, (void**)&elemento)) {
			lista_destruir(lista);
			return;
		}
		pa2m_afirmar(*elemento == lista_esperada[i], "Posicion %li: %d <-> %d", i, *elemento, lista_esperada[i]);
	}
	lista_destruir(lista);
}

void agregarElementoConDestructorTodo()
{	
	Lista* lista = lista_crear();
	char* nombre1 = "Juan";
	char* nombre2 = "Brenda";
	char* nombre3 = "Apa";

	lista_agregar_al_final(lista, nombre1);
	lista_agregar_elemento(lista, 0, nombre2);
	lista_agregar_al_final(lista, nombre3);

	pa2m_afirmar(lista_cantidad_elementos(lista) == 3, "La lista agregó 3 nombres");
	
	lista_destruir(lista);
}

int main()
{
	pa2m_nuevo_grupo("Prueba crear lista");
	crearLista();
	
	pa2m_nuevo_grupo("Prueba agregar elementos simples al comienzo y comprobar lugares");
	agregarElementosSimplesAlComienzo();

	pa2m_nuevo_grupo("Prueba agregar elementos simples al final y comprobar lugares");
	agregarElementosSimplesAlFinal();

	pa2m_nuevo_grupo("Prueba agregar en distintos lugares y comprobar lugares");
	agregarElementosSimplesEnElMedio();

	pa2m_nuevo_grupo("Agregando estructuras");
	agregarElementoConDestructorTodo();

	return pa2m_mostrar_reporte();
}
