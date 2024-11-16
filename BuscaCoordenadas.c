#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio2.h>
#include "Headers\\Moldura.h"
#include "Headers\\KDTree.h"
#include "Headers\\Pilha.h"
#include "Headers\\PilhaInt.h"

void BuscaProximos (Lista **L, KDTree *raiz, int Pto[K], int raio) {
	Pilha *P;

	init(&P);
	push(&P,raiz);
	while(!isEmpty(P)) {
		pop(&P,&raiz);
		if(raiz) {
			if(DistanciaEuclidiana(Pto,raiz->ponto) <= raio)
				InsereLista(&*L,raiz);
			if(raiz->dir)
				push(&P,raiz->dir);
			if(raiz->esq)
				push(&P,raiz->esq);
		}
	}
}

int main (void) {
	KDTree *T = NULL;
	Lista *L = NULL;
	int Pontos[TF][2], Ponto[2], raio;

	RetiraCursor();
	Dimensao();
	MolduraCompleta();

	InserePontos(Pontos,Ponto,&raio); // Gera 20 pontos aleatórios (2 dimensões) de 1 a 40.
	ExibePontos(Pontos); // Exibe os pontos antes da ordenação
	InsereArvoreR(&T,Pontos,0,TF,0); // Insere os pontos na árvore do tipo KD-Tree de maneira Recursiva
	//InsereArvore(&T,Pontos); // Insere os pontos na árvore do tipo KD-Tree de maneira Iterativa
	ExibeArvore(T);
	getch();

	ExibeDados(Ponto,raio);
	ExibePlano(T,Ponto,4,7,130,49,0);
	getch();
	BuscaProximos(&L,T,Ponto,raio);
	ExibeProximos(L);
	getch();

	MsgFim();
	getch();

	return 0;
}
