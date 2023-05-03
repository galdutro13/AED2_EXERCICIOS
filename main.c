#include <stdio.h>
#include "grafo_datastruct.h"
#include "busca_emLargura.h"
#include "stack.h"

Vertices *inicializa() {

    Vertices *listadj; //array de vértices -> contains the head of every adjacency list.

    NO *listaNos;
    if (!(listadj = (Vertices *) malloc(sizeof(Vertices)))) {
        fprintf(stderr, "Não foi possível alocar a lista de adjacência!");
        exit(1);
    }

    if (!(listaNos = (NO *) malloc(sizeof(NO) * tamanho))) {
        fprintf(stderr, "Não foi possível alocar os nós para listadj!");
        exit(1);
    }

    if (!(listadj->FLAG = (int *) malloc(sizeof(int) * tamanho))) {
        fprintf(stderr, "Não foi possível alocar os nós para listadj!");
        exit(1);
    }

    if (!(listadj->DIST = (int *) malloc(sizeof(int) * tamanho))) {
        fprintf(stderr, "Não foi possível alocar os nós para listadj!");
        exit(1);
    }

    if (!(listadj->VIA = (int *) malloc(sizeof(int) * tamanho))) {
        fprintf(stderr, "Não foi possível alocar os nós para listadj!");
        exit(1);
    }

    listadj->inicio = listaNos;
    return listadj;

}


NO *criaNO(int valor) {
    NO *no;

    if (!(no = (NO *) malloc(sizeof(NO)))) {
        fprintf(stderr, "Não foi possível alocar um nó!");
        exit(1);
    }

    no->val = valor;
    no->prox = NULL;

    return no;
}

void addAresta(NO *head, NO *adj) {

    if (head && adj) {
        while (head->prox) {
            head = head->prox;
        }
        head->prox = adj;

    } else {
        fprintf(stderr, "Não foi possível adicionar essa aresta.");
        exit(1);
    }
}

bool arestaFinder(Vertices *g, NO *head, NO *adj) {
    if (head && adj) {
        if (head->val == adj->val) {
            return true;
        }
        if (adj->prox)
            return arestaFinder(g, head, adj->prox);

    }

    return false;
}

bool explore_grafo(Vertices *g, NO *head, NO *adj) {
    if (head && adj) {
        if (head->val == adj->val) {
            return true;
        }
        if (adj->prox)
            return arestaFinder(g, head, &g->inicio[adj->prox->val]);

    }

    return false;
}

bool existeAresta(Vertices *g, NO *head, NO *adj) {
    if (head && adj) {
        if (head->val == adj->val) {
            return true;
        }
        if (adj->prox)
            return existeAresta(g, head, adj->prox);

    }

    return false;
}


void novaAresta(Vertices *lista, int head, int adj) {
    if (head < tamanho && adj < tamanho) {
        NO *headNO = &lista->inicio[head];
        NO *adjNO = criaNO(adj);

        for (int i = 0; i < tamanho; i++) {
            lista->FLAG[i] = 0;
        }

        if (!existeAresta(lista, headNO, adjNO)) {
            addAresta(headNO, adjNO);
        }

        lista->inicio[head] = *headNO;

        return;
    }

    fprintf(stderr, "Essa aresta não existe");
    exit(1);
}

void destroyAresta(Vertices *lista, int head, int adj) {
    if (explore_grafo(lista, &lista->inicio[head], &lista->inicio[adj])) {

        NO *p = &lista->inicio[head];

        while (p->prox) {
            if (p->prox->val == adj) {
                NO *temp = p->prox;
                p->prox = p->prox->prox;
                free(temp);
                return;
            }
            p = p->prox;
        }
    }
}

void reset_metadata(Vertices *g) {
    for (int k = 0; k < tamanho; k++) {
        g->FLAG[k] = 0;
        g->DIST[k] = 0;
        g->VIA[k] = 0;
    }
}

void print_grafo(Vertices *g) {
    for (int i = 0; i < tamanho; i++) {
        printf("Vertice %d", g->inicio[i].val);
        NO *adj = &g->inicio[i];

        adj = adj->prox;
        while (adj != NULL) {

            printf(" -> ");
            printf("%d", adj->val);
            adj = adj->prox;
        }
        printf("\n");
    }
    printf("\n");
}

Vertices *copia(Vertices *g) {
    Vertices *copia = inicializa();

    for (int n = 0; n < tamanho; n++)
        copia->inicio[n] = *criaNO(n);

    for (int i = 0; i < tamanho; i++) {
        NO *head = &g->inicio[i];
        NO *adj = head->prox;

        while (adj) {
            novaAresta(copia, head->val, adj->val);
            adj = adj->prox;
        }
    }

    return copia;
}

Vertices *transposta(Vertices *g) {
    Vertices *transp = inicializa();

    for (int n = 0; n < tamanho; n++)
        transp->inicio[n] = *criaNO(n);

    for (int i = 0; i < tamanho; i++) {
        NO *head = &g->inicio[i];

        for (int j = 0; j < tamanho; j++) {
            NO *adj = &g->inicio[j];

            if (existeAresta(g, head, adj)) {
                novaAresta(transp, head->val, adj->val);
            }
        }
    }

    return transp;
}

//TODO: adicionar um timer e guardar o tempo de descoberta de cada vertice em DIST
//USAR ISSO PARA DETERMINAR SE O LOOP JÁ FOI DESCOBERTO OU NÃO.
//LINK: https://www.codingninjas.com/codestudio/library/count-of-simple-cycles-in-a-connected-undirected-graph-having-n-vertices
int countLoops(Vertices *g, int v, int *timer) {
    int count = 0;
    g->FLAG[v] = 1;
    g->DIST[v] = *timer;
    g->VIA[*timer] = v;
    *timer = *timer + 1;

    NO *p = &g->inicio[v];
    while (p) {
        if (p->prox && g->FLAG[p->prox->val] == 0) {
            count = count + countLoops(g, p->prox->val, timer);
        } else if (p->prox && g->FLAG[p->prox->val] == 1 && g->DIST[p->prox->val] < g->DIST[v]) {
            count = count + 1;
        }
        p = p->prox;
    }
    g->FLAG[v] = 2;
    return count;
}

void DFS1(Vertices *g, int v, int *timmer) {
    g->FLAG[v] = 1;
    NO *adj = &g->inicio[v];
    while (adj != NULL) {
        if (adj->prox && g->FLAG[adj->prox->val] == 0) {
            DFS1(g, adj->prox->val, timmer);
        }
        adj = adj->prox;
    }
    g->VIA[(*timmer)++] = v;
}

void DFS2(Vertices *g, int v, int *componente, int comp_index) {
    g->FLAG[v] = 1;
    componente[v] = comp_index;

    NO *adj = &g->inicio[v];

    while (adj != NULL) {
        if (adj->prox && g->FLAG[adj->prox->val] == 0) {
            DFS2(g, adj->prox->val, componente, comp_index);
        }
        adj = adj->prox;
    }
}

int *Kosaraju(Vertices *g) {
    reset_metadata(g);
    int index = 0;

    for (int i = 0; i < tamanho; i++) {
        if (g->FLAG[i] == 0)
            DFS1(g, i, &index);
    }
    //salvar as flags de visita
    Vertices *transp = transposta(g);

    int comp_index = 1;
    int *componentes = (int *) malloc(sizeof(int) * tamanho);
    while (index > 0) {
        int v = g->VIA[--index];
        if (transp->FLAG[v] == 0) {
            DFS2(transp, v, componentes, comp_index);
            for (int j = 0; j < tamanho; j++)
                printf("Busca a partir de %d resultou nas descobertas do vertice %d marcado com %d\n", v, j,
                       transp->FLAG[j]);
            comp_index++;
        }
    }

    return componentes;
}

void topologicalSortUtil(Vertices *DAG, int v, Stack *in_degree) {
    DAG->FLAG[v] = 1;
    NO *adj = &DAG->inicio[v];
    while (adj != NULL) {
        if (adj->prox && DAG->FLAG[adj->prox->val] == 0) {
            topologicalSortUtil(DAG, adj->prox->val, in_degree);
        }
        adj = adj->prox;
    }
    stack_push(in_degree, v);
}

void topologicalSort(Vertices *DAG) {
    Stack *in_degree_data = stack_init();
    reset_metadata(DAG);

    int index ;
    for (index = 0; index < tamanho; index++) {
        if (DAG->FLAG[index] == 0)
            topologicalSortUtil(DAG, index, in_degree_data);
    }
    index = 0;
    while (stack_is_empty(in_degree_data) == false) {
        DAG->VIA[index] = stack_pop(in_degree_data);
        index++;
    }
}

void destroyLoops(Vertices *g, int v, int *timer) {
    g->FLAG[v] = 1;
    g->DIST[v] = *timer;
    (*timer)++;

    NO *p = &g->inicio[v];
    while (p) {
        if (p->prox && g->FLAG[p->prox->val] == 0) {
            destroyLoops(g, p->prox->val, timer);
        } else if (p->prox && g->FLAG[p->prox->val] == 1 && g->DIST[p->prox->val] < g->DIST[v]) {
            g->FLAG[v] = 2;
            destroyAresta(g, p->val, p->prox->val);
        }
        p = p->prox;
    }
}

bool isEnraizada(Vertices *g) {
    bool is_enraizada = true;
    bool isThere_loops = false;
    int incidencia[tamanho];
    int num_raiz = 0;

    for (int k = 0; k < tamanho; k++)
        incidencia[k] = 0;

    int i, j;
    for (i = 0; i < tamanho; i++) {
        NO *p = &g->inicio[i];
        int head = p->val;
        for (j = 0; j < tamanho; j++) {
            NO *q = &g->inicio[j];
            if (q->prox)
                q = q->prox;

            while (q) {
                if (q->val == head)
                    incidencia[i] += 1;
                q = q->prox;
            }
        }
    }

    reset_metadata(g);
    for (int k = 0; k < tamanho; k++) {
        int timer = 0;
        int LOOP_COUNT = countLoops(g, k, &timer);
        if (incidencia[k] == 0)
            num_raiz++;
        if (LOOP_COUNT != 0 && !isThere_loops)
            isThere_loops = true;
        reset_metadata(g);
    }

    if (num_raiz != 1)
        is_enraizada = false;

    if (!isThere_loops && is_enraizada)
        return true;
    else return false;

}

int main() {
    Vertices *listadj = inicializa();

    int i;
    for (i = 0; i < tamanho; i++) {

        listadj->inicio[i] = *criaNO(i);
    }
    /* a<->h map 0<->7 */
    novaAresta(listadj, 0, 1);
    novaAresta(listadj, 1, 4);
    novaAresta(listadj, 4, 0);

    novaAresta(listadj, 1, 2);

    novaAresta(listadj, 1, 5);

    novaAresta(listadj, 2, 3);
    novaAresta(listadj, 3, 2);

    novaAresta(listadj, 2, 6);

    novaAresta(listadj, 5, 6);
    novaAresta(listadj, 6, 5);

    novaAresta(listadj, 3, 7);

    novaAresta(listadj, 6, 7);

    int timer = 0;
    printf("%d\n", countLoops(listadj, 0, &timer));
    //Vertices* transp = transposta(listadj);

    bool is_enraizada = isEnraizada(listadj);
    if (is_enraizada)
        printf("Grafo e uma arvore enraizada!\n");
    else printf("NÃO: Grafo não e uma arvore enraizada!\n");

    listadj = buscaEmLargura(listadj, 0);

    int *componentes = Kosaraju(listadj);
    int index = 0;
    printf("\nGrafo inicial:\n");

    Vertices *no_more_loops = copia(listadj);
    print_grafo(no_more_loops);

    printf("\nGrafo sem loops:\n");
    int reg = 0;
    for (int l = 0; l < tamanho; l++) destroyLoops(no_more_loops, l, &reg);
    print_grafo(no_more_loops);
    while (index < 8) {
        printf("Verdice de indice %d pretence ao componente: %d\n", listadj->inicio[index].val, *componentes);
        componentes++;
        index++;
    }

    topologicalSort(no_more_loops);
    printf("\n");
    for (int j = 0; j < tamanho; j++) {
        printf("%d", no_more_loops->VIA[j]);
        if (j != tamanho - 1)
            printf(" -> ");
    }
    return 0;
}
