#include <stdio.h>
#include "grafo_datastruct.h"
#include "busca_emLargura.h"



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

void destroyAresta(Vertices* lista, int head, int adj){
    if(existeAresta(lista, &lista->inicio[head], &lista->inicio[adj])){

        NO* p = &lista->inicio[head];

        while(p->prox){
            if(p->prox->val == adj){
                NO* temp = p->prox;
                p->prox = p->prox->prox;
                free(temp);
                return;
            }
            p = p->prox;
        }
    }
}

void reset_metadata(Vertices* g){
    for(int k = 0; k < tamanho; k++){
        g->FLAG[k] = 0;
        g->DIST[k] = 0;
    }
}
Vertices* copia(Vertices* g){
    Vertices* copia = inicializa();

    for(int n = 0; n < tamanho; n++)
        copia->inicio[n] = *criaNO(n);

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

    for(int n = 0; n < tamanho; n++)
        transp->inicio[n] = *criaNO(n);

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
//TODO: adicionar um timer e guardar o tempo de descoberta de cada vertice em DIST
//USAR ISSO PARA DETERMINAR SE O LOOP JÁ FOI DESCOBERTO OU NÃO.
//LINK: https://www.codingninjas.com/codestudio/library/count-of-simple-cycles-in-a-connected-undirected-graph-having-n-vertices
int countLoops(Vertices* g, int v, int timer){
    int count = 0;
    g->FLAG[v] = 1;
    g->DIST[v] = timer;
    timer++;

    NO* p = &g->inicio[v];
    while(p){
        if(p->prox && g->FLAG[p->prox->val] == 0){
            return count + countLoops(g, p->prox->val, timer);
        }
        else if(p->prox && p != p->prox && g->DIST[p->prox->val] < g->DIST[v]){
            g->FLAG[v] = 2;
            return 1;
        }
        p = p->prox;
    }

    return count;
}

int destroyLoops(Vertices* g, int v, int timer){
    int count = 0;
    g->FLAG[v] = 1;
    g->DIST[v] = timer;
    timer++;

    NO* p = &g->inicio[v];
    while(p){
        if(p->prox && g->FLAG[p->prox->val] == 0){
            return count + countLoops(g, p->prox->val, timer);
        }
        else if(p != p->prox && g->DIST[p->prox->val] < g->DIST[v]){
            g->FLAG[v] = 2;
            destroyAresta(g, p->val, p->prox->val);
            return 1;
        }
        p = p->prox;
    }

    return count;
}

bool isEnraizada(Vertices* g){
    bool is_enraizada = true;
    bool isThere_loops = false;
    int incidencia[tamanho];
    int num_raiz = 0;

    for(int k = 0; k < tamanho; k++)
        incidencia[k] = 0;

    int i, j;
    for(i = 0; i < tamanho; i++){
        NO* p = &g->inicio[i];
        int head = p->val;
        for(j = 0; j < tamanho; j++){
            NO* q = &g->inicio[j];
            if(q->prox)
                q = q->prox;

            while(q){
                if(q->val == head)
                    incidencia[i] += 1;
                q = q->prox;
            }
        }
    }

    reset_metadata(g);
    for(int k = 0; k < tamanho; k++) {
        int LOOP_COUNT = countLoops(g, k, 0);
        if (incidencia[k] == 0)
            num_raiz++;
        if(LOOP_COUNT != 0 && !isThere_loops)
            isThere_loops = true;
        reset_metadata(g);
    }

    if(num_raiz != 1)
        is_enraizada = false;

    if(!isThere_loops && is_enraizada)
        return true;
    else return false;

}

int main() {
    Vertices* listadj = inicializa();

    int i;
    for(i = 0; i < tamanho; i++) {

        listadj->inicio[i] = *criaNO(i);
    }

    novaAresta(listadj, 0, 1);
    novaAresta(listadj, 0, 2);

    novaAresta(listadj, 1, 3);
    novaAresta(listadj, 2, 3);
    novaAresta(listadj, 2, 4);

    novaAresta(listadj, 3, 5);
    novaAresta(listadj, 4, 7);

    novaAresta(listadj, 5, 6);

    novaAresta(listadj, 6, 7);
    novaAresta(listadj, 7, 3);

    printf("%d\n", countLoops(listadj, 0, 0));
    //Vertices* transp = transposta(listadj);

    bool is_enraizada = isEnraizada(listadj);
    if(is_enraizada)
        printf("Grafo e uma arvore enraizada!\n");
    else printf("NÃO: Grafo não e uma arvore enraizada!\n");

    listadj = buscaEmLargura(listadj, 0, 0);
    return 0;
}
