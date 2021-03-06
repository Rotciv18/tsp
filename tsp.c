#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "vnd.h"
#define OTIMA 1273

float gap (float valor, float otimo){
    return (((valor - otimo)/otimo) *100);
}

int calc_distance (float x1, float x2, float y1, float y2){
    float d = ((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1));
    return round(sqrt(d));
}

void mount_display_data_section (int n, int **M, float x[n], float y[n]){
    int i, j;
    int start = 0;
    for (i = 0; i < n ; i++)
    {
        printf ("\ni = %d", i);
        for (j = start; j < n; j++){
            if (i == j){
                M[i][j] = M[j][i] = 0;
                continue;
            }
            M[i][j] = M[j][i] = calc_distance(x[i], x[j], y[i], y[j]);
        }
        start++;
    }
    printf ("\n");
    /*for (i = 0; i < n ; i++){
        for (j = 0; j < n; j++){
            if (i == j){
                printf (" %d ", M[i][j]);
                continue;
            }
            printf(" %d ", M[i][j]);
        }
        printf ("\n");
    }*/
}

void mount_edge_weight_section (int n, int **M, FILE* file){
    int i, j;
    for (i = 0; i<n;i++){
        for (j=0;j<n;j++){
            fscanf(file, "%d", &M[i][j]);
        }
    }

    //return M;
}

void nearestNeighbor (int n, int **M, int caminho[n], int custo[n], float alfa){
    int i, j, k, menorCusto, menorCustoIndex, maiorCusto, maiorCustoIndex, aux, indexRandom;
    bool new_route = false;
/*
    bool *visited;
    visited = malloc(n* sizeof(bool));*/
    bool visited[n];

    int *LCR;
    LCR = malloc(n* sizeof(int));

    float custolimite = 0;
    float random;
    int countLCR = 0;

    for (i = 0; i < n; i++){ //inicializa array de v�rtices visitados
        visited[i] = false;
    }

    visited[0] = visited[n] = true;
    //start = 1;
    for (i = 0; i < n-1; i++){
        menorCusto = INT_MAX;
        maiorCusto = 0;
        for (j = 1; j<n; j++){
            if (i == j)
                continue;
            if (M[caminho[i]][caminho[j]] <= menorCusto && !visited[j]){
                menorCusto = M[caminho[i]][caminho[j]];
                menorCustoIndex = j;
            }
            if (M[caminho[i]][caminho[j]] >= maiorCusto && !visited[j]){
                maiorCusto = M[caminho[i]][caminho[j]];
                maiorCustoIndex = j;
            }
        }
        custolimite = menorCusto + (alfa * (maiorCusto - menorCusto));

        countLCR = 0;

        //cria LCR
        for(k = 0; k<n; k++){
            if (i == k)
                continue;
            if(M[caminho[i]][caminho[k]] < custolimite && !visited[k]){
                LCR[countLCR] = k;
                countLCR++;
            }
        }
        if (countLCR == 0){
            break;
        }
        srand(time(NULL));

        random = rand() % countLCR;
        indexRandom = (int)round(random);
        indexRandom = LCR[indexRandom];
        new_route = true;

        if (new_route){
            aux = caminho[i+1];
            caminho[i+1] = caminho[indexRandom];
            caminho[indexRandom] = aux;
            new_route = false;
            visited[indexRandom] = true;
        }
        //calcSolucao(n, M, caminho, custo);
        //start++;
    }
    free(LCR);
    //free(visited);
}

void caminhoAleatorio (int n, int **M, int caminho[n], int custo[n]){
    bool visited[n+1];
    int vizinhanca[n];
    int i, j, index;

    for (i = 0; i<=n;i++)
        visited[i] = false;
    visited[0] = visited[n] = true;

    for (i = 0; i < n; i++){
        index = 0;

        for (j = 0; j < n; j++){
            if (!visited[j]){
                vizinhanca[index] = j;
                index++;
            }
        }

        if (index == 0){
            caminho[i+1] = 0;
        } else {
            //srand(time(NULL));
            srand(time(NULL));
            int vizinho_selecionado = rand() % index;
            caminho[i+1] = vizinhanca[vizinho_selecionado];
            visited[vizinhanca[vizinho_selecionado]] = true;
        }
    }

}

int main (){
    FILE* file = fopen ("tsp3.txt", "r"); //abre o arquivo passado como argumento
    if (file == NULL)
    {
      printf ("Falha ao tentar ler arquivo.");
      return 1;
    }
    int n;
    int i = 0;
    const char *s[100];

    while (fscanf(file, "%s", &s[i])){
        if (!strcmp(&s[i], "DIMENSION:")){
            fscanf(file, "%d", &n);
            break;
        }
        ++i;
    }

    printf ("%s %d\n", &s[i], n);
    const char *s1[2000];
    bool display = false;
    bool edge = false;
    while (fscanf(file, "%s", &s1[i])){
        if (!strcmp(&s1[i], "DISPLAY_DATA_SECTION")){
            display = true;
            break;
        }
        if (!strcmp(&s1[i], "EDGE_WEIGHT_SECTION")){
            edge = true;
            break;
        }
        ++i;
    }
    //int M[n][n];
    int **M;
    M = (int**)malloc(sizeof(int*)*n+1);
    for (i = 0; i <= n; i++)
        M[i] = (int*)malloc(sizeof(int)*n+1);
    if (display){
        int index[n];
        float x[n];
        float y[n];

        for (i = 0; i<n ; i++){
            fscanf(file, "%d", &index[i]);
            //printf("\n%d", index[i]);
            fscanf(file, "%f", &x[i]);
            fscanf(file, "%f", &y[i]);
        }
        for (i = 0; i<n;i++){
            printf ("\n%d   %f    %f", index[i], x[i], y[i]);
        }
        mount_display_data_section(n, M, x, y); //L� coordenadas na se��o DISPLAY_DATA e converte para matriz de adjac�ncia
    }
    if (edge){
        mount_edge_weight_section (n, M,file); //L� a matriz dada na se��o EDGE_WEIGHT_SECTION
    }
    fclose(file);

    int j;
    /*
    for (i = 0; i < n ; i++){
        for (j = 0; j < n; j++){
            printf(" %d ", M[i][j]);
        }
        printf ("\n");
    }
    */

    int caminho[n+1]; // um vertice em caminho[x] ir� para caminho[x+1]
    int custo[n];
    for (i = 0 ; i <= n ; i++){ //constr�i caminho arbitr�rio
        if (i == n){
            caminho[i] = caminho[0];
            break;
        }
        custo[i] = INT_MAX;
        caminho[i] = i;
    }

    int solucao, menorSolucao;

    nearestNeighbor(n, M, caminho, custo, 0);
    menorSolucao = calcSolucao(n, M, caminho, custo);
    printf("\nSolucao vizinho mais proximo: %d", menorSolucao);
    //printf("\nGap: %f", gap(menorSolucao, OTIMA));
    vnd (n, M, caminho, custo);
    solucao = calcSolucao(n, M, caminho, custo);
    printf ("\nSolucao vnd: %d", solucao);
    //printf("\nGap: %f", gap(solucao, OTIMA));
    if (menorSolucao < solucao)
        menorSolucao = solucao;

    FILE* filew = fopen ("solucao.txt", "w");
    fprintf(filew, "Solucao: %d\nCaminho: ", solucao);
    for (i = 0 ; i <= n ; i++){
        fprintf(filew, "%d, ", caminho[i]);
    }
    fclose(filew);


    i = 0;
    int *solucoes;
    int *solucoesC;

    solucoesC = malloc(10*sizeof(int));
    solucoes = malloc(10*sizeof(int));
    printf ("\n----Iniciando GRASP----\n\n");

    //---------GRASP----------
    while (i < 10){
        nearestNeighbor(n, M, caminho, custo, 0.6);
        solucoesC[i] = calcSolucao(n, M, caminho, custo);
        printf("\nConstrutiva heur�stica: %d", calcSolucao(n, M, caminho, custo));
        vnd(n, M, caminho, custo);
        solucao = calcSolucao(n, M, caminho, custo);
        solucoes[i] = solucao;
        printf ("\nBusca local: %d", solucao);
        if (menorSolucao > solucao){
            menorSolucao = solucao;
            filew = fopen("solucao.txt", "w");
            fprintf(filew, "Solucao: %d\nCaminho: ", solucao);
            for (j = 0 ; j <= n ; j++){
                fprintf(filew, "%d, ", caminho[j]);
            }
            fclose(filew);
        }
        i++;
    }
    int mediaC;
    int media = mediaC = 0;
    for (i = 0; i<10;i++){
        media = media + solucoes[i];
        mediaC = mediaC + solucoesC[i];
    }

    mediaC = mediaC/10;
    media = media/10;

    printf("\n\nMedia construtiva: %d\nMedia meta-heur�stica: %d", mediaC, media);
    printf("\nMenor solucao encontrda: %d", menorSolucao);
    //printf("\nGap: %f", gap(menorSolucao, OTIMA));

    free(solucoes);
    free(solucoesC);
}
