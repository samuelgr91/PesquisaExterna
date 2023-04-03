# ifndef pesquisa_h
# define pesquisa_h
#include <time.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ITENSPAGINA 1000
#define MAXTABELA 20000
#define M 4
#define MM (M*2)
#define MM2 (M*2)
typedef struct{
    int chave;
    long int dado1;
    char dado2[500];
}tipoitem;

typedef struct{
    int acessos,comparacoes;
}dados;

typedef struct{
    int chave;
    long int dado1;
    char dado2[500];
    int prox,ant;
    int posicao,atualizado;
}tipoitemarvore;

typedef struct TipoPagina* TipoApontador;

typedef struct TipoPagina {
    short n;
    tipoitem r[MM];
    TipoApontador p[MM + 1];
}TipoPagina;


typedef enum {Interna,Externa} TipoIntExt;

typedef struct TipoPagina2* TipoApontador2;

typedef struct TipoPagina2{
    TipoIntExt Pt;
    union{
        struct{
            int ni;
            int ri[MM];
            TipoApontador2 pi[MM+1];
        }U0;
        struct{
            int ne;
            tipoitem re[MM2];
        }U1;
    }UU;
}TipoPagina2;





int PesquisaBinaria(tipoitem *pagina,int esq,int dir,int x,int*,int*);

int pesquisaPSI(int tab[],int tam,int chave,int qtd,FILE *arq,int*,int*);



void atualizaPonteiroABP(int posicaoFilho, FILE *arquivo,int *acessos,int* comparacoes);

int procuraArvoreABP(int chave, FILE *arquivo,int *acessos,int* comparacoes);



void Inicializa(TipoApontador Arvore);

int PesquisaAB(int chave,TipoApontador Ap,int *comparacoes);

void Imprime(TipoApontador arvore);

void InsereNaPagina(TipoApontador Ap,tipoitem Reg,TipoApontador Apdir,int *);

void Ins(tipoitem Reg,TipoApontador Ap,short *cresceu,tipoitem *RegRetorno,TipoApontador *ApRetorno,int *);

void InsereAB(tipoitem Reg,TipoApontador *Ap,int *comparacoes);

void limpar(TipoApontador arvore);

void InsereNaPaginaEstrela(TipoApontador2 Ap,int Reg,TipoApontador2 Apdir,int *);

void InsEstrela(tipoitem Reg,TipoApontador2 Ap,short *Cresceu,tipoitem *RegRetorno,TipoApontador2 *ApRetorno,int*);

void InsereEstrela(tipoitem Reg,TipoApontador2 *Ap,int *comparacoes);

int PesquisaEstrela(tipoitem *x,TipoApontador2 *Ap,int *);



void InsereNaPaginaFolhaEstrela(TipoApontador2 Ap,tipoitem Reg,TipoApontador2 Apdir,int *);

int *gerarChaves(int qtd,int);
# endif