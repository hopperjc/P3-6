#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <GL/glut.h>
#include <errno.h>

float prod_interno(float vec1[], float vec2[]);
void normalizar(float vec[], float ret[]);
void prod_vetorial(float vec1[], float vec2[], float ret[]);
void proj_vetores(float vec1[], float vec2[], float ret[]);
void mul_escalar(float vec[], float k, float ret[]);

void carregar_camera();
void carregar_iluminacao();
void carregar_objetos();
void normalizar_triangulos();

//variaveis da camera
float C[3];
float N[3];
float V[3];
float U[3];
float d;
float hx;
float hy;

//variaveis de iluminacao
float Pl[3];
float ka;
int Ia[3];
float kd;
float Od[3];
float ks;
int Il[3];
float n;

//variaveis de objeto
int num_pontos;
int num_triangulos;
float** pontos;
int** triangulos;
float** normais_pontos;
float** normais_triangulos;

int main(int argc, char **argv)
{
    carregar_objetos();
    /*
    normalizar_triangulos();
    int i;
    for(i = 0; i < num_triangulos; i++)
    {
        printf("%f %f %f\n", normais_triangulos[i][0], normais_triangulos[i][1], normais_triangulos[i][2]);
    }
    */
    return 0;
}

float prod_interno(float vec1[], float vec2[])
{
    return (vec1[0]*vec2[0] + vec1[1]*vec2[1] + vec1[2]*vec2[2]);
}

void normalizar(float vec[], float ret[])
{
    float mod = sqrt(prod_interno(vec,vec));

    ret[0] = vec[0]/mod;
    ret[1] = vec[1]/mod;
    ret[2] = vec[2]/mod;
}

void prod_vetorial(float vec1[], float vec2[], float ret[])
{
    ret[0] = vec1[1]*vec2[2] - vec1[2]*vec2[1];
    ret[1] = vec1[2]*vec2[0] - vec1[0]*vec2[2];
    ret[2] = vec1[0]*vec2[1] - vec1[1]*vec2[0];
}

void proj_vetores(float vec1[], float vec2[], float ret[])
{
    float prop = prod_interno(vec1,vec2)/prod_interno(vec2,vec2);

    ret[0] = prop * vec2[0];
    ret[1] = prop * vec2[1];
    ret[2] = prop * vec2[2];
}

void mul_escalar(float vec[], float k, float ret[])
{
    ret[0] = vec[0] * k;
    ret[1] = vec[1] * k;
    ret[2] = vec[2] * k;
}

void sub_vet(float vet1[], float vet2[], float ret[])
{
    ret[0] = vet1[0] - vet2[0];
    ret[1] = vet1[1] - vet2[1];
    ret[2] = vet1[2] - vet2[2];
}

void carregar_camera()
{
    FILE *fp;
    fp = fopen("camera.cfg","r");
    if (fp == NULL) {
        printf ("File not created okay, errno = %d\n", errno);
    }
    fscanf(fp," %f %f %f", &C[0], &C[1], &C[2]);
    fscanf(fp," %f %f %f", &N[0], &N[1], &N[2]);
    fscanf(fp," %f %f %f", &V[0], &V[1], &V[2]);
    fscanf(fp," %f %f %f", &d, &hx, &hy);

    fclose (fp);

    //Ortogonalizar V
    float aux1[3];
    float aux2[3];
    proj_vetores(V, N, aux1);
    sub_vet(V, aux1, aux2);
    V[0] = aux2[0];
    V[1] = aux2[1];
    V[2] = aux2[2];
    //Encontrar U
    prod_vetorial(V, N, U);

}

void carregar_iluminacao()
{
    FILE *fp;
    fp = fopen("iluminacao.txt","r");
    if (fp == NULL) {
        printf ("File not created okay, errno = %d\n", errno);
    }
    fscanf(fp," %f %f %f", &Pl[0], &Pl[1], &Pl[2]);
    fscanf(fp," %f", &ka);
    fscanf(fp," %d %d %d", &Ia[0], &Ia[1], &Ia[2]);
    fscanf(fp," %f", &kd);
    fscanf(fp," %f %f %f", &Od[0], &Od[1], &Od[2]);
    fscanf(fp," %f", &ks);
    fscanf(fp," %d %d %d", &Il[0], &Il[1], &Il[2]);
    fscanf(fp," %f", &n);

    fclose (fp);
}

void carregar_objetos()
{
    FILE *fp;
    fp = fopen("objeto.byu","r");
    if (fp == NULL) {
        printf ("Erro de leitura em arquivo, errno = %d\n", errno);
    }

    fscanf(fp," %d %d", &num_pontos, &num_triangulos);

    int i;
    pontos =  (float**) calloc(num_pontos, sizeof(float));
    for(i = 0; i < num_pontos; i++)
    {
        pontos[i] = (float*) calloc(3, sizeof(float));
        fscanf(fp," %f %f %f", &pontos[i][0], &pontos[i][1], &pontos[i][2]);
    }

    triangulos = (int**) calloc(num_triangulos, sizeof(int));
    for(i = 0; i < num_triangulos; i++)
    {
        triangulos[i] = (int*) calloc(3, sizeof(int));
        fscanf(fp," %d %d %d", &triangulos[i][0], &triangulos[i][1], &triangulos[i][2]);
    }

    fclose (fp);
}

void normalizar_triangulos()
{
    /*
    int i;
    float aux1[3];
    float aux2[3];
    float aux3[3];
    float aux4[3];
    float aux5[3];
    
    normais_triangulos = (float**) calloc(num_triangulos, sizeof(float));
    for(i = 0; i < num_triangulos; i++)
    {
        printf("%f\n",triangulos[i][0]);
        aux1[0] = pontos[(int)triangulos[i][0]][0];
        aux1[1] = pontos[(int)triangulos[i][0]][1];
        aux1[2] = pontos[(int)triangulos[i][0]][2];
        aux2[0] = pontos[(int)triangulos[i][1]][0];
        aux2[1] = pontos[(int)triangulos[i][1]][1];
        aux2[2] = pontos[(int)triangulos[i][1]][2];
        sub_vet(aux2, aux1, aux3);
        aux2[0] = pontos[(int)triangulos[i][2]][0];
        aux2[1] = pontos[(int)triangulos[i][2]][1];
        aux2[2] = pontos[(int)triangulos[i][2]][2];
        sub_vet(aux2, aux1, aux4);
        prod_vetorial(aux3, aux4, aux5);
        printf("%f %f %f", aux5[0], aux5[1], aux5[2]);
        normais_triangulos[i] = (float*) calloc(3, sizeof(float));
        normais_triangulos[i][0] = aux5[0];
        normais_triangulos[i][1] = aux5[1];
        normais_triangulos[i][2] = aux5[2];
    }
    */
}