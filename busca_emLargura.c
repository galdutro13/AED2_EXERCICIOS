//
// Created by galdu on 30/04/2023.
//

#include "busca_emLargura.h"
#include "gqueue.h"

Vertices *bfs(Vertices *g, int v, GQUEUE *queue) {
    g->FLAG[v] = 1;

    //QUEUEput(queue, &g->inicio[v]);
    int i = 0;
    while (!is_empty(queue)) {
        NO *u = &g->inicio[QUEUEget(queue)];
        int u_val = u->val;

        do {
            if (u->prox && g->FLAG[u->prox->val] == 0) {
                g->FLAG[u->prox->val] = 1;
                QUEUEput(queue, u->prox->val);
            }
            u = u->prox;
        } while (u);

        g->FLAG[u_val] = 2;
        g->VIA[i] = u_val;
        i++;
    }

    return g;
}

Vertices *buscaEmLargura(Vertices *g, int v) {
    GQUEUE *queue = QUEUEinit(v);
    for (int i = 0; i < tamanho; i++)
        g->FLAG[i] = 0;

    return bfs(g, v, queue);

}