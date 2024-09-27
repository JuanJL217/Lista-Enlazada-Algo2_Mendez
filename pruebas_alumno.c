#include "pa2m.h"
#include "string.h"
#include "src/lista.h"
#include "src/pila.h"
#include "src/cola.h"

void destruir_nombres(void *nombre)
{
	free((char *)nombre);
}

int comparar_numeros(void *a, void *b)
{
	return *(int *)a - *(int *)b;
}

bool contar_numeros_iterando(void *a, void *ctx)
{
	*(int *)ctx = +*(int *)a;
	return true;
}

// ----- PRUEBAS LISTA -------

void inicializarUnaLista()
{
	Lista *lista = lista_crear();
	if (!lista) {
		return;
	}
	int contador = 0;
	int buscar = 10;
	pa2m_afirmar(lista_cantidad_elementos(lista) == 0,
		     "Crear una lista tiene 0 elementos");
	pa2m_afirmar(lista_buscar_elemento(lista, (void *)&buscar,
					   comparar_numeros) == NULL,
		     "Buscar en una lista vacia apunta a NULL");
	pa2m_afirmar(lista_iterar_elementos(lista, contar_numeros_iterando,
					    (void *)&contador) == 0,
		     "Lista vacia, no hay elementos para iterar, retorna 0");
	pa2m_afirmar(lista_quitar_elemento(lista, 0, NULL) == false,
		     "Lista vacia, no se puede quitar nada, retorna false");
	lista_destruir(lista);
}

void agregarElementosSimplesAlComienzo()
{
	Lista *lista = lista_crear();
	size_t numero[] = { 123, 456, 789 };
	size_t resultado_esperado[] = { 789, 456, 123 };
	pa2m_afirmar(
		lista_agregar_elemento(NULL, 0, &numero[0]) == false,
		"Pasar NULL como parametro en lugar de una lista, devuelve false");
	pa2m_afirmar(
		lista_agregar_elemento(lista, 1, &numero[0]) == false,
		"Agregar un elemento, estando vacia, por fuera de la cantidad 0, no se puede agregar");
	for (size_t i = 0; i < 3; i++) {
		if (!lista_agregar_elemento(lista, 0, &numero[i])) {
			lista_destruir(lista);
			return;
		}
	}
	pa2m_afirmar(lista_cantidad_elementos(lista) == 3,
		     "Hay 3 elementos en la lista");

	void *elemento;
	for (size_t i = 0; i < 3; i++) {
		if (!lista_obtener_elemento(lista, i, &elemento)) {
			lista_destruir(lista);
			return;
		}
		pa2m_afirmar(*(size_t *)elemento == resultado_esperado[i],
			     "Posicion %li, obtenido: %li -> esperado: %li", i,
			     *(size_t *)elemento, resultado_esperado[i]);
	}

	pa2m_afirmar(
		lista_obtener_elemento(lista, 0, NULL) == true,
		"Pasar NULL como parametro me da true obtener el elemento");

	lista_destruir(lista);
}

void agregarElementosSimplesAlFinal()
{
	Lista *lista = lista_crear();
	double decimales[] = { 123.5, 456.7, 98.9, 18.3 };
	for (size_t i = 0; i < 4; i++) {
		if (!lista_agregar_al_final(lista, &decimales[i])) {
			lista_destruir(lista);
			return;
		}
	}
	pa2m_afirmar(lista_cantidad_elementos(lista) == 4,
		     "Hay 4 elementos en la lista");

	double *elemento;
	for (size_t i = 0; i < 4; i++) {
		if (!lista_obtener_elemento(lista, i, (void **)&elemento)) {
			lista_destruir(lista);
			return;
		}
		pa2m_afirmar(*elemento == decimales[i],
			     "Posicion %li, obtenido: %.1f -> esperado: %.1f",
			     i, *elemento, decimales[i]);
	}

	lista_destruir(lista);
}

void agregarElementosSimplesEnElMedio() // La prueba de NULL
{
	Lista *lista = lista_crear();
	int numeros[] = { -5, -3, 12, 434, 8, -95, 55, -898, 10, 42 };
	int lista_esperada[] = {
		-95, -5, -898, -3, 55, 42, 12, 434, 8, 10,
	};
	for (size_t i = 0; i < 5; i++) {
		lista_agregar_al_final(lista, &numeros[i]);
	}
	lista_agregar_elemento(lista, 0, &numeros[5]);
	lista_agregar_elemento(lista, 3, &numeros[6]);
	lista_agregar_elemento(lista, 2, &numeros[7]);
	lista_agregar_al_final(lista, &numeros[8]);
	lista_agregar_elemento(lista, 5, &numeros[9]);
	pa2m_afirmar(lista_cantidad_elementos(lista) == 10,
		     "Se agregaron 10 elementos");

	int *elemento;
	for (size_t i = 0; i < 10; i++) {
		if (!lista_obtener_elemento(lista, i, (void **)&elemento)) {
			lista_destruir(lista);
			return;
		}
		pa2m_afirmar(*elemento == lista_esperada[i],
			     "Posicion %li, obtenido: %d -> esperado: %d", i,
			     *elemento, lista_esperada[i]);
	}
	lista_destruir(lista);
}

void agregarElemetosConMalloc()
{
	Lista *lista = lista_crear();
	char *nombres_esperados[] = { "Juan", "Ernesto", "Juarez", "Lezama" };

	for (size_t i = 0; i < 4; i++) {
		char *nombre = malloc(strlen(nombres_esperados[i]) +
				      1 * sizeof(char *));
		if (!nombre) {
			lista_destruir_todo(lista, destruir_nombres);
			return;
		}
		strcpy(nombre, nombres_esperados[i]);
		lista_agregar_al_final(lista, nombre);
	}
	pa2m_afirmar(lista_cantidad_elementos(lista) == 4,
		     "La lista agregó 3 nombres");

	char *elemento;
	for (size_t i = 0; i < 4; i++) {
		if (!lista_obtener_elemento(lista, i, (void **)&elemento)) {
			lista_destruir_todo(lista, destruir_nombres);
			return;
		}
		pa2m_afirmar(strcmp(elemento, nombres_esperados[i]) == 0,
			     "Posicion %li, obtenido: %s -> esperado: %s", i,
			     elemento, nombres_esperados[i]);
	}

	lista_destruir_todo(lista, destruir_nombres);
}

void buscarElemento()
{
	Lista *lista = lista_crear();
	int numeros[] = { 1, 2, 5, 10, -9, 0, -1, -24, 9, -5 };
	for (size_t i = 0; i < 10; i++) {
		lista_agregar_elemento(lista, 0, &numeros[i]);
	}
	pa2m_afirmar(lista_cantidad_elementos(lista) == 10,
		     "Hay 10 elementos en la lista");

	int buscar = 5;
	int *encontrado = (int *)lista_buscar_elemento(lista, (void *)&buscar,
						       comparar_numeros);
	pa2m_afirmar(
		buscar == *encontrado,
		"Elemento %buscado se encuentra en la lista, nos da el puntero del elemento almacenado en la lista");

	buscar = 50;
	encontrado = (int *)lista_buscar_elemento(lista, (void *)&buscar,
						  comparar_numeros);
	pa2m_afirmar(encontrado == NULL,
		     "Si el elemento no se cuentra en la lista, devuelve NULL");

	buscar = -5;
	encontrado = (int *)lista_buscar_elemento(lista, (void *)&buscar,
						  comparar_numeros);
	pa2m_afirmar(
		buscar == *encontrado,
		"Si el elemento encontrado está en la última posicion, devuelve la direccion del elemento último");
	lista_destruir(lista);
}

void eliminarElementos()
{
	Lista *lista = lista_crear();
	int numero = 5;
	lista_agregar_elemento(lista, 0, &numero);

	int *elemento_quitado = NULL;

	pa2m_afirmar(lista_quitar_elemento(lista, 10,
					   (void **)&elemento_quitado) == false,
		     "Quitar un elemento por fuera del rango, es falso");
	pa2m_afirmar(lista_cantidad_elementos(lista) == 1,
		     "Hay 1 elemento en la lista");
	pa2m_afirmar(lista_quitar_elemento(lista, 0,
					   (void **)&elemento_quitado) == true,
		     "Número %d eliminado de la lista", elemento_quitado);
	pa2m_afirmar(lista_cantidad_elementos(lista) == 0, "Lista vacia");
	free(lista);
}

void eliminarPorElFinal()
{
	Lista *lista = lista_crear();
	int numeros[] = { 1, 2, 5, 2, 8, 9, 4 };
	for (size_t i = 0; i < 7; i++) {
		lista_agregar_al_final(lista, &numeros[i]);
	}

	pa2m_afirmar(lista_cantidad_elementos(lista) == 7,
		     "Se agregaron 7 elementos");

	lista_destruir(lista);
}

// ----- PRUEBAS PILA -------

void inicializarUnaPila()
{
	Pila *pila = pila_crear();
	pa2m_afirmar(pila_cantidad(pila) == 0,
		     "Inicializar una Pila tiene 0 elemento");
	pa2m_afirmar(pila_esta_vacía(pila) == true, "Pila vacia");
	pa2m_afirmar(pila_desapilar(pila) == NULL,
		     "Si no hay elementos, no puede sapilar nada");
	pila_destruir(pila);
}

// ----- PRUEBAS COLA -------

void inicializandoUnaCola()
{
	Cola *cola = cola_crear();
	pa2m_afirmar(cola_esta_vacía(cola) == true, "Cola vacia");
	pa2m_afirmar(cola_cantidad(cola) == 0,
		     "Crer una cola tiene 0 elemento");
	pa2m_afirmar(cola_desencolar(cola) == NULL,
		     "Desencolar una cola vacia devuelve NULL");
	cola_destruir(cola);
}

int main()
{
	pa2m_nuevo_grupo("================== PREUBAS LISTA ==================");
	pa2m_nuevo_grupo("Prueba crear lista");
	inicializarUnaLista();

	pa2m_nuevo_grupo(
		"Prueba agregar elementos simples al comienzo y comprobar lugares");
	agregarElementosSimplesAlComienzo();

	pa2m_nuevo_grupo(
		"Prueba agregar elementos simples al final y comprobar lugares");
	agregarElementosSimplesAlFinal();

	pa2m_nuevo_grupo(
		"Prueba agregar en distintos lugares y comprobar lugares");
	agregarElementosSimplesEnElMedio();

	pa2m_nuevo_grupo(
		"Agregando elementos usando malloc y la funcion destruir todo");
	agregarElemetosConMalloc();

	pa2m_nuevo_grupo("Prueba se buscan elementos en la lista comparando");
	buscarElemento();

	pa2m_nuevo_grupo("Pruebas eliminar elementos de la lista");
	eliminarElementos();

	pa2m_nuevo_grupo("Pruebas eliminando por el final");
	eliminarPorElFinal();

	pa2m_nuevo_grupo("============= PREUBAS ITERADOR LISTA =============");

	pa2m_nuevo_grupo("================== PREUBAS PILA ==================");
	pa2m_nuevo_grupo("Prueba crea Pila");
	inicializarUnaPila();

	pa2m_nuevo_grupo("================== PREUBAS COLA ==================");
	inicializandoUnaCola();

	return pa2m_mostrar_reporte();
}
