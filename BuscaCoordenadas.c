#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio2.h>
#include "Headers\\Moldura.h"
#include "Headers\\MatEsp.h"
#include "Headers\\KDTree.h"
#include "Headers\\Pilha.h"
#include "Headers\\PilhaInt.h"

void InsereArvore (KDTree **raiz, int Pontos[TF][K]) {
	Pilha *P;
	PilhaI *PN,*PI,*PF;
	KDTree *Pai;
	int i, ini=0, fim=TF, D, meio, N=0, meioD, meioE;

	init(&P);
	initI(&PN);
	initI(&PI);
	initI(&PF);

	D = N%K;
	Ordena(Pontos,D,ini,fim);
	meio = (ini+TF)/2;
	*raiz = CriaNo(Pontos[meio]);

	push(&P,*raiz);
	pushI(&PN,1);
	pushI(&PI,ini);
	pushI(&PF,fim);

	while(!isEmpty(P)) {
		pop(&P,&Pai);
		popI(&PN,&N);
		popI(&PI,&ini);
		popI(&PF,&fim);

		if(ini<fim) {
			D=N%K;
			meio = (ini+fim)/2;
			if(meio + 1 < fim) {

				Ordena(Pontos,D,meio+1,fim);
				meioD = ((meio + 1) + fim)/2;
				Pai->dir = CriaNo(Pontos[meioD]);
				push(&P,Pai->dir);
				pushI(&PN,N+1);
				pushI(&PI,meio+1);
				pushI(&PF,fim);
			}
			if(ini < meio) {
				Ordena(Pontos,D,ini,meio);
				meioE = (ini + meio)/2;
				Pai->esq = CriaNo(Pontos[meioE]);
				push(&P,Pai->esq);
				pushI(&PN,N+1);
				pushI(&PI,ini);
				pushI(&PF,meio);
			}
		}
	}
}

void BuscaProximos (Lista **L, KDTree *raiz, int Pto[K], int raio) {
	Pilha *P;
	PilhaI *Pn;
	int nivel, D;

	init(&P);
	initI(&Pn);
	push(&P,raiz);
	push(&Pn,0);
	while(!isEmpty(P)) {
		pop(&P,&raiz);
		pop(&Pn,&nivel);
		D = nivel % K;
		if(raiz) {
			if(DistanciaEuclidiana(Pto,raiz->ponto) <= raio)
				InsereLista(&*L,raiz);
			if(raiz->dir && ((Pto[D]-raio >= raiz->ponto[D]) || (Pto[D]+raio >= raiz->ponto[D]))) {
				push(&P,raiz->dir);
				push(&Pn,nivel+1);
			}
			if(raiz->esq && ((Pto[D]-raio <= raiz->ponto[D]) || (Pto[D]+raio <= raiz->ponto[D]))) {
				push(&P,raiz->esq);
				push(&Pn,nivel+1);
			}
		}
	}
}

int main (void) {
	KDTree *T = NULL;
	Lista *L = NULL;
	int Pontos[TF][K], Ponto[K], raio;

	RetiraCursor();
	Dimensao();
	MolduraCompleta();

	InserePontos(Pontos,Ponto,&raio); // Gera 20 pontos aleatórios (X dimensões) de 1 a 40.
	ExibePontos(Pontos); // Exibe os pontos antes da ordenação (2 dimensões)
	//InsereArvoreR(&T,Pontos,0,TF,0); // Insere os pontos na árvore do tipo KD-Tree de maneira Recursiva
	InsereArvore(&T,Pontos); // Insere os pontos na árvore do tipo KD-Tree de maneira Iterativa
	ExibeArvore(T);
	getch();

	ExibeDados(Ponto,raio);
	ExibePlano(T,Ponto,4,7,130,49,0); // Funciona apenas para 2D
	getch();
	BuscaProximos(&L,T,Ponto,raio);
	ExibeProximos(L);
	getch();

	MsgFim();
	getch();

	return 0;
}
