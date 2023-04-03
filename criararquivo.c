
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
typedef struct{
    int chave;
    long int dado1;
    char dado2[500];
}tipoitem;

int main(){
  FILE *arquivo = fopen("aleatorio.bin", "wb+");
    tipoitem registro;
    for(int i=0; i<2000000; i++){
        
        registro.chave=i;
        registro.dado1=i;
        
        fwrite(&registro,1,sizeof(tipoitem),arquivo);
        }
        
    srand(time(NULL));
    for (int i = 0; i < 2000000; i++) {
        int troca = rand() % 2000000;
        
        tipoitem temp1, temp2;

        // lendo o primeiro item
        fseek(arquivo, troca * sizeof(tipoitem), SEEK_SET);
        fread(&temp1, sizeof(tipoitem), 1, arquivo);

        // lendo o segundo item
        fseek(arquivo, i * sizeof(tipoitem), SEEK_SET);
        fread(&temp2, sizeof(tipoitem), 1, arquivo);
        //printf("%d %d %d\n",troca,temp1.chave,temp2.chave);
        // escrevendo o primeiro item na posição do segundo item
        fseek(arquivo, i * sizeof(tipoitem), SEEK_SET);
        fwrite(&temp1, sizeof(tipoitem), 1, arquivo);

        // escrevendo o segundo item na posição do primeiro item
        fseek(arquivo, troca * sizeof(tipoitem), SEEK_SET);
        fwrite(&temp2, sizeof(tipoitem), 1, arquivo);

        }
    
    rewind(arquivo);
    fclose(arquivo);

    arquivo = fopen("crescente.bin", "wb+");
    
    for(int i=0; i<2000000; i++){
        
        registro.chave=i;
        registro.dado1=i;
        
        fwrite(&registro,1,sizeof(tipoitem),arquivo);
        }
        
    rewind(arquivo);
    fclose(arquivo);


    arquivo = fopen("decrescente.bin", "wb+");
        
        for(int i=0; i<2000000; i++){
            
            registro.chave=2000000-i-1;
            registro.dado1=i;
            
            fwrite(&registro,1,sizeof(tipoitem),arquivo);
            }
    return 0;
}
