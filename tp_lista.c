#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "src/lista.h"
#include "src/csv.h"

typedef struct pokemon {
	char *nombre;
	char tipo;
	int fuerza;
	int destreza;
	int resistencia;
} Pokemon;

bool agregar_nombre(const char *str, void *ctx)
{
	char *nuevo = malloc(strlen(str) + 1);
	if (nuevo == NULL) {
		fprintf(stderr,
			"Pokemon %s no pudo ser asignado a la pokedex, problema con asignar memoria\n",
			str);
		return false;
	}
	strcpy(nuevo, str);
	*(char **)ctx = nuevo;
	return true;
}

bool agregar_tipo(const char *str, void *ctx)
{
	*(char *)ctx = *(char *)str;
	return true;
}

bool agregar_numero(const char *str, void *ctx)
{
	return sscanf(str, "%d", (int *)ctx) == 1;
}

void destruir_pokemon(void* _pokemon)
{
	Pokemon* pokemon = _pokemon;
	free(pokemon->nombre);
	free(pokemon);
}

bool mostrar_pokemones(void* _pokemon, void* nada)
{
	Pokemon* pokemon = _pokemon;
	printf("%s\n", pokemon->nombre);
	return true;
}


int comparar_nombre_pokemon(void *_p1, void *_p2)
{
	Pokemon *p1 = _p1;
	Pokemon *p2 = _p2;

	return strcmp(p1->nombre, p2->nombre);
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		perror("No hay archivo para ejecutar\n");
		return -1;
	}

	struct archivo_csv *archivo_pokemones = abrir_archivo_csv(argv[1], ';');
	if (!archivo_pokemones) {
		perror("No se pudo inicializar el archivo\n");
		return -2;
	}

	Lista* lista_pokemones = lista_crear();
	if (!lista_pokemones) {
		cerrar_archivo_csv(archivo_pokemones);
		perror("No se pudo inicializar la lista para los pokemones");
		return -3;
	}

	bool (*funciones[])(const char*, void*) = {agregar_nombre, agregar_tipo, agregar_numero, agregar_numero, agregar_numero};
	
	Pokemon pokemon;
	pokemon.nombre = NULL;
	void *punteros[] = {&pokemon.nombre, &pokemon.tipo, &pokemon.fuerza, &pokemon.destreza, &pokemon.resistencia};

	size_t lineas_leidas = 0;

	while (leer_linea_csv(archivo_pokemones, 5, funciones, punteros) == 5) {
		Pokemon* nueva_ubicacion_pokemon = malloc(sizeof(Pokemon));
		if (!nueva_ubicacion_pokemon) {
			return -4;
		}
		*nueva_ubicacion_pokemon = pokemon;
		if(!lista_agregar_al_final(lista_pokemones, (void*)nueva_ubicacion_pokemon)) {
			fprintf(stderr, "Pokemon %s no se puede agregar correctamente a la lista\n", pokemon.nombre);
		}
		lineas_leidas++;
	}

	cerrar_archivo_csv(archivo_pokemones);

	if (lineas_leidas == 0) {
		lista_destruir_todo(lista_pokemones, destruir_pokemon);
		fprintf(stderr, "ARchivo vacio\n");
		return -4;
	}	

	lista_iterar_elementos(lista_pokemones, mostrar_pokemones, NULL);

	lista_destruir_todo(lista_pokemones, destruir_pokemon);
	return 0;
}
