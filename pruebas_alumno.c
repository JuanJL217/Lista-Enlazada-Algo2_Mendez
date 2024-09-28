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

int comparar_letras(void* a, void* b)
{
	return *(char*)a - *(char*)b;
}

bool sumar_elementos(void* a, void* total)
{
	*(int*)total += *(int*)a;
	return true;
}

bool buscar_el_multiplo(void* a, void* ctx)
{
	if (*(int*)a % 5 == 0){
		*(int*)ctx = *(int*)a;
		return false;
	}
	return true;
}

// ---------------- PRUEBAS LISTA ----------------

void inicializarUnaLista()
{
	Lista *lista = lista_crear();
	if (!lista) {
		return;
	}
	int contador = 0;
	int buscar = 10;
	pa2m_afirmar(lista_cantidad_elementos(lista) == 0,
		     "Crear una lista tiene 0 elementos, está vacía");
	pa2m_afirmar(lista_buscar_elemento(lista, (void *)&buscar,
					   comparar_numeros) == NULL,
		     "Lista vacía, buscar elemento retorna NULL");
	pa2m_afirmar(lista_iterar_elementos(lista, contar_numeros_iterando,
					    (void *)&contador) == 0,
		     "Lista vacia, iterar lista retorna 0");
	pa2m_afirmar(lista_quitar_elemento(lista, 0, NULL) == false,
		     "Lista vacia, quitar elemento retorna false");
	lista_destruir(lista);
}

void agregarElementosSimplesAlComienzo()
{
	Lista *lista = lista_crear();
	size_t numero[] = { 123, 456, 789, 90, 13};
	pa2m_afirmar(lista_agregar_elemento(lista, 1, &numero[0]) == false, "Agregar un elemento fuera del rango permitido (posicion mayor a la cantidad acual), agregar elemento retorna false");
	for (size_t i = 0; i < 5; i++) {
		if (!lista_agregar_elemento(lista, 0, &numero[i])) {
			lista_destruir(lista);
			return;
		}
	}
	pa2m_afirmar(lista_cantidad_elementos(lista) == 5, "Hay 5 elementos en la lista");

	size_t* elemento;
	for (size_t i = 0; i < 5; i++) {
		if (!lista_obtener_elemento(lista, i, (void**)&elemento)) {
			lista_destruir(lista);
			return;
		}
		pa2m_afirmar(elemento == &numero[4-i], "Obtener en posicion %li, dirección obtenido: '%p' -> esperado: '%p'", i, elemento, &numero[4-i]);
	}

	pa2m_afirmar(
		lista_obtener_elemento(lista, 0, NULL) == true,
		"Si busco algo en cualquier posicion y el parametro es NULL, retorna true solo que no obtengo la dirección");

	lista_destruir(lista);
}

void agregarElementosSimplesAlFinal()
{
	Lista *lista = lista_crear();
	double decimales[] = { 123.5, 456.7, 98.9, 18.3 };
	for (size_t i = 0; i < 4; i++) {
		if (!lista_agregar_al_final(lista, (void*)&decimales[i])) {
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
		pa2m_afirmar(elemento == &decimales[i],"Obtener en posicion %li, dirección obtenido: '%p' -> esperado: '%p'", i, elemento, &decimales[i] );
	}

	lista_destruir(lista);
}

void agregarElementosSimplesEnElMedio() // La prueba de NULL
{
	Lista *lista = lista_crear();
	int numeros[] = { -5, -3, 12, 434, 8, -95, 55, -898, 10, 42 };
	int* lista_esperada[] = {&numeros[5], &numeros[0], &numeros[7], &numeros[1], &numeros[6], &numeros[9], &numeros[2], &numeros[3], &numeros[4], &numeros[8]};

	for (size_t i = 0; i < 5; i++) {
		lista_agregar_al_final(lista, &numeros[i]);
	}
	lista_agregar_elemento(lista, 0, &numeros[5]);
	lista_agregar_elemento(lista, 3, &numeros[6]);
	lista_agregar_elemento(lista, 2, &numeros[7]);
	lista_agregar_al_final(lista, &numeros[8]);
	lista_agregar_elemento(lista, 5, &numeros[9]);
	pa2m_afirmar(lista_cantidad_elementos(lista) == 10, "Se agregaron 10 elementos");

	int *elemento;
	for (size_t i = 0; i < 10; i++) {
		if (!lista_obtener_elemento(lista, i, (void **)&elemento)) {
			lista_destruir(lista);
			return;
		}
		pa2m_afirmar(elemento == lista_esperada[i],
			     "Obtener en posicion %li, dirección obtenido: '%p' -> esperado: '%p'", i,
			     *elemento, *lista_esperada[i]);
	}
	lista_destruir(lista);
}

void agregarElemetosConMalloc()
{
	Lista *lista = lista_crear();
	char *nombres[] = { "Juan", "Ernesto", "Juarez", "Lezama" };
	char* direccioes_creadas_con_malloc[4];

	for (size_t i = 0; i < 4; i++) {
		char *nombre = malloc(strlen(nombres[i]) +
				      1 * sizeof(char *));
		if (!nombre) {
			lista_destruir_todo(lista, destruir_nombres);
			return;
		}
		strcpy(nombre, nombres[i]);
		direccioes_creadas_con_malloc[i] = nombre;
		lista_agregar_al_final(lista, nombre);
	}
	pa2m_afirmar(lista_cantidad_elementos(lista) == 4,
		     "La lista agregó 4 nombres, y almacenados en la lista con malloc");

	char *elemento;
	for (size_t i = 0; i < 4; i++) {
		if (!lista_obtener_elemento(lista, i, (void **)&elemento)) {
			lista_destruir_todo(lista, destruir_nombres);
			return;
		}
		pa2m_afirmar(elemento == direccioes_creadas_con_malloc[i],
			     "Obtener en posicion %li, dirección obtenido: '%p' -> esperado: '%p'", i,
			     elemento, direccioes_creadas_con_malloc[i]);
	}

	lista_destruir_todo(lista, destruir_nombres);
}

void buscarElementoFuncionComparacion()
{
	Lista *lista = lista_crear();
	int numeros[] = { 1, 2, 5, 10, -9, 0, -1, -24, 9, -5 };
	for (size_t i = 0; i < 10; i++) {
		lista_agregar_elemento(lista, 0, &numeros[i]);
	}
	pa2m_afirmar(lista_cantidad_elementos(lista) == 10,
		     "Hay 10 elementos en la lista");

	int buscar;
	int *encontrado;
	for (int i = 0; i < 10; i ++) {
		buscar = numeros[i];
		encontrado = (int *)lista_buscar_elemento(lista, (void *)&buscar, comparar_numeros);
		pa2m_afirmar(encontrado == &numeros[i], "Se busca el elemento '%d', direccion obtenido: '%p' -> esperado: %p", buscar, encontrado, &numeros[i]);
	}
			

	buscar = 100;
	encontrado = lista_buscar_elemento(lista, (void*)&buscar, comparar_numeros);
	pa2m_afirmar(encontrado == NULL, "Buscar un elemento que no está en la lista, la función retornará NULL");

	lista_destruir_todo(lista, NULL);

}

void eliminarElementos()
{
	Lista *lista = lista_crear();
	int numero = 5;
	lista_agregar_elemento(lista, 0, &numero);

	int *elemento_quitado = NULL;
	pa2m_afirmar(lista_cantidad_elementos(lista) == 1,
		     "Hay 1 elemento en la lista");
	pa2m_afirmar(lista_quitar_elemento(lista, 10,
					   (void **)&elemento_quitado) == false,
		     "Quitar un elemento por fuera del rango, retorna falso");
	pa2m_afirmar(lista_quitar_elemento(lista, 0,
					   (void **)&elemento_quitado) == true,
		     "Elemento eliminado de la lista");
	pa2m_afirmar(lista_cantidad_elementos(lista) == 0, "Lista vacia");
	free(lista);
}

void eliminarYComprobarBuscando()
{
	Lista* lista = lista_crear();
	char letras[] = { '8', 'b', 'c', '2', 'e', 'f', 'g', '8'};
	for (size_t i = 0; i < 8; i++) {
		if (!lista_agregar_elemento(lista, i, (void*)&letras[i])){
			lista_destruir(lista);
			return;
		}
	}
	pa2m_afirmar(lista_cantidad_elementos(lista) == 8, "Se agregaron 8 elementos diferentes en la lista");
	char* elemento_quitado;
	pa2m_afirmar(lista_quitar_elemento(lista, 0, (void**)&elemento_quitado) == true, "Se quitó el elemento en la posicion 0");
	pa2m_afirmar(elemento_quitado == &letras[0], "El elemento quitado por el inicio es '%c', direccion obtenida: '%p' -> esperado: '%p'", letras[0], elemento_quitado, &letras[0]);
	pa2m_afirmar(lista_buscar_elemento(lista, (void*)&letras[0], comparar_letras) == &letras[7], "Buscar el elemento eliminado sí lo encuentra, hay repetido");
	pa2m_afirmar(lista_cantidad_elementos(lista) == 7, "Hay 7 elementos en la lista");

	pa2m_afirmar(lista_quitar_elemento(lista, lista_cantidad_elementos(lista)-1, (void**)&elemento_quitado) == true, "Se quitó el elemento al final de la lista");
	pa2m_afirmar(elemento_quitado == &letras[7], "El elemento quitado por el final es '%c', direccion obtenida: '%p' -> esperado: '%p'", letras[7], elemento_quitado, &letras[7]);
	pa2m_afirmar(lista_buscar_elemento(lista, (void*)&letras[7], comparar_letras) == false, "Buscar el elemento eliminado no lo encuentra, era el elemento repetido");
	pa2m_afirmar(lista_cantidad_elementos(lista) == 6, "Hay 6 elementos en la lista");

	pa2m_afirmar(lista_quitar_elemento(lista, 2, NULL) == true, "Se quitó un elemento en el medio de la lista, auque se haya pasado NULL como parametro");
	pa2m_afirmar(lista_buscar_elemento(lista, (void*)&letras[3], comparar_letras) == false, "El elemento eliminado no se encuentra en la lista");
	pa2m_afirmar(lista_cantidad_elementos(lista) == 5, "Hay 5 elementos en la lista");

	char* elemento_obtenido;
	lista_obtener_elemento(lista, 0, (void**)&elemento_obtenido);
	pa2m_afirmar(elemento_obtenido == &letras[1], "Obtener en posicion 0, direccion obtenida: '%p', -> esperado: '%p'", elemento_obtenido, &letras[1]);
	lista_obtener_elemento(lista, 1, (void**)&elemento_obtenido);
	pa2m_afirmar(elemento_obtenido == &letras[2], "Obtener en posicion 1, direccion obtenida: '%p', -> esperado: '%p'", elemento_obtenido, &letras[2]);
	lista_obtener_elemento(lista, 2, (void**)&elemento_obtenido);
	pa2m_afirmar(elemento_obtenido == &letras[4], "Obtener en posicion 2, direccion obtenida: '%p', -> esperado: '%p'", elemento_obtenido, &letras[4]);
	lista_obtener_elemento(lista, 3, (void**)&elemento_obtenido);
	pa2m_afirmar(elemento_obtenido == &letras[5], "Obtener en posicion 3, direccion obtenida: '%p', -> esperado: '%p'", elemento_obtenido, &letras[5]);
	lista_obtener_elemento(lista, 4, (void**)&elemento_obtenido);
	pa2m_afirmar(elemento_obtenido == &letras[6], "Obtener en posicion 4, direccion obtenida: '%p', -> esperado: '%p'", elemento_obtenido, &letras[6]);

	lista_destruir_todo(lista, NULL);
}

void eliminarPorElFinal()
{
	Lista *lista = lista_crear();
	int numeros[] = { 1, -2, 5, -3, 8, 10, -4 };
	for (size_t i = 0; i < 7; i++) {
		if(!lista_agregar_al_final(lista, &numeros[i])) {
			lista_destruir(lista);
			return;
		}
	}

	pa2m_afirmar(lista_cantidad_elementos(lista) == 7,
		     "Se agregaron 7 elementos");

	int* elemento_quitado;
	for (size_t i = 0; i < 7; i ++) {
		lista_quitar_elemento(lista, lista_cantidad_elementos(lista)-1, (void**)&elemento_quitado);
		pa2m_afirmar(lista_cantidad_elementos(lista) == 6-i, "Se quitó un elemento por el final, ahora quedan %li", 6-i);
		pa2m_afirmar(lista_buscar_elemento(lista, &numeros[6-i], comparar_numeros) == NULL, "El elemento '%d' no se encuentra en la lista", numeros[6-i]);
	}
	pa2m_afirmar(lista_cantidad_elementos(lista) == 0, "La lista esta vacia");

	free(lista);
}

void eliminarPorElInicio()
{
	Lista *lista = lista_crear();
	int numeros[] = { 1, -2, 5, -3, 8, 10, -4 };
	for (size_t i = 0; i < 7; i++) {
		lista_agregar_al_final(lista, &numeros[i]);
	}

	pa2m_afirmar(lista_cantidad_elementos(lista) == 7,
		     "Se agregaron 7 elementos");

	int* elemento_quitado;
	for (size_t i = 0; i < 7; i ++) {
		lista_quitar_elemento(lista, 0, (void**)&elemento_quitado);
		pa2m_afirmar(lista_cantidad_elementos(lista) == 6-i, "Se quitó un elemento por el final, ahora quedan %li", 6-i);
		pa2m_afirmar(lista_buscar_elemento(lista, &numeros[i], comparar_numeros) == NULL, "El elemento '%d' no se encuentra en la lista", numeros[i]);
	}
	pa2m_afirmar(lista_cantidad_elementos(lista) == 0, "La lista esta vacia");

	free(lista);
}

void eliminarPorElMedio()
{
	return;
}

void iterarTodosLosElementos()
{
	Lista* lista = lista_crear();
	int numeros[] = {1, 2, 3, 4, 5, 6, 7, 8 ,9 ,10};
	for(size_t i = 0; i < 10; i ++) {
		if (!lista_agregar_al_final(lista, (void*)&numeros[i])) {
			lista_destruir_todo(lista, NULL);
			return;
		}
	}
	pa2m_afirmar(lista_cantidad_elementos(lista) == 10, "Hay 10 elementos en la lista");

	int suma_total = 0;
	size_t cantidad_iteraciones = lista_iterar_elementos(lista, sumar_elementos, &suma_total);
	pa2m_afirmar(cantidad_iteraciones == lista_cantidad_elementos(lista), "Si se itera todos los elementos, da como resultado la cantidad de elementos en la lista");
	pa2m_afirmar(suma_total == 55, "La suma de todos los numeros iterados es 55");
	lista_destruir(lista);
}

void verificarCorte()
{
	Lista* lista = lista_crear();
	int numeros[] = {12, 3, 4, 58, 28, 1, 49, 60, 29, 17, 5};
	for (size_t i = 0; i < 11; i ++) {
		if(!lista_agregar_al_final(lista, (void*)&numeros[i])) {
			lista_destruir(lista);
			return;
		}
	}

	int* elemento_obtenido;
	int multiplo_de_5 = 0;;
	size_t cantidad_iterados = lista_iterar_elementos(lista, buscar_el_multiplo, &multiplo_de_5);
	pa2m_afirmar(cantidad_iterados == 8, "Cortó en la posición 7 de la lista");
	lista_obtener_elemento(lista, cantidad_iterados-1, (void*)&elemento_obtenido);
	pa2m_afirmar(*elemento_obtenido == multiplo_de_5, "El elemento obtenido es el mismo que el elemento que verificó el cierre del iterador");
	lista_destruir(lista);
}

void listaNula()
{	
	void* lista = NULL;
	int multiplo_de_2;
	int buscado = 5;
	pa2m_afirmar(lista_buscar_elemento(lista, (void*)&buscado, comparar_numeros) == NULL, "Tener una lista nula, buscar elemento devuelve NULL");
	pa2m_afirmar(lista_obtener_elemento(lista, 0, NULL) == false,"Tener una lista nula, obtener elemento devolverá false");
	pa2m_afirmar(lista_cantidad_elementos(lista) == 0, "Lista nula devuelve 0 elementos");
	pa2m_afirmar(lista_quitar_elemento(lista, 0, NULL) == false, "Lista nula, quitar elemento dará false");
	pa2m_afirmar(lista_iterar_elementos(lista, buscar_el_multiplo, &multiplo_de_2) == 0, "Tener una lista nula, el iterador interno devolverá 0");
}

// ---------------- PRUEBAS ITERADOR EXTERNO ----------------



// ---------------- PRUEBAS PILA ----------------

void inicializarUnaPila()
{
	Pila *pila = pila_crear();
	pa2m_afirmar(pila_cantidad(pila) == 0,
		     "Crear una Pila tiene 0 elemento");
	pa2m_afirmar(pila_esta_vacía(pila) == true, "La Pila está vacia");
	pa2m_afirmar(pila_desapilar(pila) == NULL,
		     "Si no hay elementos, no puede desapilar nada");
	pa2m_afirmar(pila_tope(pila) == NULL,
		     "No hay elemento en la pila, ver el tope retorna NULL");
	pila_destruir(pila);
}

void apilarYDesapilarElementos()
{
	Pila *pila = pila_crear();
	if (!pila)
		return;
	int numeros[] = { 1, 5, 9, 20, 1, 58 };
	for (size_t i = 0; i < 6; i++) {
		if (!pila_apilar(pila, (void *)&numeros[i])) {
			pila_destruir(pila);
			return;
		}
	}
	pa2m_afirmar(pila_cantidad(pila) == 6,
		     "Se agregaron 6 elementos a la pila");
	pa2m_afirmar(pila_esta_vacía(pila) == false, "La pila no se encuentra vacia");

	int *puntero;
	for (int j = 5; j >= 0; j--) {
		puntero = (int*) pila_tope(pila);
		pa2m_afirmar(
			puntero == &numeros[j],
			"Ver el tope de la pila tiene la dirección '%p' y debe ser '%p'",
			puntero, &numeros[j]);
		puntero = (int*) pila_desapilar(pila);
		pa2m_afirmar(
			puntero == &numeros[j],
			"Elemento desapilado tiene la direccion '%p' y debe ser '%p'",
			puntero, &numeros[j]);
	}
	pa2m_afirmar(pila_tope(pila) == NULL, "No hay elementos, ver tope devuelve NULL");
	pa2m_afirmar(pila_esta_vacía(pila) == true, "Pila vacia");
	pila_destruir(pila);
}

void apilarDesapilarVariasVeces()
{
	Pila* pila = pila_crear();
	if (!pila)
		return;

	double decimales1[] = {1.23, 34.1, 45.5, 09.4, 9.2, 12.9};
	double decimales2[] = {4.6, 9.0, 3.6, 334.1, 111.11};

	for (size_t i = 0; i < 6; i++) {
		if(!pila_apilar(pila, (void*)&decimales1[i])) {
			pila_destruir(pila);
			return;
		}
	}
	pa2m_afirmar(pila_cantidad(pila) == 6, "Se agregaron 6 elementos a la Pila");

	double* puntero;
	for (size_t i = 0; i < 2; i++) {
		//No veo el tope, por el test anterior
		puntero = (double*) pila_desapilar(pila);
		pa2m_afirmar(puntero == &decimales1[5-i], "Elemento desapilado tiene la dirección '%p' y debe ser '%p'", puntero, &decimales1[5-i]);
	}	
	pa2m_afirmar(pila_cantidad(pila) == 4, "Hay 4 elementos en la Pila");

	for (size_t i = 0; i < 5; i++) {
		if(!pila_apilar(pila, (void*)&decimales2[i])) {
			pila_destruir(pila);
			return;
		}
	}

	pa2m_afirmar(pila_cantidad(pila) == 9, "Se agregaron 5 elementos más, ahora hay 9 elementos en la Pila");

	for(size_t i = 0; i < 5; i++) {
		puntero = (double*) pila_tope(pila);
		pa2m_afirmar(puntero == &decimales2[4-i], "Ver el tope de la pila tiene la dirección '%p' y debe ser '%p'", puntero, &decimales2[4-i]);
		puntero = (double*) pila_desapilar(pila);
		pa2m_afirmar(puntero == &decimales2[4-i], "Elemento desapilado tiene la dirección '%p' y debe ser '%p'", puntero, &decimales2[4-i]);
	}

	for (size_t i = 0; i < 4; i++) {
		puntero = (double*) pila_tope(pila);
		pa2m_afirmar(puntero == &decimales1[3-i], "Ver el tope de la pila tiene la dirección '%p' y debe ser '%p'", puntero, &decimales1[3-i]);
		puntero = (double*) pila_desapilar(pila);
		pa2m_afirmar(puntero == &decimales1[3-i], "Elemento desapilado tiene la dirección '%p' y debe ser '%p'", puntero, &decimales1[3-i]);	
	}

	pa2m_afirmar(pila_cantidad(pila) == 0, "Pila vacia");
	pila_destruir(pila);
}

void pilaNula()
{	
	char* nombre = "Juan";
	void* pila = NULL;
	pa2m_afirmar(pila_apilar(pila, (void*)&nombre) == false, "Tener una pila nula, apilar da false");
	pa2m_afirmar(pila_desapilar(pila) == NULL, "Tener una pila nula, desapilar retorna NULL");
	pa2m_afirmar(pila_cantidad(pila) == 0, "Tener una pila nula, la cantidad de elementos es 0");
	pa2m_afirmar(pila_esta_vacía(pila) == false, "Tener una pila nula, está vacio retorna false");
	pa2m_afirmar(pila_tope(pila) == NULL, "Tener una pila nula, ver el tope retorna NULL");
}

// ---------------- PRUEBAS COLA ----------------

void inicializandoUnaCola()
{
	Cola *cola = cola_crear();
	pa2m_afirmar(cola_cantidad(cola) == 0,
		     "Crer una cola tiene 0 elemento");
	pa2m_afirmar(cola_esta_vacía(cola) == true, "La cola está vacía");
	pa2m_afirmar(cola_desencolar(cola) == NULL,
		     "Desencolar una cola vacia devuelve NULL");
	pa2m_afirmar(cola_frente(cola) == NULL, "Al no tener elementos en la cola, ver el frente retorna NULL");
	cola_destruir(cola);
}

void encolarYDesencolarElementos()
{
	return;
}

void encolarDesencolarVariasVeces()
{
	return;
}

void colaNula()
{
	return;
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
	buscarElementoFuncionComparacion();
	pa2m_nuevo_grupo("Pruebas eliminar elementos de la lista");
	eliminarElementos();
	eliminarYComprobarBuscando();
	pa2m_nuevo_grupo("Pruebas vaciar lista eliminando por el final");
	eliminarPorElFinal();
	pa2m_nuevo_grupo("Pruebas vaciar lista eliminando al inicio");
	eliminarPorElInicio();
	pa2m_nuevo_grupo("Pruebas vaciar lista eliminando por el medio");
	eliminarPorElMedio();
	pa2m_nuevo_grupo("Pruebas iterador interno");
	iterarTodosLosElementos();
	verificarCorte();
	pa2m_nuevo_grupo("Prueba lista nula");
	listaNula();

	pa2m_nuevo_grupo("============= PREUBAS ITERADOR EXTERNO LISTA =============");

	pa2m_nuevo_grupo("================== PREUBAS PILA ==================");
	pa2m_nuevo_grupo("Prueba crear Pila");
	inicializarUnaPila();
	pa2m_nuevo_grupo("Prueba apilar y desapilar");
	apilarYDesapilarElementos();
	pa2m_nuevo_grupo("Pruebas apilando y desapilando en la misma Pila");
	apilarDesapilarVariasVeces();
	pa2m_nuevo_grupo("Prueba pila nula");
	pilaNula();

	pa2m_nuevo_grupo("================== PREUBAS COLA ==================");
	pa2m_nuevo_grupo("Prueba crear Cola");
	inicializandoUnaCola();
	pa2m_nuevo_grupo("Prueba encolar y desencolar");
	encolarYDesencolarElementos();
	pa2m_nuevo_grupo("Prueba encolando y desencolando en la misma cola");
	encolarDesencolarVariasVeces();
	pa2m_nuevo_grupo("Prueba cola nula");
	colaNula();

	return pa2m_mostrar_reporte();
}
