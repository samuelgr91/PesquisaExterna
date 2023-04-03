#include "pesquisa.h"

int* gerarChaves(int qtd,int situacao){
    int  i, j, num, flag;
    int *vetor=malloc(20*sizeof(int));
    int inf=2000000-qtd,sup=2000000;//arquivo decrescente vai de 2M-1 ate 0, entao preciso de chaves entre esses valores
    srand(time(NULL)); // inicializa o gerador de números aleatórios

    for (i = 0; i < 20; i++) {
        do {
            if(situacao==2)
                num = (rand() % (sup-inf+1) )+ inf;
            else
                num = rand() % (qtd + 1); // gera um número aleatório entre 0 e max
            flag = 1;
            for (j = 0; j < i; j++) {
                if (num == vetor[j]) {
                    flag = 0;
                    break;
                }
            }
        } while (!flag);
        vetor[i] = num;
    }
    return vetor;
}



int PesquisaBinaria(tipoitem *pagina,int esq,int dir,int x,int *acessos,int *comparacoes){
  int meio=(int)(esq+dir)/2;
  if(esq > dir)
    return -1;

  else if(x < pagina[meio].chave){
    (*comparacoes)++;
    return PesquisaBinaria(pagina, esq,meio-1, x,acessos,comparacoes);
  }
    
  else if(x > pagina[meio].chave){
    (*comparacoes)++;
    return PesquisaBinaria(pagina, meio+1,dir, x,acessos,comparacoes);
  }
    
  else 
    return meio;
}


void atualizaPonteiroABP(int posicaoFilho, FILE *arquivo,int *acessos,int* comparacoes) {
    int posicaoPai = 0;
    tipoitemarvore filho, pai;
    fseek(arquivo, posicaoFilho * sizeof(tipoitemarvore), SEEK_SET);//resgata registro do filho
    fread(&filho, sizeof(tipoitemarvore), 1, arquivo);
    (*acessos)++;

    // Obtém a raiz da árvore
    fseek(arquivo, 0, SEEK_SET);//inicializa na raiz 
    fread(&pai, sizeof(tipoitemarvore), 1, arquivo);
    (*acessos)++;
    while (1) {
        if (filho.chave < pai.chave) {
            (*comparacoes)++;
            if (pai.ant == -1) {//encontrou a posicao
                break;
            }
            posicaoPai = pai.ant;//caso contrario tem que procurar fazendo o caminhamento pela esquerda
            fseek(arquivo, pai.ant * sizeof(tipoitemarvore), SEEK_SET);
            fread(&pai, sizeof(tipoitemarvore), 1, arquivo);
            (*acessos)++;
        }
        else if (filho.chave > pai.chave) {
            (*comparacoes)++;
            if (pai.prox == -1) {//encontrou a posicao 
                break;
            }
            posicaoPai = pai.prox;//caso contrario tem que procurar fazendo o caminhamento pela direita
            fseek(arquivo, pai.prox * sizeof(tipoitemarvore), SEEK_SET);
            fread(&pai, sizeof(tipoitemarvore), 1, arquivo);
            (*acessos)++;
        }
    }

    // Atualiza o ponteiro anterior ou próximo do pai com a posição do filho
    if (filho.chave < pai.chave) {
        (*comparacoes)++;
        pai.ant = posicaoFilho;
    }
    else {
        (*comparacoes)++;
        pai.prox = posicaoFilho;
    }
    fseek(arquivo, posicaoPai * sizeof(tipoitemarvore), SEEK_SET);//vai ate a posicao do pai e sobrescreve com o ponteiro do filho atualizado
    fwrite(&pai, sizeof(tipoitemarvore), 1, arquivo);
    (*acessos)++;
}

int procuraArvoreABP(int chave, FILE *arquivo,int *acessos,int* comparacoes) {//funçao faz o caminhamento para encontrar um determinado registro
    
    tipoitemarvore pai;
    fseek(arquivo, 0, SEEK_SET);

    while (fread(&pai, sizeof(tipoitemarvore), 1, arquivo) == 1) {
        (*acessos)++;
        (*comparacoes)++;
        if (chave == pai.chave)//encontrou 
           return chave;

        else if (chave < pai.chave) {
            (*comparacoes)++;
            if (pai.ant == -1) {//item nao existe
                return -1;
            }
            fseek(arquivo, pai.ant * sizeof(tipoitemarvore), SEEK_SET);//continua caminhamento pela esquerda
        }
        else if (chave > pai.chave) {
            (*comparacoes)++;
            if (pai.prox == -1) {//item nao existe
                return -1;
            }
            fseek(arquivo, pai.prox * sizeof(tipoitemarvore), SEEK_SET);//continua caminhamento pela direita
        }
    }
    return -1;
}




int pesquisaPSI(int tab[],int tam,int chave,int qtd,FILE *arq,int *acessos,int *comparacoes){
    rewind(arq);
    tipoitem pagina[ITENSPAGINA];
    int i,quantitens;
    long desloc;

    i=0;
    
    while(i < tam && tab[i] <= chave)//i vai ate a possivel pagina do registro
       i++;
    (*comparacoes)+=i;
    if(i == 0)//nao tem nenhuma pagina
        return 0;
    else {
        if(i<tam)
            quantitens = ITENSPAGINA;
        else{
            //fseek(arq , 0, SEEK_END);
            quantitens=qtd%ITENSPAGINA;//se qtd de itens nao for multiplo da qtd por pagina a ultima pagina esta "incompleta"
            if(quantitens == 0)
                quantitens=ITENSPAGINA;
        }
    
        desloc = (i-1) * ITENSPAGINA *sizeof(tipoitem);
        fseek (arq, desloc,SEEK_SET);
        fread(&pagina,sizeof(tipoitem),quantitens, arq);
        (*acessos)++;
        int retorno=PesquisaBinaria(pagina,0,quantitens-1,chave,acessos,comparacoes);//pesquisa pelo item dentro da pagina
        if(retorno == -1)
            return 0;
        else 
            return 1;
      
    }
}


void Inicializa(TipoApontador Arvore){
    Arvore = NULL;
}

int PesquisaAB(int chave,TipoApontador Ap,int *comparacoes){
    long i=1;
    if(Ap == NULL)
        return 0;
    while(i<Ap->n && chave > Ap->r[i-1].chave){//vai ate a possivel posicao do item ou do seu pai
        (*comparacoes)++;
        i++;
    }
       
    if(chave == Ap->r[i-1].chave){//verifica se achou o item
        (*comparacoes)++;
        return 1;
    }
        //caso contrario continua a busca pelo lado correto ate encontrar item ou ate apontar para NULL(nao achou)
    else if(chave < Ap->r[i-1].chave){
        (*comparacoes)++;
        return PesquisaAB(chave,Ap->p[i-1],comparacoes);
    }
       
    else    {
        (*comparacoes)++;
        return PesquisaAB(chave,Ap->p[i],comparacoes);
    }
        
}

void Imprime(TipoApontador arvore){//imprime arvore b crescente
    int i = 0;
    if (arvore == NULL) 
        return;
    while (i <= arvore->n) {
        Imprime(arvore->p[i]);
        if(i != arvore->n)
            printf("%d ",arvore->r[i].chave);
        i++;
    }
}

void InsereNaPagina(TipoApontador Ap,tipoitem Reg,TipoApontador Apdir,int *comparacoes){
    
    int k;
    k=Ap->n;
    short NaoAchouPosicao=0;
    if(k>0)//verifica se existe elementos
        NaoAchouPosicao=1;
    while(NaoAchouPosicao){//se existe elementos procura a posicao correta e move os elementos necessarios e seus ponteiros
        (*comparacoes)++;
        if(Reg.chave >= Ap->r[k-1].chave){
            
            NaoAchouPosicao=0;
            break;
        }
        Ap->r[k]=Ap->r[k-1];
        Ap->p[k+1]=Ap->p[k];
        k--;
        if(k<1)
            NaoAchouPosicao=0;
    }
    Ap->r[k]=Reg;
    Ap->p[k+1]=Apdir;
    Ap->n++;
}

void Ins(tipoitem Reg,TipoApontador Ap,short *Cresceu,tipoitem *RegRetorno,TipoApontador *ApRetorno,int *comparacoes){
    long i=1;
    
    TipoApontador ApTemp;
    if(Ap == NULL){//ou nao existe a arvore ou chegou nas folhas
        *Cresceu=1;
        (*RegRetorno) = Reg;
        (*ApRetorno) = NULL;
        return;
    }
    while(i < Ap->n && Reg.chave > Ap->r[i-1].chave){//encontra local correto para inserçao 
        (*comparacoes)++;
        i++;
    }
        
    if(Reg.chave == Ap->r[i-1].chave){//ja existe
        (*comparacoes)++;
        *Cresceu=0;
        return;
    }
    if(Reg.chave < Ap->r[i-1].chave){//caso seja menor tem que caminhar pelo ponteiro a esquerda do item maior que ele
        (*comparacoes)++;
        i--;
    }
        
    Ins(Reg,Ap->p[i],Cresceu,RegRetorno,ApRetorno,comparacoes);//faz recursao ate encontrar local de inserçao
    if(!(*Cresceu))
        return;
    if(Ap->n < MM){//caso pagina cabe item
        InsereNaPagina(Ap,*RegRetorno,*ApRetorno,comparacoes);
        *Cresceu=0;
        return;
    }
    ApTemp=malloc(sizeof(TipoPagina));//criacao de nova pagina pois a que deveria ser inserirido esta lotada
    ApTemp->n=0;
    ApTemp->p[0]=NULL;

    if(i< (M+1)){//saber se o item ficara na nova pagina ou na pagina existente
        InsereNaPagina(ApTemp,Ap->r[MM-1],Ap->p[MM],comparacoes);
        Ap->n--;
        InsereNaPagina(Ap,*RegRetorno,*ApRetorno,comparacoes);
    }
    else    
        InsereNaPagina(ApTemp,*RegRetorno,*ApRetorno,comparacoes);
        
    for(int j=M+2;j<=MM;j++)//move os itens para a nova pagina
        InsereNaPagina(ApTemp,Ap->r[j-1],Ap->p[j],comparacoes);

    Ap->n=M;//atualiza o tamanho da nova pagina
    ApTemp->p[0]=Ap->p[M+1];//primeiro filho da nova pagina é o ultimo da pagina que foi dividida
    *RegRetorno=Ap->r[M];//registro que deve subir
    *ApRetorno=ApTemp;//filho a direita do que vai subir
}


void InsereAB(tipoitem Reg,TipoApontador *Ap,int *comparacoes){
    short Cresceu;
    tipoitem RegRetorno;
    TipoPagina *ApRetorno,*ApTemp;
    Ins(Reg,*Ap,&Cresceu,&RegRetorno,&ApRetorno,comparacoes);
    if(Cresceu){//se precisa de nova raiz
        ApTemp=malloc(sizeof(TipoPagina));
        ApTemp->n=1;
        ApTemp->r[0]=RegRetorno;
        ApTemp->p[1]=ApRetorno;
        ApTemp->p[0]=*Ap;
        *Ap=ApTemp;
    }
}


void limpar(TipoApontador arvore){
    int i = 0;
    if (arvore == NULL) return;
    while (i <= arvore->n) {
        limpar(arvore->p[i]);
        i++;
    }
    free(arvore);
}



void InsereNaPaginaEstrela(TipoApontador2 Ap,int Reg,TipoApontador2 Apdir,int *comparacoes){
    int k;//insere na pagina indice
    //printf("%d \n",Reg);
        k=Ap->UU.U0.ni;
    
        short NaoAchouPosicao=0;
        if(k>0)
            NaoAchouPosicao=1;
        while(NaoAchouPosicao){
            (*comparacoes)++;
            if(Reg >= Ap->UU.U0.ri[k-1]){
                NaoAchouPosicao=0;
                break;
            }
            Ap->UU.U0.ri[k]=Ap->UU.U0.ri[k-1];
            Ap->UU.U0.pi[k+1]=Ap->UU.U0.pi[k];
            k--;
            if(k<1)
                NaoAchouPosicao=0;
        }
        Ap->UU.U0.ri[k]=Reg;//insere a chave
        Ap->UU.U0.pi[k+1]=Apdir;//atualiza o ponteiro a direita do novo item
        Ap->UU.U0.ni++;
    
}

void InsereNaPaginaFolhaEstrela(TipoApontador2 Ap,tipoitem Reg,TipoApontador2 Apdir,int *comparacoes){
    int k;//insere na pagina externa
        
        k=Ap->UU.U1.ne;
        short NaoAchouPosicao=0;
        if(k>0)
            NaoAchouPosicao=1;
        while(NaoAchouPosicao){
            (*comparacoes)++;
            if(Reg.chave >= Ap->UU.U1.re[k-1].chave){
                NaoAchouPosicao=0;
                break;
            }
            Ap->UU.U1.re[k]=Ap->UU.U1.re[k-1];
            //Ap->p[k+1]=Ap->p[k];
            k--;
            if(k<1)
                NaoAchouPosicao=0;
        }
        Ap->UU.U1.re[k]=Reg;//insere todo o registro
        //Ap->p[k+1]=Apdir;//nao tem ponteiros
        Ap->UU.U1.ne++;
    
    
}


void InsEstrela(tipoitem Reg,TipoApontador2 Ap,short *Cresceu,tipoitem *RegRetorno,TipoApontador2 *ApRetorno,int *comparacoes){
    long i=1;
    
    TipoApontador2 ApTemp;
    //como ja foi criada a raiz, e as folhas nao tem apontadores nao precisa da verificacao do Ap==NULL
    if(Ap->Pt == Interna){//enccontra pagina externa ideal pra inserçao
        while(i < Ap->UU.U0.ni && Reg.chave > Ap->UU.U0.ri[i-1]){
            (*comparacoes)++;
            i++;
        }
        

        (*comparacoes)++;
        if(Reg.chave < Ap->UU.U0.ri[i-1])
            i--;
        
        InsEstrela(Reg,Ap->UU.U0.pi[i],Cresceu,RegRetorno,ApRetorno,comparacoes);
    }
    else{
        while(i < Ap->UU.U1.ne && Reg.chave > Ap->UU.U1.re[i-1].chave){//quando na pagina externa precisa decobrir a posicao
            (*comparacoes)++;
            i++;
        }
        
            
    }
    if(!(*Cresceu))//verifica se o processo ja finalizou, caso cresceu==True ele continua o programa
        return;

    if(Ap->Pt == Externa)//o Registro da recursao tem que ser atualizado para o item que vai ser inserido na pagina externa
        *RegRetorno=Reg;
    
    if(Ap->Pt == Interna && Ap->UU.U0.ni < MM){
        InsereNaPaginaEstrela(Ap,RegRetorno->chave,*ApRetorno,comparacoes);
        *Cresceu=0;
        return;
    }
    
    else if(Ap->Pt == Interna && Ap->UU.U0.ni >= MM){  //cria pagina interna nova 
        ApTemp=malloc(sizeof(TipoPagina2));
        ApTemp->Pt=Interna;
        ApTemp->UU.U0.ni=0;
        ApTemp->UU.U0.pi[0]=NULL;
        

        if(i< (M+1)){//saber se o item ficara na nova pagina ou na pagina existente
        InsereNaPaginaEstrela(ApTemp,Ap->UU.U0.ri[MM-1],Ap->UU.U0.pi[MM],comparacoes);
        Ap->UU.U0.ni--;
        InsereNaPaginaEstrela(Ap,RegRetorno->chave,*ApRetorno,comparacoes);
        }
        else    
            InsereNaPaginaEstrela(ApTemp,RegRetorno->chave,*ApRetorno,comparacoes);
            
        for(int j=M+1;j<=MM;j++)
            InsereNaPaginaEstrela(ApTemp,Ap->UU.U0.ri[j-1],Ap->UU.U0.pi[j],comparacoes);
        
        Ap->UU.U0.ni=M;
        ApTemp->UU.U0.pi[0]=Ap->UU.U0.pi[M+1];//ponteiro a esquerda da nova pagina recebe o ponteiro mais a direita da pagina dividida
        RegRetorno->chave=ApTemp->UU.U0.ri[0];//apenas a chave do item que deve subir
        *ApRetorno=ApTemp;//filho a direita do item que vai subir
        
    }

       
    else if(Ap->Pt == Externa && Ap->UU.U1.ne < MM){
            InsereNaPaginaFolhaEstrela(Ap,*RegRetorno,NULL,comparacoes);
            *Cresceu=0;
            return;
        }

    else if(Ap->Pt == Externa && Ap->UU.U1.ne >= MM){//cria nova pagina externa
            ApTemp=malloc(sizeof(TipoPagina2));
            ApTemp->Pt=Externa;
            ApTemp->UU.U1.ne=0;

            if(i< (M+1)){//saber se o item ficara na nova pagina ou na pagina existente
                InsereNaPaginaFolhaEstrela(ApTemp,Ap->UU.U1.re[MM-1],NULL,comparacoes);
                Ap->UU.U1.ne--;
                InsereNaPaginaFolhaEstrela(Ap,*RegRetorno,*ApRetorno,comparacoes);
            }
            else
                InsereNaPaginaFolhaEstrela(ApTemp,*RegRetorno,*ApRetorno,comparacoes);
            
            for(int j=M+1;j<=MM;j++)
                InsereNaPaginaFolhaEstrela(ApTemp,Ap->UU.U1.re[j-1],NULL,comparacoes);


            Ap->UU.U1.ne=M;
            *RegRetorno=ApTemp->UU.U1.re[0];//o item que vai subir
            *ApRetorno=ApTemp;//filho a direita do que vai subir é a nova pagina    
    }
}



void InsereEstrela(tipoitem Reg,TipoApontador2 *Ap,int *comparacoes){
    short Cresceu=1;//inicia com a criaçao de pagina nova
    tipoitem RegRetorno;
    TipoPagina2 *ApRetorno,*ApTemp,*ApTemp2;
    if(*Ap == NULL ){//ja inicializa primeira pagina como externa
        
        ApTemp2 = malloc(sizeof(TipoPagina2));
        ApTemp2->Pt=Externa;
        ApTemp2->UU.U1.ne=1;
        ApTemp2->UU.U1.re[0]=Reg;
    
        *Ap=ApTemp2;
        
    }
    else{
        InsEstrela(Reg,*Ap,&Cresceu,&RegRetorno,&ApRetorno,comparacoes);
        if(Cresceu){
            ApTemp=malloc(sizeof(TipoPagina2));
            ApTemp->Pt=Interna;
            ApTemp->UU.U0.ni=1;
            ApTemp->UU.U0.ri[0]=RegRetorno.chave;
            ApTemp->UU.U0.pi[0]=*Ap;
            ApTemp->UU.U0.pi[1]=ApRetorno;
            *Ap=ApTemp;    
    }
    }
    
}


int PesquisaEstrela(tipoitem *x,TipoApontador2 *Ap,int *comparacoes){
    int i;
    TipoApontador2 Pag;
    Pag=*Ap;
    if((*Ap)->Pt == Interna){
        i=1;
        (*comparacoes)++;
        while(i < Pag->UU.U0.ni && x->chave > Pag->UU.U0.ri[i-1])
            i++; 
        
        (*comparacoes)++;       
        if(x->chave < Pag->UU.U0.ri[i-1])
            return PesquisaEstrela(x,&Pag->UU.U0.pi[i-1],comparacoes);
        else    
            return PesquisaEstrela(x,&Pag->UU.U0.pi[i],comparacoes);
    }
    i=1;
    while(i < Pag->UU.U1.ne && x->chave > Pag->UU.U1.re[i-1].chave){
        (*comparacoes)++;
        i++;
    }
        
    if(x->chave == Pag->UU.U1.re[i-1].chave){
        (*comparacoes)++;
        return 1;
    }
        
    else
        return 0;
}



