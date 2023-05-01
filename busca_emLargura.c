//
// Created by galdu on 30/04/2023.
//

#include "busca_emLargura.h"
#include "gqueue.h"
Vertices* bfs(Vertices* g, int v, GQUEUE* queue){
    g->FLAG[v] = 1;
    QUEUEput(queue, &g->inicio[v]);

    while (!QUEUEempty(queue)){
        NO* u = QUEUEget(queue);
        int u_val = u->val;
        while (u){
            if(u->prox && g->FLAG[u->prox->val] == 0){
                g->FLAG[v] = 1;
                QUEUEput(queue, u->prox);
            }
            u = u->prox;
        }
    }

}
Vertices* buscaEmLargura(Vertices* g, int v, int timer){
    Vertices* resp = inicializa();
    GQUEUE* queue = QUEUEinit(&g->inicio[v]);
    for(int i = 0; i < tamanho; i++)
        g->FLAG[i] = 0;



}