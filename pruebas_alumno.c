#include <stdlib.h>
#include <stdio.h>
#include "pa2m.h"
#include "src/hash.h"

bool modificar_vector(const char* clave, void* valor, void* aux)
{
	int* valor_aux = (int*)valor;
	(*valor_aux) = (*valor_aux) + *(int*)aux;
	return true;
}

void pruebas_rehash()
{
	size_t capacidad = 4;
	hash_t* nuevo_hash = hash_crear(capacidad);

	char* texto = "prueba";
	int elemento = 1;
	char* texto2 = "prueba1";
	int elemento2 = 2;
	char* texto3 = "prueba2";
	int elemento3 = 3;
	char* texto4 = "prueba3";
	int elemento4 = 4;
	char* texto5 = "prueba4";
	int elemento5 = 5;

	hash_insertar(nuevo_hash, texto, &elemento, NULL);
	hash_insertar(nuevo_hash, texto2, &elemento2, NULL);
	hash_insertar(nuevo_hash, texto3, &elemento3, NULL);
	hash_insertar(nuevo_hash, texto4, &elemento4, NULL);
	hash_insertar(nuevo_hash, texto5, &elemento5, NULL);

	pa2m_afirmar(hash_cantidad(nuevo_hash) == 5, "Luego de rehashear la cantidad de elemento es la correcta.");
	pa2m_afirmar(*(int*)hash_obtener(nuevo_hash, texto) == elemento && *(int*)hash_obtener(nuevo_hash, texto) == elemento && 
	*(int*)hash_obtener(nuevo_hash, texto2) == elemento2 && *(int*)hash_obtener(nuevo_hash, texto3) == elemento3 && 
	*(int*)hash_obtener(nuevo_hash, texto4) == elemento4 && *(int*)hash_obtener(nuevo_hash, texto5) == elemento5, 
	"Todos los elementos estan insertados correctamente.");

	hash_destruir(nuevo_hash);
}

void pruebas_iterador()
{
    size_t capacidad = 10;
	hash_t* nuevo_hash = hash_crear(capacidad);

	char* texto1 = "prueba1";
	int elemento1 = 1;
	char* texto2 = "prueba2";
	int elemento2 = 2;
	char* texto3 = "prueba3";
	int elemento3 = 3;
	char* texto4 = "prueba4";
	int elemento4 = 4;
	char* texto5 = "prueba5";
	int elemento5 = 5;

	hash_insertar(nuevo_hash, texto1, &elemento1, NULL);
	hash_insertar(nuevo_hash, texto2, &elemento2, NULL);
	hash_insertar(nuevo_hash, texto3, &elemento3, NULL);
	hash_insertar(nuevo_hash, texto4, &elemento4, NULL);
	hash_insertar(nuevo_hash, texto5, &elemento5, NULL);

	int valor_a_sumar = 10;
	hash_con_cada_clave(nuevo_hash, modificar_vector, &valor_a_sumar);

	pa2m_afirmar(elemento1 == 11 && elemento2 == 12 && elemento3 == 13 && elemento4 == 14 && elemento5 == 15, 
	"Se aplica correctamente la funcion pasada al iterador.");

	hash_destruir_todo(nuevo_hash, NULL);
}

void eliminacion_y_actualizacion()
{
	size_t capacidad = 5;
	hash_t* nuevo_hash = hash_crear(capacidad);

	char* texto = "prueba";
	int elemento = 1;

	hash_insertar(nuevo_hash, texto, &elemento, NULL);
	
	int elemento2 = 2;
	void* aux = NULL;

	hash_insertar(nuevo_hash, texto, &elemento2, &aux);
	pa2m_afirmar(*(int*)(hash_obtener(nuevo_hash, texto)) == elemento2, "El elemento se actualizo correctamente");
	pa2m_afirmar(hash_cantidad(nuevo_hash) == 1, "Actualizar un elemento no aumenta la cantidad");
	pa2m_afirmar(*(int*)aux == elemento, "El elemento anterior se guardo correctamente");

	pa2m_afirmar(*(int*)hash_quitar(nuevo_hash, texto) == elemento2, "Eliminar devuelve el elemento correcto.");
	pa2m_afirmar(hash_quitar(nuevo_hash, texto) == NULL, "Eliminar una clave ya borrada devuelve NULL.");

	hash_destruir_todo(nuevo_hash, NULL);
}

void pruebas_de_insercion()
{
	size_t capacidad = 5;
	hash_t* nuevo_hash = hash_crear(capacidad);

	char* texto = "prueba";
	int elemento = 1;
	hash_insertar(nuevo_hash, texto, &elemento, NULL);

	pa2m_afirmar(hash_cantidad(nuevo_hash) == 1, "Insertar un elemento aumenta la cantidad");
	pa2m_afirmar(*(int*)(hash_obtener(nuevo_hash, texto)) == elemento, "El elemento se inserto correctamente");

	pa2m_afirmar(*(int*)hash_quitar(nuevo_hash, texto) == elemento, "Eliminar un elemento devuelve el elemento correcto.");
	pa2m_afirmar(hash_cantidad(nuevo_hash) == 0, "Luego de eliminar un elemento la cantidad es la correcta.");

	hash_destruir_todo(nuevo_hash, NULL);
}

void pruebas_creacion()
{
	size_t capacidad = 5;
	hash_t* nuevo_hash = hash_crear(capacidad);

	pa2m_afirmar(hash_cantidad(nuevo_hash) == 0, "Un hash recien creado tiene cantidad cero.");

	hash_destruir_todo(nuevo_hash, NULL);
}

int main()
{
	pa2m_nuevo_grupo("Pruebas de creacion.");
	pruebas_creacion();
	pa2m_nuevo_grupo("Pruebas de Incersion.");
	pruebas_de_insercion();
	pa2m_nuevo_grupo("Pruebas de eliminacion y actualizacion.");
	eliminacion_y_actualizacion();
	pa2m_nuevo_grupo("Pruebas con iterador interno.");
	pruebas_iterador();
	pa2m_nuevo_grupo("Pruebas de rehash.");
	pruebas_rehash();


	return pa2m_mostrar_reporte();
}
