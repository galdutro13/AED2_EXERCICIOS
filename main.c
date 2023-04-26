#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define tamanho 8

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

Vertices* inicializa(){

    Vertices* listadj; //array de vértices -> contains the head of every adjacency list.

    NO* listaNos;
    if(!(listadj = (Vertices*)malloc(sizeof(Vertices)))){
        fprintf(stderr, "Não foi possível alocar a lista de adjacência!");
        exit(1);
    }

    if(!(listaNos = (NO*)malloc(sizeof(NO) * tamanho))){
        fprintf(stderr, "Não foi possível alocar os nós para listadj!");
        exit(1);
    }

    if(!(listadj->FLAG = (int*)malloc(sizeof(int) * tamanho))){
        fprintf(stderr, "Não foi possível alocar os nós para listadj!");
        exit(1);
    }

    if(!(listadj->DIST = (int*)malloc(sizeof(int) * tamanho))){
        fprintf(stderr, "Não foi possível alocar os nós para listadj!");
        exit(1);
    }

    if(!(listadj->VIA = (int*)malloc(sizeof(int) * tamanho))){
        fprintf(stderr, "Não foi possível alocar os nós para listadj!");
        exit(1);
    }

    listadj->inicio = listaNos;
    return listadj;

}


NO* criaNO(int valor){
    NO* no;

    if(!(no = (NO*)malloc(sizeof(NO)))){
        fprintf(stderr, "Não foi possível alocar um nó!");
        exit(1);
    }

    no->val = valor;
    no->prox = NULL;

    return no;
}

void addAresta(NO* head, NO* adj){

    if(head && adj){
        while(head->prox){
            head = head->prox;
        }
        head->prox = adj;

    } else {
        fprintf(stderr, "Não foi possível adicionar essa aresta.");
        exit(1);
    }
}

bool arestaFinder(Vertices* g, NO* head, NO* adj){
    if(head && adj && !g->FLAG[adj->val]){
        if(head->val == adj->val){
            return true;
        }

        g->FLAG[adj->val] = 1;
        if(adj->prox)
            return arestaFinder(g, head, adj->prox);

    }

    return false;
}

bool existeAresta(Vertices* g, NO* head, NO* adj){
    bool retValue = arestaFinder(g, head, adj);

    for(int i = 0; i < tamanho; i++){
        g->FLAG[i] = 0;
    }

    return retValue;
}


void novaAresta(Vertices* lista, int head, int adj){
    if(head < tamanho && adj < tamanho){
        NO* headNO = &lista->inicio[head];
        NO* adjNO = criaNO(adj);

        if(!existeAresta(lista, headNO, adjNO)){
            addAresta(headNO, adjNO);
        }

        lista->inicio[head] = *headNO;

        return;
    }

    fprintf(stderr, "Essa aresta não existe");
    exit(1);
}


Vertices* copia(Vertices* g){
    Vertices* copia = inicializa();

    for(int i = 0; i < tamanho; i++){
        NO* head = &g->inicio[i];
        NO* adj = head->prox;

        while(adj){
            novaAresta(copia, head->val, adj->val);
            adj = adj->prox;
        }
    }

    return copia;
}

Vertices* transposta(Vertices* g){
    Vertices* transp = inicializa();

    for(int i = 0; i < tamanho; i++){
        NO* head = &g->inicio[i];

        for(int j = 0; j < tamanho; j++){
            NO* adj = &g->inicio[j];

            if(existeAresta(g, head, adj)){
                novaAresta(transp, adj->val, head->val);
            }
        }
    }

    return transp;
}

int countLoops(Vertices* g, int v){
    int count = 0;
    g->FLAG[v] = 1;

    NO* p = &g->inicio[v];
    while(p){
        if(p->prox && g->FLAG[p->prox->val] == 0){
            return count + countLoops(g, p->prox->val);
        }
        else if(p->prox && g->FLAG[p->prox->val] == 1){
            g->FLAG[v] = 2;
            return 1;
        }
        p = p->prox;
        //p++; ???
    }

    return count;
}

int main() {
    Vertices* listadj = inicializa();

    int i;
    for(i = 0; i < tamanho; i++) {

        listadj->inicio[i] = *criaNO(i);
    }

    novaAresta(listadj, 0, 1);
    novaAresta(listadj, 0, 2);
    novaAresta(listadj, 1, 2);

    novaAresta(listadj, 3, 4);
    novaAresta(listadj, 3, 5);
    novaAresta(listadj, 4, 2);
    novaAresta(listadj, 4, 0);
    novaAresta(listadj, 2, 3);


    printf("%d\n", countLoops(listadj, 0));

    return 0;
}
