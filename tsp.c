#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

int calc_distance (float x1, float x2, float y1, float y2){
    float d = ((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1));
    return round(sqrt(d));
}

void mount_display_data_section (int n, int M[n][n], float x[n], float y[n]){
    int i, j;
    int start = 0;
    for (i = 0; i < n - 1 ; i++)
    {
        for (j = start; j < n ; j++){
            if (i == j){
                M[i][j] = M[j][i] = 0;
                continue;
            }
            M[i][j] = M[j][i] = calc_distance(x[i], x[j], y[i], y[j]);
        }
        start++;
    }
    printf ("\n");
    for (i = 0; i < n ; i++){
        for (j = 0; j < n; j++){
            if (i == j){
                printf (" %d ", M[i][j]);
                continue;
            }
            printf(" %d ", M[i][j]);
        }
        printf ("\n");
    }
}

void mount_edge_weight_section (int n, int M[n][n], FILE* file){
    int i, j;
    for (i = 0; i<n;i++){
        for (j=0;j<n;j++){
            fscanf(file, "%d", &M[i][j]);
        }
    }
    for (i = 0; i < n ; i++){
        for (j = 0; j < n; j++){
            printf(" %d ", M[i][j]);
        }
        printf ("\n");
    }
}

int main (){
    FILE* file = fopen ("bayg29.txt", "r"); //abre o arquivo passado como argumento
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
        int M[n][n];
        mount_display_data_section(n, M, x, y); //Lê coordenadas na seção DISPLAY_DATA e converte para matriz de adjacência
    }

    if (edge){
        int M[n][n];
        mount_edge_weight_section (n, M,file); //Lê a matriz dada na seção EDGE_WEIGHT_SECTION
    }
    fclose(file);
}
