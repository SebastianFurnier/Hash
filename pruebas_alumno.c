#include <stdlib.h>
#include "pa2m.h"
#include "src/hash.h"

bool modificar_vector(const char *clave, void *valor, void *aux)
{
	int *valor_aux = (int *)valor;
	(*valor_aux) = (*valor_aux) + *(int *)aux;
	return true;
}

void prueba_iterador_interno()
{
	size_t capacidad = 10;
	hash_t *nuevo_hash = hash_crear(capacidad);

	char *clave = "prueba";
	int elemento = 1;
	char *clave_dos = "prueba1";
	int elemento_dos = 2;
	char *clave_tres = "prueba2";
	int elemento_tres = 3;
	char *clave_cuatro = "prueba3";
	int elemento_cuatro = 4;
	char *clave_cinco = "prueba4";
	int elemento_cinco = 5;

	hash_insertar(nuevo_hash, clave, &elemento, NULL);
	hash_insertar(nuevo_hash, clave_dos, &elemento_dos, NULL);
	hash_insertar(nuevo_hash, clave_tres, &elemento_tres, NULL);
	hash_insertar(nuevo_hash, clave_cuatro, &elemento_cuatro, NULL);
	hash_insertar(nuevo_hash, clave_cinco, &elemento_cinco, NULL);

	size_t elementos_recorridos =
		hash_con_cada_clave(nuevo_hash, NULL, NULL);
	size_t valor_aux = 10;

	pa2m_afirmar(elementos_recorridos == 0,
		     "Recorrer la funcion con una funcion nula devuelve cero.");
	elementos_recorridos =
		hash_con_cada_clave(NULL, modificar_vector, NULL);
	pa2m_afirmar(
		elementos_recorridos == 0,
		"Recorrer un hash nulo con una funcion valida devuelve cero.");
	elementos_recorridos =
		hash_con_cada_clave(nuevo_hash, modificar_vector, &valor_aux);
	pa2m_afirmar(
		elementos_recorridos == 5,
		"Recorrer un hash valido con una funcion valida, recorre todo los elementos.");
	pa2m_afirmar(elemento == 11 && elemento_dos == 12 &&
			     elemento_tres == 13 && elemento_cuatro == 14 &&
			     elemento_cinco == 15,
		     "La funcion se aplica correctamente a cada elemento.");

	hash_destruir(nuevo_hash);
}

void pruebas_rehash()
{
	size_t capacidad = 4;
	hash_t *nuevo_hash = hash_crear(capacidad);

	char *clave = "prueba";
	int elemento = 1;
	char *clave_dos = "prueba1";
	int elemento_dos = 2;
	char *clave_tres = "prueba2";
	int elemento_tres = 3;
	char *clave_cuatro = "prueba3";
	int elemento_cuatro = 4;
	char *clave_cinco = "prueba4";
	int elemento_cinco = 5;

	hash_insertar(nuevo_hash, clave, &elemento, NULL);
	hash_insertar(nuevo_hash, clave_dos, &elemento_dos, NULL);
	hash_insertar(nuevo_hash, clave_tres, &elemento_tres, NULL);
	hash_insertar(nuevo_hash, clave_cuatro, &elemento_cuatro, NULL);
	hash_insertar(nuevo_hash, clave_cinco, &elemento_cinco, NULL);

	pa2m_afirmar(
		hash_cantidad(nuevo_hash) == 5,
		"Luego de rehashear la cantidad de elemento es la correcta.");
	pa2m_afirmar(*(int *)hash_obtener(nuevo_hash, clave) == elemento &&
			     *(int *)hash_obtener(nuevo_hash, clave) ==
				     elemento &&
			     *(int *)hash_obtener(nuevo_hash, clave_dos) ==
				     elemento_dos &&
			     *(int *)hash_obtener(nuevo_hash, clave_tres) ==
				     elemento_tres &&
			     *(int *)hash_obtener(nuevo_hash, clave_cuatro) ==
				     elemento_cuatro &&
			     *(int *)hash_obtener(nuevo_hash, clave_cinco) ==
				     elemento_cinco,
		     "Todos los elementos estan insertados correctamente.");

	hash_destruir(nuevo_hash);
}

void pruebas_obtencion_y_contener()
{
	size_t capacidad = 10;
	hash_t *nuevo_hash = hash_crear(capacidad);

	char *clave = "prueba";
	int elemento = 1;
	char *clave_dos = "prueba1";
	int elemento_dos = 2;
	char *clave_tres = "prueba2";
	int elemento_tres = 3;
	char *clave_invalida = "invalida";

	hash_insertar(nuevo_hash, clave, &elemento, NULL);
	hash_insertar(nuevo_hash, clave_dos, &elemento_dos, NULL);
	hash_insertar(nuevo_hash, clave_tres, &elemento_tres, NULL);

	pa2m_afirmar(hash_obtener(NULL, clave) == NULL,
		     "Obtener un elemento de un hash invalido devuelve NULL.");
	pa2m_afirmar(hash_obtener(nuevo_hash, NULL) == NULL,
		     "Obtener una clave invalida devuelve NULL.");
	pa2m_afirmar(hash_obtener(nuevo_hash, clave_invalida) == NULL,
		     "Obtener una clave no insertada devuelve NULL.");
	pa2m_afirmar(*(int *)hash_obtener(nuevo_hash, clave) == elemento,
		     "Buscar una calve valida, devuelve el elemento correcto.");
	hash_quitar(nuevo_hash, clave);
	pa2m_afirmar(hash_obtener(nuevo_hash, clave) == NULL,
		     "Obtener una clave que fue borrada devuelve NULL.");

	hash_destruir(nuevo_hash);
}

void eliminacion_y_actualizacion()
{
	size_t capacidad = 5;
	hash_t *nuevo_hash = hash_crear(capacidad);

	char *texto = "prueba";
	int elemento = 1;

	hash_insertar(nuevo_hash, texto, &elemento, NULL);

	char *clave_invalida = "clave invalida";
	int elemento_dos = 2;
	void *aux = NULL;

	hash_insertar(nuevo_hash, texto, &elemento_dos, &aux);

	pa2m_afirmar(hash_quitar(NULL, texto) == NULL,
		     "Intentar eliminar de un hash NULO devuelve NULL.");

	pa2m_afirmar(hash_quitar(nuevo_hash, NULL) == NULL,
		     "Eliminar una clave nula devuelve NULL.");

	pa2m_afirmar(hash_quitar(nuevo_hash, clave_invalida) == NULL,
		     "Eliminar una clave invalida devuelve NULL.");

	pa2m_afirmar(*(int *)(hash_obtener(nuevo_hash, texto)) == elemento_dos,
		     "El elemento se actualizo correctamente");

	pa2m_afirmar(hash_cantidad(nuevo_hash) == 1,
		     "Actualizar un elemento no aumenta la cantidad");

	pa2m_afirmar(*(int *)aux == elemento,
		     "El elemento anterior se guardo correctamente");

	pa2m_afirmar(*(int *)hash_quitar(nuevo_hash, texto) == elemento_dos,
		     "Eliminar devuelve el elemento correcto.");

	pa2m_afirmar(hash_quitar(nuevo_hash, texto) == NULL,
		     "Eliminar una clave ya borrada devuelve NULL.");

	hash_destruir_todo(nuevo_hash, NULL);
}

void pruebas_de_insercion()
{
	size_t capacidad = 5;
	hash_t *nuevo_hash = hash_crear(capacidad);

	char *clave = "prueba";
	int elemento = 1;

	pa2m_afirmar(hash_insertar(NULL, clave, &elemento, NULL) == NULL,
		     "Insertar en un hash nulo devuelve NULL");
	pa2m_afirmar(hash_insertar(nuevo_hash, NULL, &elemento, NULL) == NULL,
		     "Insertar una clave invalida devuelve NULL.");
	pa2m_afirmar(hash_insertar(nuevo_hash, clave, NULL, NULL) != NULL,
		     "Se puede insertar un elemento NULL.");

	hash_insertar(nuevo_hash, clave, &elemento, NULL);

	pa2m_afirmar(hash_cantidad(nuevo_hash) == 1,
		     "Insertar un elemento aumenta la cantidad");
	pa2m_afirmar(*(int *)(hash_obtener(nuevo_hash, clave)) == elemento,
		     "El elemento se inserto correctamente");

	pa2m_afirmar(*(int *)hash_quitar(nuevo_hash, clave) == elemento,
		     "Eliminar un elemento devuelve el elemento correcto.");
	pa2m_afirmar(
		hash_cantidad(nuevo_hash) == 0,
		"Luego de eliminar un elemento la cantidad es la correcta.");

	hash_destruir_todo(nuevo_hash, NULL);
}

void pruebas_creacion()
{
	size_t capacidad = 5;
	hash_t *nuevo_hash = hash_crear(capacidad);

	pa2m_afirmar(hash_cantidad(nuevo_hash) == 0,
		     "Un hash recien creado tiene cantidad cero.");

	hash_destruir(nuevo_hash);

	capacidad = 1;
	nuevo_hash = hash_crear(capacidad);

	pa2m_afirmar(
		nuevo_hash != NULL,
		"Puede crearse un hash con capacidad menor a 3 correctamente.");
	hash_destruir(nuevo_hash);
}

int main()
{
	pa2m_nuevo_grupo("Pruebas de creacion.");
	pruebas_creacion();
	pa2m_nuevo_grupo("Pruebas de Incersion.");
	pruebas_de_insercion();
	pa2m_nuevo_grupo("Pruebas de eliminacion y actualizacion.");
	eliminacion_y_actualizacion();
	pa2m_nuevo_grupo("Pruebas de obtencion y contener.");
	pruebas_obtencion_y_contener();
	pa2m_nuevo_grupo("Pruebas de rehash.");
	pruebas_rehash();
	pa2m_nuevo_grupo("Pruebas con el iterador interno.");
	prueba_iterador_interno();

	return pa2m_mostrar_reporte();
}
