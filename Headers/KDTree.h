#define K 2
#define TF 5

struct kdtree {
	int ponto[2];
	struct kdtree *esq, *dir;
};
typedef struct kdtree KDTree;

struct lista {
	KDTree *no;
	struct lista *prox;
};
typedef struct lista Lista;

char Folha (KDTree *no) {
	if(!no->esq && !no->dir)
		return 1;
	return 0;
}

KDTree *CriaNo (int Ponto[K]) {
	KDTree *NC = (KDTree*)malloc(sizeof(KDTree));

	NC->esq = NC->dir = NULL;
	NC->ponto[0] = Ponto[0];
	NC->ponto[1] = Ponto[1];

	return NC;
}

Lista *NovaCaixa (KDTree *no) {
	Lista *NC = (Lista*)malloc(sizeof(Lista));

	NC->prox = NULL;
	NC->no = no;

	return NC;
}

void InsereLista(Lista **L, KDTree *no) {
	Lista *aux, *Nova = NovaCaixa(no);

	if(!*L)
		*L = Nova;
	else {
		aux = *L;
		while(aux->prox)
			aux = aux->prox;
		aux->prox = Nova;
	}
}

void QtdeFilhos (KDTree *raiz, int *cont) {
	if(raiz) {
		if(raiz->esq) {
			(*cont)++;
			QtdeFilhos(raiz->esq,&*cont);
		}
		if(raiz->dir) {
			(*cont)++;
			QtdeFilhos(raiz->dir,&*cont);
		}
	}
}

void ExibeVertical (KDTree *raiz, int c, int l) {
	int i, filhos = 0, QtdeEsq = 0, QtdeDir = 0;

	if(raiz) {
		if(raiz->esq) {
			filhos = 1;
			QtdeFilhos(raiz->esq,&filhos);
		}
		c+=filhos;

		gotoxy(2+c*5,l);
		printf("(%d,%d)\n",raiz->ponto[0], raiz->ponto[1]);
		ExibeVertical(raiz->esq,c-filhos,l+1);
		ExibeVertical(raiz->dir,c+1,l+1);
	}
}

void ExibeHorizontal (KDTree *raiz) {
	int i;
	static int n=-1;

	if(raiz) {
		n++;
		ExibeHorizontal(raiz->dir);
		for(i=0; i<5*n; i++)
			printf(" ");
		printf("(%d,%d)\n", raiz->ponto[0], raiz->ponto[1]);
		ExibeHorizontal(raiz->esq);
		n--;
	}
}

void Ordena(int Pontos[TF][K], int D, int ini, int fim) {
	int i,j, aux[K],aux2 = fim;
	
	for(i=ini;i<fim-1;i++) {
		for(j=i;j<fim;j++) {
			if(Pontos[i][D] > Pontos[j][D]) {
				aux[0] = Pontos[i][0];
				aux[1] = Pontos[i][1];
				Pontos[i][0] = Pontos[j][0];
				Pontos[i][1] = Pontos[j][1];
				Pontos[j][0] = aux[0];
				Pontos[j][1] = aux[1];
			}
		}
	}
	/*while(fim >= ini) {
		for(i=ini; i<fim; i++) {
			if(Pontos[i][D] > Pontos[i+1][D]) {
				aux[0] = Pontos[i][0];
				aux[1] = Pontos[i][1];
				Pontos[i][0] = Pontos[i+1][0];
				Pontos[i][1] = Pontos[i+1][1];
				Pontos[i+1][0] = aux[0];
				Pontos[i+1][1] = aux[1];
			}
		}
		fim--;
	}*/
}

/*void InsereArvoreR (KDTree **raiz, int Pontos[TF][K], int ini, int fim, int N) {
	int D, meio;
	if(ini<fim) {
		D = N%K;
		Ordena(Pontos,D,ini,fim);
		meio = (ini+fim-1)/2;
		*raiz = CriaNo(Pontos[meio]);
		InsereArvoreR(&(*raiz)->esq,Pontos,ini,meio,N+1);
		InsereArvoreR(&(*raiz)->dir,Pontos,meio+1,fim,N+1);
	}
}*/

void InsereArvoreR (KDTree **raiz, int Pontos[TF][K], int ini, int fim, int N) {
	int D, meio;
	if(ini<fim) {
		D = N%K;
		Ordena(Pontos,D,ini,fim);
		meio = (ini+fim)/2;
		*raiz = CriaNo(Pontos[meio]);
		InsereArvoreR(&(*raiz)->esq,Pontos,ini,meio,N+1);
		InsereArvoreR(&(*raiz)->dir,Pontos,meio+1,fim,N+1);
	}
}

void InserePontos (int Pontos[TF][K], int Ponto[K], int *raio) {
	int i=0;

	srand(4);
	//srand(time(NULL));
	*raio = rand() % 10 + 1;
	Ponto[0] = rand() % 40 + 1;
	Ponto[1] = rand() % 40 + 1;
	while(i<TF) {
		Pontos[i][0] = rand() % 40 + 1; // A função rand() % N gera um número aleatório de zero até o número contido na variável N menos 1
		Pontos[i][1] = rand() % 40 + 1;
		i++;
	}
}

void DesenhaPlano (KDTree *raiz, int CI, int LI ,int CF, int LF, int N) {
	int D,i;

	if(raiz) {
		D = N%K;

		textbackground(8);
		textcolor(15);
		gotoxy(4 + 3*raiz->ponto[0],48);
		printf("%d",raiz->ponto[0]);
		gotoxy(5, 7 + (raiz->ponto[1]));
		printf("%d",raiz->ponto[1]);
		textbackground(7);
		textcolor(0);

		if (D == 0) { // eixo X
			if(raiz->ponto[0] != 1 && raiz->ponto[0] != 40) {
				gotoxy(4 + 3*raiz->ponto[0],LI);
				printf("%c",203);
				gotoxy(4 + 3*raiz->ponto[0],LF);
				printf("%c",202);
				i = LI + 1;
				while(i<LF) {
					gotoxy(4 + 3*raiz->ponto[0],i);
					printf("%c",186);
					i++;
				}
			}
			DesenhaPlano(raiz->esq,CI,LI,4 + 3*raiz->ponto[0],LF,N+1);
			DesenhaPlano(raiz->dir,4 + 3*raiz->ponto[0],LI,CF,LF,N+1);
		} else {
			if(raiz->ponto[1] != 1 && raiz->ponto[1] != 40) {
				gotoxy(CI,7 + raiz->ponto[1]);
				printf("%c",204);
				gotoxy(CF,7 + raiz->ponto[1]);
				printf("%c",185);
				i = CI + 1;
				while(i<CF) {
					gotoxy(i,7 + raiz->ponto[1]);
					printf("%c",205);
					i++;
				}
			}
			DesenhaPlano(raiz->esq,CI,LI,CF,7 + raiz->ponto[1],N+1);
			DesenhaPlano(raiz->dir,CI,7 + raiz->ponto[1],CF,LF,N+1);
		}
		gotoxy(4 + 3*raiz->ponto[0],7 + raiz->ponto[1]);
		textcolor(0);
		printf("%c (%d,%d)",207,raiz->ponto[0],raiz->ponto[1]);
		textcolor(0);
	}
}

void ExibeDados (int Ponto[K], int raio) {
	char Print[100]="";

	system("mode con cols=133 lines=52");
	Moldura(1,1,133,52,0,3);
	FundoQuadro(1,1,133,52,3);

	sprintf(Print,"PONTO DE REFERENCIA: (%d,%d)   -   RAIO: %d",Ponto[0],Ponto[1],raio);
	ExibeTexto(130,3,0,7,Print,"DADOS:");

	getch();
}

void ExibePlano (KDTree *raiz, int Ponto[K], int CI, int LI, int CF, int LF, int N) {
	Moldura(CI,LI,CF,LF,0,8);
	FundoQuadro(CI,LI,CF,LF,8);
	Sombra(CI,LI,CF,LF,0);
	Titulo(CI,CF,"PLANO:",LI);

	Moldura(CI+3,LI+1,CI+120,LI+40,0,7);
	FundoQuadro(CI+3,LI+1,CI+120,LI+40,7);

	DesenhaPlano(raiz,CI+3,LI+1,CI+120,LI+40,0);

	gotoxy(CI + Ponto[0]*3,LI + Ponto[1]);
	textcolor(12);
	printf("%c (%d,%d)",207,Ponto[0],Ponto[1]);
	textcolor(0);
}

void ExibePontos(int Pontos[TF][K]) {
	int i=0, CI, CF;
	char PLinha[100]="",SLinha[100]="",aux[10];

	while(i<TF/2) {
		sprintf(aux,("(%d,%d)"),Pontos[i][0],Pontos[i][1]);
		strcat(PLinha,aux);
		i++;
	}
	while(i<TF) {
		sprintf(aux,("(%d,%d)"),Pontos[i][0],Pontos[i][1]);
		strcat(SLinha,aux);
		i++;
	}

	if(strlen(PLinha) > strlen(SLinha)) {
		CI = 114/2-(strlen(PLinha)/2+2);
		CF = 114/2+(strlen(PLinha)/2+2);
	} else {
		CI = 114/2-(strlen(SLinha)/2+2);
		CF = 114/2+(strlen(SLinha)/2+2);
	}

	Moldura(CI,3,CF,8,0,7);
	FundoQuadro(CI,3,CF,8,7);
	Sombra(CI,3,CF,8,0);
	Titulo(CI,CF,"PONTOS GERADOS:",3);

	gotoxy((CI+CF)/2 - strlen(PLinha)/2,5);
	printf("%s",PLinha);
	gotoxy((CI+CF)/2 - strlen(SLinha)/2,6);
	printf("%s",SLinha);
}

void ExibeArvore(KDTree *raiz) {
	Moldura(5,11,110,19,0,7);
	FundoQuadro(5,11,110,19,7);
	Sombra(5,11,110,19,0);
	Titulo(5,110,"KD-TREE:",11);
	ExibeVertical(raiz,1,13);
}

int DistanciaEuclidiana (int A[2], int B[2]) {
	return sqrt((A[0]-B[0])*(A[0]-B[0]) + (A[1]-B[1])*(A[1]-B[1]));
}

void ExibeProximos (Lista *L) {
	char print[100]="", aux[10];
	while(L) {
		sprintf(aux,"(%d,%d)",L->no->ponto[0],L->no->ponto[1]);
		strcat(print,aux);
		L = L->prox;
	}
	if(!strlen(print))
		ExibeTexto(133,18,0,14,"NENHUM PONTO ENCONTRADO DENTRO DO RAIO","PONTOS DENTRO DO RAIO:");
	else
		ExibeTexto(133,18,0,14,print,"PONTOS DENTRO DO RAIO:");
}

