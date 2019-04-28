#ifndef VND_H_INCLUDED
#define VND_H_INCLUDED
#include <stdbool.h>
#include <limits.h>

//Retorna Solução dado um caminho atual
int calcSolucao (int n, int M[n][n], int caminho[n], int custo[n]){
    int i;
    int solucao = 0;
    for (i = 0; i<n; i++){
        custo[i] = M[caminho[i]][caminho[i+1]]; //FAVOR LEMBRAR DISSO QUANDO QUISER SABER CUSTOS DE CAMINHOS
    }
    for (i = 0; i < n; i++){
        solucao = solucao + custo[i];
    }
    return solucao;
}


void vnd (int n, int M[n][n], int caminho[n], int custo[n]){
    bool foundSol = false;
    int solucao = calcSolucao(n, M, caminho, custo);
    int novaSolucao = INT_MAX;
    int i, j, aux;
    i = j = 1; //Começa de 1 pois 0 é o vértice de partida e não deve sofrer swap
    int start = 1;
    //----SWAP----
    while (i < n-1){ //'n-1' pois o último vértice é o vértice de partida e não deve sofrer swap
        while (j < n-1){
            if (i == j){
                j++;
                continue;
            }
            aux = caminho [i];
            caminho[i] = caminho[j];
            caminho[j] = aux;
            j++;
            novaSolucao = calcSolucao(n, M, caminho, custo);
            if (novaSolucao < solucao){
                foundSol = true;
                break;
            }
        }
        j = ++start;
        i++;
        if (foundSol){
            printf ("\nNova solucao encontrada: %d, i = %d", novaSolucao, i);
            i = j = start = 1;
            solucao = novaSolucao;
            foundSol = false;
        }
    }
    printf ("\nNova solucao: %d", solucao);
    printf("\nCaminho: ");
    for (i = 0 ; i <= n ; i++){
        printf ("%d->", caminho[i]);
    }

    //------2-OPT-----
    int k;
    for (k = 4 ; k < n ; k++){
        printf("\nk = %d", k);
        for (i = k-3; i<k;i++){
            aux = caminho[i];
            caminho[i] = caminho[k];
            caminho[k] = aux;
        }
        novaSolucao = calcSolucao(n, M, caminho, custo);
        if (novaSolucao < solucao){
            solucao = novaSolucao;
            printf("\nOPT! Nova solucao: %d", solucao);
        } else { //desfaz a troca caso a solução não seja melhor para manter as rotas
            for (i = k-3; i<k;i++){
                aux = caminho[i];
                caminho[i] = caminho[k];
                caminho[k] = aux;
            }
            printf ("  - Deu ruim");
        }
    }

    //-----RE-INSERTION-----

}


#endif // VND_H_INCLUDED
