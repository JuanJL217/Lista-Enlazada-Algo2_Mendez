<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TDA LISTA

# ESTO ES UN TEMPLATE, COMPLETAR LO QUE HAGA FALTA, ELIMINAR LO QUE SOBRE (INCLUIDA ESTA LÍNEA)

## Alumno: Juan Ernesto Juarez Lezama - 110418 - jjuarez@fi.uba.ar

- Para compilar:

```bash
línea de compilación
```

- Para ejecutar:

```bash
línea de ejecución
```

- Para ejecutar con valgrind:
```bash
línea con valgrind
```

---

##  Funcionamiento

Explicación de cómo funcionan las estructuras desarrolladas en el TP y el funcionamiento general del mismo.

Aclarar en esta parte todas las decisiones que se tomaron al realizar el TP, cosas que no se aclaren en el enunciado, fragmentos de código que necesiten explicación extra, etc.

Incluír **EN TODOS LOS TPS** los diagramas relevantes al problema (mayormente diagramas de memoria para explicar las estructuras, pero se pueden utilizar otros diagramas si es necesario).

### Por ejemplo:

El programa funciona abriendo el archivo pasado como parámetro y leyendolo línea por línea. Por cada línea crea un registro e intenta agregarlo al vector. La función de lectura intenta leer todo el archivo o hasta encontrar el primer error. Devuelve un vector con todos los registros creados.

<div align="center">
<img width="70%" src="img/diagrama1.svg">
</div>

En el archivo `sarasa.c` la función `funcion1` utiliza `realloc` para agrandar la zona de memoria utilizada para conquistar el mundo. El resultado de `realloc` lo guardo en una variable auxiliar para no perder el puntero original en caso de error:

```c
int *vector = realloc(vector_original, (n+1)*sizeof(int));

if(vector == NULL)
    return -1;
vector_original = vector;
```


<div align="center">
<img width="70%" src="img/diagrama2.svg">
</div>

---

## Respuestas a las preguntas teóricas

## ¿Qué es una lista/pila/cola? Explicar con diagramas.

Tanto la lista, pila y cola son Estructuras que almacenan datos, que tienen usos especificos, excepto por la lista, que puede funcionar como pila y cola, pero estas no pueden funcionar como lista.

### 1) Pila:
La Pila tiene la capacidad de "recordar" lo último que ingresaste, por ejemplo: Vamos ingresando los números `5, 2, 8, 10, 14, 6`. Lo que hace una Pila es que a medida que vayas ingresando elemento, el primero que ingresaste se va para el fondo de la Pila y siempre tendrás a disposición el último que agregaste, en nuestro ejemplo, cuando decidimos sacar un elemento, será el número 6, porque fue el último en ingresar a la Pila, luego tendrémos el 14, y así sucesivamente hasta obtener el último elemento, que seríá el número 5, que fue el primero que ingresamos a la Pila, entonces:`El último elemento en salir es el primer elemento que agregaste`.

"ADJUNTAR IMAGEN"

### 2) Cola:
A diferencia de la Pila, la manera de almacenar los datos es distinto, porque ya no sacas el último elemento que agregaste, ahora, `El primer elemento en salir es el primer que agregaste`. Con el mismo ejemplo , al agregar los números `5, 2, 8, 10, 14, 6`, cuando quieras dejar vacia la Cola, se quita primero el número 5, luego el 2 y así hasta llegar al último elemento que agregaste, siendo nuestro ejemplo, el 6.

"ADJUNTAR IMAGEN"

### 3) Lista:
LA diferencia entre lo que es una lista con lo que es una Pila o una Cola, es la manera en como accedemos a los elementos. Como vimos, la manera de acceder a un elemento en estas 2 estructuras, depende de qué estructra estemos usando, si es una Pila, accedemos al elemento siendo este el último que agregamos; una Cola, al primer elemento. La manera en como una lista accede a sus elementos, es que podemos acceder a cualquier posicion, ya sea al inicio, al final o en medio. Y también una lista puede actuar como Lista y como Pila (dependdiente de cómo agregas y como sacas elementos).

"ADJUNTAR IMAGEN"

 ## Explica y analiza las diferencias de complejidad entre las implementaciones de lista simplemente enlazada, doblemente enlazada y vector dinámico para las operaciones (mencionar que para mis complejidades, sabré la cantidad de elementos en las listas).

 1) lista simplemente enlazada:
Una lista simplemente enlazada, en su estructura interna de nodos, habrá solo una dirección para recorrer la lista. El caso general es cuando el direccionamiento de los nodos va de inicio a final.

Insertar/obtener/eliminar al inicio:
Insertar un elemento al inicio: cuando es la inicio, siempre será O(1), ya que es instantaneo, no hay que recorrer ningún elemento, siendo esta lógica, obtener y eliminar también serán O(1), (siendo que al eliminar el inicio, su nuevo inicio será el siguiente, el cual ya apunta).

Insertar/obtener/eliminar al final:
Insertar un elemento al final, dependiendo como sea la estructura de la lista, puede ser O(1) si es que tenemos un puntero que apunte al final de la lista, caso contrario O(n), porque recorrerá toda la lista, para poder encontrar el final. Obtener elemento seguirá la misma lógica que insertar al final, si ya sé donde acaba, es O(1), si no, O(n). En el caso de eliminar elemento, en cualquiera de las 2 opciones, siempre será O(n), porque: Si sabemos donde acaba el final, lo eliminamos, pero debemos saber cuál es el elemento anterior al final, y como no sabemos cuál es (ya que los nodos solo apuntan en una dirección), debemos iterar hasta encontrar el penúltimo nodo (debemos iterar hasta encontrar ese nodo conociendo la cantidad de elementos) antes de eliminar el último y lo mismo sería si no sabemos el puntero al final, ya que, como no nos importa que haya un final (puntero), entonces es ir recorriendo la lista, llegar al final y eliminarlo.

Insertar/obtener/eliminar al medio:
Insertar al medio: Con cualquiera de las 2 posibles estructuras de una lista, conociendo el puntero al final d ela lista, la manera de agregar, buscar y eliminar un elemento, será O(n), porque vamos a tener que recorrer la lista.

2) Lista doblemente enlazada:
En esta implementación, cada nodo tendrá doble direccionamiento, tanto al siguiente nodo como al nodo anterior.

Insertar/obtener/eliminar al inicio: Siguiendo la mismoa lógica de la estructura anterior todo eso es O(1), ya que no hay que ir a buscar nada.

Insertar/obtener/eliminar al final:
Aquí sí cambia con el anterior, primero mencionar que insertar sigue siendo O(1) al igual que obtener, pero eliminar, ya no es O(n), porque al ser doblemente enlazada, tenemos direccion al nodo anteior, entonces no tenemos que iterar.

Insertar/obtener/eliminar al medio:
Insertar, buscar y eliminar al medio, siguen siendo O(n), ya que debemos iterar.

3) Vector dinámico:
Al ser un vector, tiene memoria contigua, sabremos las posiciones, aparte de saber la cantidad de elementos, en esta estructura, deseo decir que, aparte de conocer la cantidad de elementos, tambien deseo saber donde inicia mi vector.

Insertar/obtener/eliminar al inicio:
Insertar ya no es O(1) como en la simplementa o doblemente enlazada, esta vez, el primero elemento debe ser el primer bloque del vector, entonces para posicionar el inicio, debo mandar a la derecha y seder el espacio para el nuevo elemento, tanto eliminar y obtener serían O(1), ya que solo me basta con tener una lógica interna que, cuando borre el el primero, el usuario ingrese la posicion [0], seria darle lo que está en la posicion [1] (lo cual es O(1)), ignora la posicion [0], ya que, aunque está el elemento en el vector, el usuario ya lo eliminó.

Insertar/obtener/eliminar al final:
Insertar al final es O(1), ya que sabemos la cantidad de elementos, al igual que obtener es O(1). Eliminar también es O(1), porque no le damos acceso a ese elemento, aunque esté en el vector, insertar es O(1), ya que reemplazamos aquellos que borramos, y siempre que agregamos, no tenemos que mover nada a ningún lado, no como insertar al inicio, ya que, si no habia lugar a "izquierda" (en direccion al inicio), se demen mover los elementos.

Insertar/obtener/eliminar al medio:
Aquí es distinto, insertar en el medio, porque, aunque sea directo darle la posicion donde quiero agegar, debo mover a la derecha para darle espacio a mi nuevo elemento. Obtener seria O(1), y eliminar sería O(n), ya que debo hacer que el lado derecho, vaya para la izquierda y "rellenar" ese bloque.

 ## Explica la complejidad de las operaciones implementadas en tu trabajo para la pila y la cola.