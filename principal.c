#include "pesquisa.h"
//arquivo decrescente vai de 1999999 ate 0

int main(int argc,char *argv[]) {
    clock_t start, end;
    double elapsed;
    start = clock();

    int metodo =atoi(argv[1]);
    int qtd =atoi(argv[2]);
    int situacao =atoi(argv[3]);
    int chave =atoi(argv[4]);
    
    
    
    //int *chaves=gerarChaves(qtd,situacao);
    int acessos=0,comparacoes=0,contarq=0;
    FILE *arq;
    tipoitem aux;

    if((metodo == 1 && situacao != 1) || (metodo > 4 || metodo < 1) || (qtd > 2000000 || qtd < 1) || (situacao > 4 || situacao < 1)){
        printf("Entrada invalida!!\n\nMetodos:\n1:Pesquisa sequencialmente indexada(Apenas ordem crescente)\n2:Arvore binaria externa\n3:Arvore B\n4:Arvore B*\n");
        printf("\nQuantidade permitida: Entre 1 item e 2 Milhoes\n");
        printf("\nSituacoes:\n1:Ordenado crescentemente\n2:Ordenado decrescentemente\n3:Ordenado aleatoriamente\n");
        return 0;
    }
        
    
    if(situacao == 1){
            if((arq = fopen("crescente.bin","rb")) == NULL){
                printf("Erro na abertura do arquivo\n");
                return 0;
            }
        }
        else if(situacao == 2){
            if((arq = fopen("decrescente.bin","rb")) == NULL){
                printf("Erro na abertura do arquivo\n");
                return 0;
            }
        }
        else{
            if((arq = fopen("aleatorio.bin","rb")) == NULL){
                printf("Erro na abertura do arquivo\n");
                return 0;
            }
        }
    
    if((argc == 6) &&  strcmp(argv[5],"-P") == 0){
        while(fread(&aux,sizeof(tipoitem),1,arq) > 0 && contarq < qtd){
            printf("%d ",aux.chave);
            contarq++;
        }
        printf("\n");
        rewind(arq);
    }
    

    switch (metodo)
    {
    case 1:
    {
        int tabela[MAXTABELA];
        tipoitem x[ITENSPAGINA];
    
        
        int pos=0,cont=0;
        while(fread(&x,sizeof(tipoitem),ITENSPAGINA,arq) > 0 && cont < qtd){
            tabela[pos] = x[0].chave;
            pos++;
            cont+=ITENSPAGINA;
            acessos++;
        }
        fseek(arq,0,SEEK_SET);
        
        if (pesquisaPSI(tabela, pos, chave,qtd, arq,&acessos,&comparacoes))
            printf ("Registro de codigo %d foi localizado\n",chave);
        else
            printf ("Registro de codigo %d nao foi localizado\n",chave);
        rewind(arq);
        fclose(arq);
        break;
    }
    case 2:
    {

        FILE *arquivo2 = fopen("arvorebin.bin", "wb+");
        tipoitem temp;
        tipoitemarvore itemcompleto;
    
        
        int pos=0;
    
        while(fread(&temp,sizeof(tipoitem),1,arq) == 1 && pos < qtd){
            itemcompleto.chave=temp.chave;
            itemcompleto.dado1=temp.dado1;
            strcpy(temp.dado2,itemcompleto.dado2);
            itemcompleto.ant=-1;
            itemcompleto.prox=-1;
            itemcompleto.posicao=pos;
            itemcompleto.atualizado=0;
            fwrite(&itemcompleto,sizeof(tipoitemarvore),1,arquivo2);
            pos++;
            acessos++;
        }

        rewind(arquivo2);

        for (int i = 1; i < pos; i++) {
            atualizaPonteiroABP(i, arquivo2,&acessos,&comparacoes);
        }
        
        int retorno=procuraArvoreABP(chave,arquivo2,&acessos,&comparacoes);
        if(retorno != -1)
            printf ("Registro de codigo %d foi localizado\n",chave);
        else    
            printf ("Registro de codigo %d nao foi localizado\n",chave);
        rewind(arquivo2);
        fclose(arquivo2);
        break;
    }
    case 3:
    {    
        tipoitem temp;
        
        TipoApontador Ap=NULL;
        Inicializa(Ap);
        int cont=0;
        while(fread(&temp,sizeof(tipoitem),1,arq) == 1 && cont < qtd){
            InsereAB(temp,&Ap,&comparacoes);
            cont++;
            acessos++;
        }
        fclose(arq);
        int retorno=PesquisaAB(chave,Ap,&comparacoes);
        if(retorno>0)
            printf ("Registro de codigo %d foi localizado\n",chave);
        else    
            printf ("Registro de codigo %d nao foi localizado\n",chave);
        //Imprime(Ap);
        limpar(Ap);
        
        break;
    }
    case 4:
    {
        

    
        tipoitem temp,pesquisado;
        pesquisado.chave=chave;
        
        TipoApontador2 Ap=NULL;
        int cont=0;
        while(fread(&temp,sizeof(tipoitem),1,arq) == 1 && cont < qtd){
            InsereEstrela(temp,&Ap,&comparacoes);
            cont++;
            acessos++;
        }
        rewind(arq);
        fclose(arq);
        
        int retorno=PesquisaEstrela(&pesquisado,&Ap,&comparacoes);
        if(retorno)
            printf ("Registro de codigo %d foi localizado\n",chave);
        else    
            printf ("Registro de codigo %d nao foi localizado\n",chave);
        
        
        
        break;
    }
}
    
    end = clock();

    elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nTempo de execucao: %f segundos\nTransferencias : %d\nComparacoes: %d\n", elapsed,acessos,comparacoes);
    return 0;
}
