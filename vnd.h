#ifndef VND_H_INCLUDED
#define VND_H_INCLUDED
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>

int rand_lim(int min_n, int max_n) {
    return rand() % (max_n - min_n + 1) + min_n;
}

//Retorna Solução dado um caminho atual
int calcSolucao (int n, int **M, int caminho[n], int custo[n]){
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

bool vndSwap (int n, int **M, int caminho[n], int custo[n], int *solucao){

    int i, j, start, novaSolucao, menorSolucao, v1, v2, aux;
    bool swaped = false;

    i = j = start = 1;
    menorSolucao = *solucao;
    novaSolucao = INT_MAX;

    while (i < n-1){
        while (j < n-1){
            if (i == j){
                j++;
                continue;
            }
            if (j == i+1){ //Vértices adjacentes
                novaSolucao = *solucao - M[caminho[i-1]][caminho[i]] - M[caminho[j]][caminho[j+1]]
                                  + M[caminho[i-1]][caminho[j]] + M[caminho[i]][caminho[j+1]];
            } else{ // Se os dois vértices não são adjacentes~
                novaSolucao = *solucao - M[caminho[i-1]][caminho[i]] - M[caminho[j]][caminho[j+1]]
                                  + M[caminho[i-1]][caminho[j]] + M[caminho[i]][caminho[j+1]]
                                  - M[caminho[i]][caminho[i+1]] - M[caminho[j-1]][caminho[j]]
                                  + M[caminho[j]][caminho[i+1]] + M[caminho[j-1]][caminho[i]];
            }
            if (novaSolucao < menorSolucao){
                menorSolucao = novaSolucao;
                v1 = i;
                v2 = j;
            }
            j++;
        }
        j = ++start;
        i++;
    }
    if (menorSolucao < *solucao){
        aux = caminho[v1];
        caminho[v1] = caminho[v2];
        caminho[v2] = aux;
        swaped = true;
        *solucao = calcSolucao(n, M, caminho, custo);
        printf("\nSWAP Nova solucao: %d", *solucao);
    }

    return swaped;
}

bool vnd_reinsert (int n, int **M, int caminho[n], int custo[n], int *solucao){
    int i, j, start, menorSolucao, v1, v2, aux;
    bool swaped = false;
    int sol = *solucao;
    i = j = start = 1;
    menorSolucao = *solucao;
    int novaSolucao = INT_MAX;

    while (i<n-1){
        while (j<n){
            if (i == j){
                j++;
                continue;
            }
            novaSolucao = *solucao - M[caminho[i-1]][caminho[i]] - M[caminho[i]][caminho[i+1]] - M[caminho[j]][caminho[j+1]]
                                   + M[caminho[i-1]][caminho[i+1]] + M[caminho[j]][caminho[i]] + M[caminho[i]][caminho[j+1]];
            if (novaSolucao < menorSolucao){
                v1 = i;
                v2 = j;
                menorSolucao = novaSolucao;
            }
            j++;
        }
        j = ++start;
        i++;
    }
    if (menorSolucao < *solucao){
        aux = caminho[v1];
        for (i = v1; i <= v2; i++){
           if (i == v2){
               caminho[i] = aux;
               break;
           }
           caminho[i] = caminho[i+1];
        }
        *solucao = calcSolucao (n, M, caminho, custo);
        swaped = true;
        printf ("\nRE-INSERT nova solucao: %d", *solucao);
    }
    return swaped;
}

bool vnd_twoOpt(int n, int **M, int caminho[n], int custo[n], int *solucao){
    int i, j, menorSolucao, novaSolucao, v1, v2, aux;
    bool swaped = false;

    i = j = 1;
    menorSolucao = *solucao;
    novaSolucao = INT_MAX;

    while (i < n-2){
        for (j = i+3 ; j<=n; j++){
            novaSolucao = *solucao - M[caminho[i-1]][caminho[i]] - M[caminho[j+1]][caminho[j]]
                                   + M[caminho[j+1]][caminho[i]] + M[caminho[i-1]][caminho[j]];
            if (novaSolucao < menorSolucao){
                menorSolucao = novaSolucao;
                v1 = i;
                v2 = j;
            }
        }
        ++i;
    }
    j = v2;
    if (menorSolucao < *solucao){
        for (i = v1; i<j;i++){
            aux = caminho[i];
            caminho[i] = caminho[j];
            caminho[j] = aux;
            j--;
        }
        *solucao = calcSolucao(n, M, caminho, custo);
        printf ("\n2-OPT Nova solucao: %d", *solucao);
        swaped = true;
    }
    return swaped;
}

void vnd (int n, int M[n][n], int caminho[n], int custo[n]){
    bool foundSol = false;
    int i = 0;
    int solucao = calcSolucao(n, M, caminho, custo);
    /*int novaSolucao = INT_MAX;
    int i, j, aux;
    i = j = 1; //Começa de 1 pois 0 é o vértice de partida e não deve sofrer swap
    int start = 1;
*/
    while (true){
        if (!vndSwap(n, M, caminho, custo, &solucao))
            break;
    }

    while (true){
        if (!vnd_twoOpt(n, M, caminho, custo, &solucao))
            break;
    }

    while (true){
        if (!vnd_reinsert(n, M, caminho, custo, &solucao))
            break;
    }
    printf("\nCaminho: ");
    for (i = 0 ; i <= n ; i++){
        printf ("%d, ", caminho[i]);
    }
    printf ("\nSolucao total: %d", calcSolucao(n, M, caminho, custo));




    //------2-OPT-----
    /*int k;
    for (k = 4 ; k < n ; k++){
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
        }
    }*/

}


#endif // VND_H_INCLUDED
