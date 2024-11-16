struct pilhaint {
	int elem;
	struct pilhaint *prox;
};
typedef struct pilhaint PilhaI;

void initI (PilhaI **P) {
	*P = NULL;
}

void pushI (PilhaI **P, int elem) {
	PilhaI *Novo = (PilhaI*)malloc(sizeof(PilhaI));

	Novo->elem = elem;
	Novo->prox = *P;
	*P = Novo;
}

void popI (PilhaI **P, int *elem) {
	PilhaI *del = *P;

	*elem = (*P)->elem;
	*P = (*P)->prox;
	free(del);
}

char isEmptyI (PilhaI *P) {
	return P == NULL;
}
