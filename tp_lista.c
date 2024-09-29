#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "src/lista.h"
#include "src/csv.h"

const size_t CAPACIDAD_INICIAL_TEXTO = 20;

typedef struct pokemon {
	char *nombre;
	char tipo;
	int fuerza;
	int destreza;
	int resistencia;
} Pokemon;

typedef struct texto {
	char *texto_almacenado;
	size_t tamaño_del_texto;
	size_t capacidad;
} Texto;

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

void destruir_pokemones(void *_pokemon)
{
	Pokemon *pokemon = (Pokemon*)_pokemon;
	free(pokemon->nombre);
	free(pokemon);
}

int comparar_nombre_pokemon(void *_p1, void *_p2)
{
	Pokemon *p1 = (Pokemon*)_p1;
	Pokemon *p2 = (Pokemon*)_p2;

	return strcmp(p1->nombre, p2->nombre);
}

Texto *texto_crear()
{
	Texto *texto = malloc(sizeof(Texto));
	if (!texto)
		return NULL;
	char *caracteres = malloc(CAPACIDAD_INICIAL_TEXTO * sizeof(char));
	if (!caracteres) {
		free(texto);
		return NULL;
	}
	texto->texto_almacenado = caracteres;
	texto->capacidad = CAPACIDAD_INICIAL_TEXTO;
	texto->tamaño_del_texto = 0;
	return texto;
}

bool texto_redimencionar(Texto *texto)
{
	texto->capacidad *= 2;
	char *nuevo_bloque = realloc(texto->texto_almacenado,
				     texto->capacidad * sizeof(char));
	if (!nuevo_bloque)
		return false;
	texto->texto_almacenado = nuevo_bloque;
	return true;
}

bool esta_llegando_al_tope_del_vector(Texto *texto, size_t capacidad,
				      size_t tamanño_texto)
{
	return tamanño_texto >= (capacidad * 75) / 100;
}

bool texto_agregar_caracter(Texto *texto, char caracter)
{
	if (esta_llegando_al_tope_del_vector(texto, texto->capacidad,
					     texto->tamaño_del_texto)) {
		if (!texto_redimencionar(texto)) {
			return false;
		}
	}
	texto->texto_almacenado[texto->tamaño_del_texto] = caracter;
	texto->tamaño_del_texto++;
	texto->texto_almacenado[texto->tamaño_del_texto] = '\0';
	return true;
}

void texto_destruir(Texto *texto)
{
	if (!texto)
		return;
	free(texto->texto_almacenado);
	free(texto);
}

void mostrar_pokemon_encontrado(Pokemon* pokemon_encontrado)
{
	printf("- Datos del pokemon %s:\n",
			pokemon_encontrado->nombre);
	printf("   Tipo: %c\n",
			pokemon_encontrado->tipo);
	printf("   Fuerza: %d\n",
			pokemon_encontrado->fuerza);
	printf("   Destreza: %d\n",
			pokemon_encontrado->destreza);
	printf("   Resistencia: %d\n",
			pokemon_encontrado->resistencia);
}

void mostrar_menu()
{
	printf("Elija una opcion:\n");
	printf("1) Buscar un Pokemon\n");
	printf("2) Mostrar todos los pokemones en la lista\n");
	printf("3) Salir\n");
	printf("\n");
	printf("Opcion: ");
}

void buscar_pokemon(Lista* lista_pokemones)
{
	printf("\n");
	Texto *nombre_pokemon_buscar = texto_crear();
	int caracter = getc(stdin);
	while (caracter != '\n' && caracter != EOF) {
		if (!texto_agregar_caracter(
				nombre_pokemon_buscar, (char)caracter)) {
			texto_destruir(nombre_pokemon_buscar);
			printf("Error al agregar el nombre");
		}
		caracter = getc(stdin);
	}

	Pokemon *pokemon_encontrado =
		(Pokemon *)lista_buscar_elemento(
			lista_pokemones,
			(void *)&nombre_pokemon_buscar
				->texto_almacenado,
			comparar_nombre_pokemon);
	if (pokemon_encontrado) {
		mostrar_pokemon_encontrado(pokemon_encontrado);
	} else {
		printf("Pokemon no encontrado\n");
	}
	texto_destruir(nombre_pokemon_buscar);
}

void mostrar_pokemones(Lista* lista_pokemones)
{
	printf("\n");
	size_t posicion = 1;
	Lista_iterador *iterador =
		lista_iterador_crear(lista_pokemones);
	while (lista_iterador_hay_siguiente(iterador)) {
		Pokemon *pokemon_actual = (Pokemon *)
			lista_iterador_obtener_elemento_actual(
				iterador);
		printf("%li) %s\n", posicion,
				pokemon_actual->nombre);
		posicion++;
		lista_iterador_avanzar(iterador);
	}
	lista_iterador_destruir(iterador);
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

	Lista *lista_pokemones = lista_crear();
	if (!lista_pokemones) {
		cerrar_archivo_csv(archivo_pokemones);
		perror("No se pudo inicializar la lista para los pokemones");
		return -3;
	}

	bool (*funciones[])(const char *,
			    void *) = { agregar_nombre, agregar_tipo,
					agregar_numero, agregar_numero,
					agregar_numero };

	Pokemon pokemon;
	pokemon.nombre = NULL;
	void *punteros[] = { &pokemon.nombre, &pokemon.tipo, &pokemon.fuerza,
			     &pokemon.destreza, &pokemon.resistencia };

	size_t lineas_leidas = 0;

	while (leer_linea_csv(archivo_pokemones, 5, funciones, punteros) == 5) {
		Pokemon *nueva_ubicacion_pokemon = malloc(sizeof(Pokemon));
		if (!nueva_ubicacion_pokemon) {
			return -4;
		}
		*nueva_ubicacion_pokemon = pokemon;
		if (!lista_agregar_al_final(lista_pokemones,
					    (void *)nueva_ubicacion_pokemon)) {
			fprintf(stderr,
				"Pokemon %s no se puede agregar correctamente a la lista\n",
				pokemon.nombre);
		}
		lineas_leidas++;
	}

	cerrar_archivo_csv(archivo_pokemones);

	if (lineas_leidas == 0) {
		lista_destruir_todo(lista_pokemones, destruir_pokemones);
		fprintf(stderr, "ARchivo vacio\n");
		return -4;
	}

	// Aqui inicia la lectura;

	bool seguir = true;

	while (seguir) {
		mostrar_menu();
		Texto *texto = texto_crear();
		if (!texto) {
			return -5;
		}
		int caracter = getc(stdin);
		while (caracter != '\n' && caracter != EOF) {
			if (!texto_agregar_caracter(texto, (char)caracter)) {
				texto_destruir(texto);
				lista_destruir_todo(lista_pokemones, destruir_pokemones);
				printf("Error al agregar caracteres");
				return -6;
			}
			caracter = getc(stdin);
		}

		if (strcmp(texto->texto_almacenado, "1") == 0) {
			buscar_pokemon(lista_pokemones);
		} else if (strcmp(texto->texto_almacenado, "2") == 0) {
			mostrar_pokemones(lista_pokemones);
		} else if (strcmp(texto->texto_almacenado, "3") == 0) {
			seguir = false;
		}
		printf("\n");
		texto_destruir(texto);
	}

	lista_destruir_todo(lista_pokemones, destruir_pokemones);
	return 0;
}
