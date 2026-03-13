/*
 * Objetivo: Completar las primitivas de la pila para evaluación de RPN.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

// =========================================================
// DEFINICIONES DE TIPOS (contenido de pilas.h)
// =========================================================

#define FULL 100

typedef enum { FALSO = 0, VERDADERO = 1 } BOOLEAN;
typedef enum { VALOR, OPERADOR } TIPO;

typedef struct {
    TIPO tipo;
    union {
        int val;
        char op;
    } u;
} DATO;

typedef struct elemento {
    DATO dato;
    struct elemento *siguiente;
} ELEMENTO;

typedef struct {
    int cnt;
    ELEMENTO *tope;
} PILA;

// =========================================================
// SECCIÓN 1: PRIMITIVAS DE LA PILA
// =========================================================

void inicializar(PILA *stk) {
    stk->cnt = 0; // Inicializamos el contador a 0
    stk->tope = NULL; // Inicializamos el tope a NULL
}

BOOLEAN estavacia(PILA *stk) {
    return (stk->cnt == 0) ? VERDADERO : FALSO; // La pila está vacía si el contador es 0
}

BOOLEAN estallena(PILA *stk) {
    return (stk->cnt == FULL) ? VERDADERO : FALSO; // La pila está llena si el contador alcanza FULL
}

void push(PILA *stk, DATO x) { // Agrega un nuevo elemento al tope de la pila
    ELEMENTO *nuevo = (ELEMENTO *)malloc(sizeof(ELEMENTO));
    nuevo->dato = x;
    nuevo->siguiente = stk->tope;
    stk->tope = nuevo;
    stk->cnt++;
}

DATO pop(PILA *stk) { // Elimina el elemento del tope de la pila y lo devuelve
    if (estavacia(stk)) {
        printf("ERROR: Intento de POP en pila vacia (Stack Underflow)\n");
        exit(1);
    }
    DATO valor;
    ELEMENTO *temp = stk->tope;
    valor = temp->dato;
    stk->tope = temp->siguiente;
    stk->cnt--;
    free(temp);
    return valor;
}

// =========================================================
// SECCIÓN 2: LÓGICA DE ALTO NIVEL
// =========================================================

void rellenar(PILA *stk, const char *str) {
    const char *p = str;
    char c1, c2;
    BOOLEAN b1, b2;
    DATO d;
    PILA aux;

    inicializar(stk);
    inicializar(&aux);

    while (*p != '\0') {
        while (isspace(*p) || *p == '\t' || *p == ',') {
            p++;
        }
        if (*p == '\0') break;

        b1 = (BOOLEAN)((c1 = *p) == '+' || c1 == '-' || c1 == '*');
        b2 = (BOOLEAN)((c2 = *(p + 1)) == ' ' || c2 == '\t' || c2 == ',' || c2 == '\0');

        if (b1 && b2) {
            d.tipo = OPERADOR;
            d.u.op = c1;
            p++;
        } else {
            d.tipo = VALOR;
            assert(sscanf(p, "%d", &d.u.val) == 1);
            while (*p != '\0' && !isspace(*p) && *p != '\t' && *p != ',') {
                p++;
            }
        }

        if (!estallena(&aux)) {
            push(&aux, d);
        }
    }

    while (!estavacia(&aux)) {
        d = pop(&aux);
        if (!estallena(stk)) {
            push(stk, d);
        }
    }
}

int evaluar(PILA *polaca) {
    DATO d, d1, d2;
    PILA evaluacion;

    inicializar(&evaluacion);

    while (!estavacia(polaca)) {
        d = pop(polaca);
        switch (d.tipo) {
            case VALOR:
                push(&evaluacion, d);
                break;
            case OPERADOR:
                d2 = pop(&evaluacion);
                d1 = pop(&evaluacion);
                d.tipo = VALOR;
                switch (d.u.op) {
                    case '+': d.u.val = d1.u.val + d2.u.val; break;
                    case '-': d.u.val = d1.u.val - d2.u.val; break;
                    case '*': d.u.val = d1.u.val * d2.u.val; break;
                }
                push(&evaluacion, d);
                break;
        }
    }

    d = pop(&evaluacion);
    return d.u.val;
}

int main(void) {
    PILA polaca;

    printf("--- INICIO DE PRACTICA ---\n\n");

    // Prueba 1: 3 4 5 + * => (4+5)*3 = 27
    char str1[] = "3 4 5 + *";
    inicializar(&polaca);
    rellenar(&polaca, str1);
    printf("Expresion : %s\n", str1);
    printf("Esperado  : 27\n");
    printf("Obtenido  : %d\n\n", evaluar(&polaca));

    // Prueba 2: 5 1 2 + 4 * + 3 - => 5+(1+2)*4-3 = 14
    char str2[] = "5 1 2 + 4 * + 3 -";
    inicializar(&polaca);
    rellenar(&polaca, str2);
    printf("Expresion : %s\n", str2);
    printf("Esperado  : 14\n");
    printf("Obtenido  : %d\n\n", evaluar(&polaca));

    // Prueba 3: 2 3 4 * + => 2+(3*4) = 14
    char str3[] = "2 3 4 * +";
    inicializar(&polaca);
    rellenar(&polaca, str3);
    printf("Expresion : %s\n", str3);
    printf("Esperado  : 14\n");
    printf("Obtenido  : %d\n", evaluar(&polaca));

    return 0;
}
