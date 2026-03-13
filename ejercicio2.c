/*
 * Objetivo: Utilizar el comportamiento LIFO para invertir cadenas.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// =========================================================
// DEFINICIONES DE TIPOS
// =========================================================

typedef char DATA;

typedef struct elemento {
    DATA d;
    struct elemento *siguiente;
} ELEMENTO;

typedef struct {
    int cnt;
    ELEMENTO *tope;
} PILA;

// =========================================================
// PRIMITIVAS DE LA PILA
// =========================================================

void inicializar(PILA *s) {
    s->cnt = 0;
    s->tope = NULL;
}

int estavacia(PILA *s) {
    return s->cnt == 0;
}

void push(PILA *s, DATA x) {
    ELEMENTO *nuevo = (ELEMENTO *)malloc(sizeof(ELEMENTO));
    nuevo->d = x;
    nuevo->siguiente = s->tope;
    s->tope = nuevo;
    s->cnt++;
}

DATA pop(PILA *s) {
    ELEMENTO *temp = s->tope;
    DATA valor = temp->d;
    s->tope = temp->siguiente;
    s->cnt--;
    free(temp);
    return valor;
}

// =========================================================
// FUNCIÓN PRINCIPAL: esPalindromo
// =========================================================

int esPalindromo(char cadena[]) {
    PILA original, invertida;
    inicializar(&original);
    inicializar(&invertida);

    int i, longitud = strlen(cadena);

    // 1. Filtrar solo letras y llenar 'original'
    for (i = 0; i < longitud; i++) {
        if (isalpha(cadena[i]))
            push(&original, tolower(cadena[i]));
    }

    // 2. Pasar original a invertida (el orden se invierte solo)
    //    Luego reconstruir original pasando por copia
    PILA copia;
    inicializar(&copia);

    while (!estavacia(&original))
        push(&invertida, pop(&original));

    // Reconstruir original y tener copia al mismo tiempo
    while (!estavacia(&invertida)) {
        DATA c = pop(&invertida);
        push(&original, c);
        push(&copia, c);
    }

    // 3. Comparar original vs copia letra a letra
    while (!estavacia(&original) && !estavacia(&copia)) {
        if (pop(&original) != pop(&copia))
            return 0; // No es palíndromo
    }

    return 1; // Es palíndromo
}

// =========================================================
// MAIN
// =========================================================

int main() {
    printf("--- TEST DE PALINDROMOS ---\n\n");

    char prueba1[] = "Anita lava la tina";
    char prueba2[] = "Estructuras de Datos";
    char prueba3[] = "A man a plan a canal Panama";
    char prueba4[] = "Reconocer";
    char prueba5[] = "anita";

    printf("'%s'\n  -> %s\n\n", prueba1,
           esPalindromo(prueba1) ? "ES PALINDROMO" : "NO ES PALINDROMO");

    printf("'%s'\n  -> %s\n\n", prueba2,
           esPalindromo(prueba2) ? "ES PALINDROMO" : "NO ES PALINDROMO");

    printf("'%s'\n  -> %s\n\n", prueba3,
           esPalindromo(prueba3) ? "ES PALINDROMO" : "NO ES PALINDROMO");

    printf("'%s'\n  -> %s\n\n", prueba4,
           esPalindromo(prueba4) ? "ES PALINDROMO" : "NO ES PALINDROMO");

    printf("'%s'\n  -> %s\n\n", prueba5,
           esPalindromo(prueba5) ? "ES PALINDROMO" : "NO ES PALINDROMO");

    return 0;
}
