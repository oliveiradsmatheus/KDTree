struct pilha {
	KDTree *elem;
	struct pilha *prox;
};
typedef struct pilha Pilha;

void init (Pilha **P) {
	*P = NULL;
}

void push (Pilha **P, KDTree *no) {
	Pilha *Novo = (Pilha*)malloc(sizeof(Pilha));

	Novo->elem = no;
	Novo->prox = *P;
	*P = Novo;
}

void pop (Pilha **P, KDTree **no) {
	Pilha *del = *P;

	*no = (*P)->elem;
	*P = (*P)->prox;
	free(del);
}

char isEmpty (Pilha *P) {
	return P == NULL;
}

void PreOrdem (KDTree *raiz) {
	Pilha *P;
	gotoxy(1,1);
	init(&P);
	push(&P,raiz);
	while(!isEmpty(P)) {
		pop(&P,&raiz);
		if(Folha(raiz))
			printf("(%d,%d)",raiz->ponto[0],raiz->ponto[1]);
		else {
			if(raiz->dir)
				push(&P,raiz->dir);
			if(raiz->esq)
				push(&P,raiz->esq);
		}
	}
}
