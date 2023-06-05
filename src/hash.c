#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "hash.h"

#define FACTOR_CARGA_MAXIMO 0.7
#define TAMANIO_MINIMO 3

struct hash
{
	size_t capacidad;
	size_t cantidad;
	void** vector;
};

typedef struct nodo
{
	char* clave;
	void* valor;
	struct nodo *siguiente;
}nodo_t;

size_t funcion_hash(const char *str)
{
    size_t hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + (unsigned long)c; // hash * 33 + c
    }
    return hash;
}

hash_t *hash_crear(size_t capacidad)
{
	if (capacidad < TAMANIO_MINIMO)
		capacidad = TAMANIO_MINIMO;

	hash_t *hash = malloc(sizeof(hash_t));

	if (!hash)
		return NULL;

	hash->capacidad = capacidad;
	hash->cantidad = 0;
	hash->vector = calloc(1, sizeof(nodo_t*)*capacidad);

	return hash;
}

nodo_t* crear_nodo(const char *clave, void *elemento)
{
	nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
	
	if (!nuevo_nodo)
		return NULL;

	size_t i = strlen(clave);
	char* nueva_clave = malloc(i+1);
	if (!nueva_clave)
		return NULL;
	strcpy(nueva_clave, clave);

	nuevo_nodo->clave = nueva_clave;
	nuevo_nodo->siguiente = NULL;
	nuevo_nodo->valor = elemento;

	return nuevo_nodo;
}

void insertar_par(hash_t* hash, nodo_t* nodo)
{
	size_t posicion = funcion_hash(nodo->clave) % hash->capacidad;

	if (hash->vector[posicion] == NULL){
		hash->vector[posicion] = nodo;
		return;
	}

	nodo_t* nodo_aux = hash->vector[posicion];

	while (nodo_aux->siguiente != NULL)
	{
		nodo_aux = nodo_aux->siguiente;
	}
	nodo_aux->siguiente = nodo;

	return;
}
  
hash_t* rehash(hash_t* hash)
{
	hash_t* hash_nuevo = hash_crear(hash->capacidad*2);
	
	if (!hash_nuevo)
		return NULL;

	size_t i = 0;
	nodo_t* nodo_aux;

	while (i < hash->capacidad)
	{
		nodo_aux = hash->vector[i];
		while (nodo_aux != NULL)
		{
			insertar_par(hash_nuevo, nodo_aux);
			nodo_aux = nodo_aux->siguiente;
		}
		i++;
	}

	return hash_nuevo;
}

hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{
	if (!hash || !clave)
		return NULL;

	float carga = (float)(hash->cantidad) / (float)(hash->capacidad);

	if (carga > FACTOR_CARGA_MAXIMO)
		rehash(hash);
		
	size_t posicion = funcion_hash(clave) % hash->capacidad;
	nodo_t* nodo_actual = hash->vector[posicion];

	if (nodo_actual == NULL) {
		hash->vector[posicion] = crear_nodo(clave, elemento);
		hash->cantidad++;
		if (anterior != NULL)
			*anterior = NULL;
		return hash;
	}

	nodo_t* nodo_anterior;

	while (nodo_actual != NULL)
	{
		if (strcmp(nodo_actual->clave, clave) == 0){
			if (anterior != NULL)
				*anterior = nodo_actual->valor;
			nodo_actual->valor = elemento;
			return hash;
		}
		nodo_anterior = nodo_actual;
		nodo_actual = nodo_actual->siguiente;
	}

	nodo_t* nuevo_nodo = crear_nodo(clave, elemento);

	if (!nuevo_nodo)
		return NULL;

	nodo_anterior->siguiente = nuevo_nodo;
	hash->cantidad++;
	return hash;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	void* elemento_aux = NULL;
	nodo_t* nodo_aux;
	size_t posicion = funcion_hash(clave) % hash->capacidad;

	nodo_aux = hash->vector[posicion];

	if (nodo_aux == NULL)
		return NULL;

	if (strcmp(nodo_aux->clave, clave) == 0){
		hash->vector[posicion] = nodo_aux->siguiente;
		elemento_aux = nodo_aux->valor;
		free(nodo_aux->clave);
		free(nodo_aux);
		hash->cantidad--;
		return elemento_aux;
	}

	nodo_t* nodo_a_eliminar;

	while (nodo_aux != NULL){
		if (strcmp(nodo_aux->siguiente->clave, clave) == 0){
			nodo_a_eliminar = nodo_aux->siguiente;
			nodo_aux->siguiente = nodo_aux->siguiente->siguiente;
			elemento_aux = nodo_a_eliminar->valor;
			free(nodo_a_eliminar->clave);
			free(nodo_a_eliminar);
			hash->cantidad--;
			return elemento_aux;
		}
		nodo_aux = nodo_aux->siguiente;
	}
	return elemento_aux;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;
	
	size_t posicion = funcion_hash(clave) % hash->capacidad;

	if (hash->vector[posicion] == NULL)
		return NULL;
	
	nodo_t* nodo_actual = hash->vector[posicion];

	while(nodo_actual != NULL){
		if (strcmp(nodo_actual->clave, clave) == 0)
			return nodo_actual->valor;
		nodo_actual = nodo_actual->siguiente;
	}
	return NULL;
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return false;

	size_t posicion = funcion_hash(clave) % hash->capacidad;
	nodo_t* nodo_actual = hash->vector[posicion];

	while (nodo_actual != NULL){
		if (strcmp(nodo_actual->clave, clave) == 0)
			return true;
		nodo_actual = nodo_actual->siguiente;
	}

	return false;
}

size_t hash_cantidad(hash_t *hash)
{
	if (!hash)
		return 0;
	return hash->cantidad;
}

void hash_destruir(hash_t *hash)
{
	if (!hash)
		return;
	hash_destruir_todo(hash, NULL);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash)
		return;

	size_t posicion = 0;
	nodo_t* aux;
	nodo_t* siguiente;

	while (posicion < hash->capacidad){
		aux = hash->vector[posicion];
		while (aux!= NULL){
			siguiente = aux->siguiente;
			if (destructor != NULL)
				destructor(aux->valor);
			free(aux->clave);
			free(aux);
			aux = siguiente;
		}
		free(aux);
		posicion++;
	}
	free(hash->vector);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{
	size_t recorridos = 0;
	if (!hash || !f)
		return recorridos;
	
	int posicion = 0;
	bool continuar = true;

	nodo_t* nodo_actual;
	
	while ((recorridos < hash->cantidad) && continuar){
		nodo_actual = hash->vector[posicion];
		while (continuar && nodo_actual != NULL)
		{
			continuar = f(nodo_actual->clave, nodo_actual->valor, aux);
			nodo_actual = nodo_actual->siguiente;
			recorridos++;
		}
		posicion++;
	}

	return recorridos;
}
