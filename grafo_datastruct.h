//
// Created by galdu on 30/04/2023.
//

#ifndef COUNTLOOPS_GRAFO_DATASTRUCT_H
#define COUNTLOOPS_GRAFO_DATASTRUCT_H
#include <stdlib.h>
#include <stdbool.h>

#define tamanho 10

typedef struct no{
    int val;
    struct no* prox;
}NO;

typedef struct {
    NO* inicio;
    int* FLAG;
    int* DIST;
    int* VIA;
} Vertices;


Vertices* inicializa();
void novaAresta(Vertices* lista, int head, int adj);
#endif //COUNTLOOPS_GRAFO_DATASTRUCT_H
