#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "vnd.h"

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

void nearestNeighbor (int n, int **M, int caminho[n], int custo[n]){
    int i, j, menorCusto, menorCustoIndex, aux;
    bool new_route = false;
    bool visited[n];

    for (i = 0; i < n; i++){ //inicializa array de vértices visitados
        visited[i] = false;
    }

    visited[0] = visited[n] = true;
    //start = 1;
    for (i = 0; i < n-1; i++){
        menorCusto = custo[i];
        for (j = 0; j<n; j++){
            if (i == j)
                continue;
            if (M[caminho[i]][caminho[j]] < menorCusto && !visited[j]){ //Alterado e agora tá todo cagado mas ainda funciona
                menorCusto = M[caminho[i]][caminho[j]];
                menorCustoIndex = j;
                new_route = true;
            }
        }
        if (new_route){
            aux = caminho[i+1];
            caminho[i+1] = caminho[menorCustoIndex];
            caminho[menorCustoIndex] = aux;
            new_route = false;
            visited[menorCustoIndex] = true;
        }
        calcSolucao(n, M, caminho, custo);
        //start++;
    }
}

int main (){
    FILE* file = fopen ("bier127.txt", "r"); //abre o arquivo
    if (file == NULL)
    {
      printf ("Falha ao tentar ler arquivo.");
      return 1;
    }
    int n;
    int i = 0;
    const char *s[100]; 

    while (fscanf(file, "%s", &s[i])){ //procura pela string "DIMENSION:" no arquivo
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
    while (fscanf(file, "%s", &s1[i])){ //procura por uma das duas strings para saber com o quê lidar
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
        mount_display_data_section(n, M, x, y); //Lê coordenadas na seção DISPLAY_DATA e converte para matriz de adjacência
    }
    if (edge){
        mount_edge_weight_section (n, M,file); //Lê a matriz dada na seção EDGE_WEIGHT_SECTION
    }
    fclose(file);

    int j;
    /*
    for (i = 0; i < n ; i++){ //printa a matriz de adjacencia
        for (j = 0; j < n; j++){
            printf(" %d ", M[i][j]); 
        }
        printf ("\n");
    }
    */

    int caminho[n+1]; // array contendo a ROTA de uma solução
    int custo[n];     //array contendo o CUSTO de cada aresta (custo[i] == M[caminho][i]][caminho[i+]]
    for (i = 0 ; i <= n ; i++){ //constrói caminho arbitrário
        if (i == n){
            caminho[i] = caminho[0];
            break;
        }
        custo[i] = INT_MAX;
        caminho[i] = i;
    }

    int solucao, menorSolucao;

    //heurística construtiva
    nearestNeighbor (n, M, caminho, custo);
    menorSolucao = calcSolucao(n, M, caminho, custo);
    printf("\nSolucao vizinho mais proximo: %d", menorSolucao);
    //busca local
    vnd (n, M, caminho, custo);

    printf("\nCaminho: ");
    //printa o caminho
    for (i = 0 ; i <= n ; i++){
        printf ("%d, ", caminho[i]);
    }
    printf ("\nSolucao total: %d", calcSolucao(n, M, caminho, custo));
    /*while (true){
        nearestNeighbor(n, M, caminho, custo);
        vnd (n, M, caminho, custo);
        printf("\nCaminho: ");
        for (i = 0 ; i <= n ; i++){
            printf ("%d, ", caminho[i]);
        }
        printf ("\nSolucao total: %d", calcSolucao(n, M, caminho, custo));
    }*/
}
