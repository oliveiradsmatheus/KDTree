#define K 2
#define TF 20

struct kdtree {
	int ponto[K];
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
	int i;
	KDTree *NC = (KDTree*)malloc(sizeof(KDTree));

	NC->esq = NC->dir = NULL;
	for(i=0; i<K; i++)
		NC->ponto[i] = Ponto[i];

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
		printf("(");
		for(i=0; i<K; i++) {
			printf("%d",raiz->ponto[i]);
			if(i+1 == K)
				printf(")");
			else
				printf(",");
		}
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
		printf("(");
		for(i=0; i<K; i++) {
			printf("%d",raiz->ponto[i]);
			if(i+1 == K)
				printf(")");
			else
				printf(",");
		}
		ExibeHorizontal(raiz->esq);
		n--;
	}
}

void Ordena(int Pontos[TF][K], int D, int ini, int fim) {
	int i,j,k,aux[K],aux2 = fim;

	for(i=ini; i<fim-1; i++) {
		for(j=i; j<fim; j++) {
			if(Pontos[i][D] > Pontos[j][D])
				for(k=0; k<K; k++) {
					aux[k] = Pontos[i][k];
					Pontos[i][k] = Pontos[j][k];
					Pontos[j][k] = aux[k];
				}
		}
	}
}

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

void InserePontos (int Pontos[TF][K], int Ponto[K], int *raio) { // KD
	int i,j;
	
	srand(time(NULL));
	*raio = rand() % 10 + 1;
	for(i=0; i<K; i++)
		Ponto[i] = rand() % 40 + 1;
	for(i=0; i<TF; i++)
		for(j=0; j<K; j++)
			Pontos[i][j] = rand() % 40 + 1; // A função rand() % N gera um número aleatório de zero até o número contido na variável N menos 1
}

void DesenhaPlano (KDTree *raiz, int CI, int LI ,int CF, int LF, int N, Pont **Linha, Pont **Coluna) { // 2D
	MatEsp *Campo;
	int i;

	if(raiz) {
		textbackground(8);
		textcolor(15);
		gotoxy(4 + 3*raiz->ponto[0],48);
		printf("%d",raiz->ponto[0]);
		gotoxy(5, 48 - (raiz->ponto[1]));
		printf("%d",raiz->ponto[1]);
		textbackground(7);
		textcolor(0);

		if (N%K == 0) { // eixo X
			if(raiz->ponto[0] != 1 && raiz->ponto[0] != 40) {
				BuscaCampo(*Linha,48-LI,raiz->ponto[0],&Campo); // Campo do Inicio
				if(!Campo) {
					InsereMat(&*Linha,&*Coluna,48-LI,raiz->ponto[0],203);
					gotoxy(4 + 3*raiz->ponto[0],LI);
					printf("%c",203);
				} else {
					switch(Campo->valor) {
						case 205:
							Campo->valor = 203;
							break;
						case 202:
							Campo->valor = 206;
							break;
					}
					gotoxy(4 + 3*raiz->ponto[0],LI);
					printf("%c",Campo->valor);
				}
				BuscaCampo(*Linha,48-LF,raiz->ponto[0],&Campo);
				if(!Campo) {
					InsereMat(&*Linha,&*Coluna,48-LF,raiz->ponto[0],202);
					gotoxy(4 + 3*raiz->ponto[0],LF);
					printf("%c",202);
				} else {
					switch(Campo->valor) {
						case 205:
							Campo->valor = 202;
							break;
						case 203:
							Campo->valor = 206;
							break;
					}
					gotoxy(4 + 3*raiz->ponto[0],LF);
					printf("%c",Campo->valor);
				}
				i = LI + 1;
				while(i<LF) {
					BuscaCampo(*Linha,48-i,raiz->ponto[0],&Campo);
					if(!Campo)
						InsereMat(&*Linha,&*Coluna,48-i,raiz->ponto[0],186);
					gotoxy(4 + 3*raiz->ponto[0],i);
					printf("%c",186);
					i++;
				}
			}
			DesenhaPlano(raiz->esq,CI,LI,4 + 3*raiz->ponto[0],LF,N+1,&*Linha,&*Coluna);
			DesenhaPlano(raiz->dir,4 + 3*raiz->ponto[0],LI,CF,LF,N+1,&*Linha,&*Coluna);
		} else {
			if(raiz->ponto[1] != 1 && raiz->ponto[1] != 40) {
				BuscaCampo(*Linha,raiz->ponto[1],CI/3-1,&Campo); // Campo do Inicio
				if(!Campo) {
					InsereMat(&*Linha,&*Coluna,raiz->ponto[1],CI/3-1,204);
					gotoxy(CI,48 - raiz->ponto[1]);
					printf("%c",204);
				} else {
					switch(Campo->valor) {
						case 186:
							Campo->valor = 204;
							break;
						case 185:
							Campo->valor = 206;
							break;
					}
					gotoxy(CI,48 - raiz->ponto[1]);
					printf("%c",Campo->valor);
				}
				BuscaCampo(*Linha,raiz->ponto[1],CF/3-1,&Campo);
				if(!Campo) {
					InsereMat(&*Linha,&*Coluna,raiz->ponto[1],CF/3-1,185);
					gotoxy(CF,48 - raiz->ponto[1]);
					printf("%c",185);
				} else {
					switch(Campo->valor) {
						case 186:
							Campo->valor = 185;
							break;
						case 204:
							Campo->valor = 206;
							break;
					}
					gotoxy(CF,48 - raiz->ponto[1]);
					printf("%c",Campo->valor);
				}
				i = CI + 1;
				while(i<CF) {
					BuscaCampo(*Linha,raiz->ponto[1],i/3-1,&Campo);
					if(!Campo)
						InsereMat(&*Linha,&*Coluna,raiz->ponto[1],i/3-1,205);
					gotoxy(i,48 - raiz->ponto[1]);
					printf("%c",205);
					i++;
				}
			}
			DesenhaPlano(raiz->esq,CI,48 - raiz->ponto[1],CF,LF,N+1,&*Linha,&*Coluna);
			DesenhaPlano(raiz->dir,CI,LI,CF,48 - raiz->ponto[1],N+1,&*Linha,&*Coluna);
		}
		gotoxy(4 + 3*raiz->ponto[0],48 - raiz->ponto[1]);
		textcolor(0);
		printf("%c (%d,%d)",207,raiz->ponto[0],raiz->ponto[1]);
		textcolor(0);
	}
}

void ExibeDados (int Ponto[K], int raio) {
	int i;
	char Print[300]="", aux[50];

	system("mode con cols=133 lines=52");
	Moldura(1,1,133,52,0,3);
	FundoQuadro(1,1,133,52,3);

	strcat(Print,"PONTO DE REFERENCIA: (");
	for(i=0; i<K; i++) {
		sprintf(aux,"%d",Ponto[i]);
		strcat(Print,aux);
		if(i+1 == K)
			strcat(Print,")");
		else
			strcat(Print,",");
	}
	sprintf(aux,"      -      RAIO: %d",raio);
	strcat(Print,aux);
	ExibeTexto(130,3,0,7,Print,"DADOS:");
}

void InsereMolduraMatriz (Pont **Linha, Pont **Coluna, int CI, int LI, int CF, int LF) {
	int i;
	
	InsereMat(&*Linha,&*Coluna,LI,CI,200);
	InsereMat(&*Linha,&*Coluna,LI,CF,188);
	InsereMat(&*Linha,&*Coluna,LF,CI,201);
	InsereMat(&*Linha,&*Coluna,LF,CF,187);
	for(i=CI+1; i<CF; i++) {
		InsereMat(&*Linha,&*Coluna,LI,i,205);
		InsereMat(&*Linha,&*Coluna,LF,i,205);
	}
	for(i=LI+1; i<LF; i++) {
		InsereMat(&*Linha,&*Coluna,i,CI,186);
		InsereMat(&*Linha,&*Coluna,i,CF,186);
	}
}

void ExibePlano (KDTree *raiz, int Ponto[K], int CI, int LI, int CF, int LF, int N) {
	Pont *Linha, *Coluna;
	Init(&Linha,&Coluna);

	Moldura(CI,LI,CF,LF,0,8);
	FundoQuadro(CI,LI,CF,LF,8);
	Sombra(CI,LI,CF,LF,0);
	Titulo(CI,CF,"PLANO:",LI);

	Moldura(CI+3,LI+1,CI+120,LI+40,0,7);
	FundoQuadro(CI+3,LI+1,CI+120,LI+40,7);
	
	InsereMolduraMatriz(&Linha,&Coluna,1,1,40,40);
	DesenhaPlano(raiz,CI+3,LI+1,CI+120,LI+40,0,&Linha,&Coluna);
	
	/*system("cls"); // Função de teste para exibição da Matriz Esparsa
	ExibeMatriz(Linha);
	getch();*/	
	
	gotoxy(CI + Ponto[0]*3,48 - Ponto[1]);
	textcolor(12);
	printf("%c (%d,%d)",207,Ponto[0],Ponto[1]);
	textcolor(0);
}

void ExibePontos(int Pontos[TF][K]) {
	int i=0,j, CI, CF;
	char PLinha[200]="",SLinha[200]="",aux[50];

	while(i<TF/2) {
		strcat(PLinha,"(");
		for(j=0; j<K; j++) {
			sprintf(aux,"%d",Pontos[i][j]);
			strcat(PLinha,aux);
			if(j+1 == K)
				strcat(PLinha,")");
			else
				strcat(PLinha,",");
		}
		i++;
	}
	while(i<TF) {
		strcat(SLinha,"(");
		for(j=0; j<K; j++) {
			sprintf(aux,"%d",Pontos[i][j]);
			strcat(SLinha,aux);
			if(j+1 == K)
				strcat(SLinha,")");
			else
				strcat(SLinha,",");
		}
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

double DistanciaEuclidiana (int A[K], int B[K]) {
	int i;
	double dif=0;

	for(i=0; i<K; i++)
		dif += pow((A[i]-B[i]),2);

	return sqrt(dif);
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

