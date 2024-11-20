struct matrizesparsa {
	int linha, coluna, valor;
	struct matrizesparsa *pL, *pC;
};
typedef struct matrizesparsa MatEsp;

struct TpPont {
	int pos;
	MatEsp *Campo;
	struct TpPont *prox, *ant;
};
typedef struct TpPont Pont;

void Init (Pont **Linha, Pont **Coluna) {
	*Linha = *Coluna = NULL;
}

void Posicionar (Pont **Cabeca, int i, Pont **auxPosi) {
	Pont *novaPosi, *aux, *ant;

	if(!*Cabeca) { // Não existem linhas
		novaPosi = (Pont*)malloc(sizeof(Pont));
		novaPosi->ant = novaPosi->prox = NULL;
		novaPosi->Campo = NULL;
		novaPosi->pos = i;
		*Cabeca = *auxPosi = novaPosi;
	} else {
		aux = *Cabeca;
		while(aux && aux->pos < i) { // Ver se existe a linha
			ant = aux;
			aux = aux->prox;
		}

		if(aux && aux->pos == i) // Achou a linha
			*auxPosi = aux;
		else {
			novaPosi = (Pont*)malloc(sizeof(Pont));
			novaPosi->ant = novaPosi->prox = NULL;
			novaPosi->Campo = NULL;
			novaPosi->pos = i;

			if(aux == *Cabeca) { // Primeira posição
				(*Cabeca)->ant = novaPosi;
				novaPosi->prox = *Cabeca;
				*auxPosi = novaPosi;
				*Cabeca = novaPosi;
			} else {
				ant->prox = novaPosi;
				novaPosi->ant = ant;
				novaPosi->prox = aux;
				*auxPosi = novaPosi;
				if(aux) {
					aux->ant = novaPosi;
				}
			}
		}
	}
}

void VerificaOcupado (Pont *auxLinha, int j, MatEsp **ret) {
	MatEsp *aux;

	aux = auxLinha->Campo;
	while(aux && j > aux->coluna)
		aux = aux->pL;

	if (aux && aux->coluna == j)
		*ret = aux;
	else
		*ret = NULL;
}

void BuscaCampo (Pont *Linha, int linha, int coluna, MatEsp **Campo) {
	MatEsp *aux = NULL;
	
	while(Linha && Linha->pos < linha)
		Linha = Linha->prox;
	if(Linha && Linha->pos == linha) {
		VerificaOcupado(Linha,coluna,&aux);
		if(aux)
			*Campo = aux;
		else
			*Campo = NULL;
	} else
		*Campo = NULL;
}

void InsereMat (Pont **Linha, Pont** Coluna, int i, int j, int valor) {
	MatEsp *aux, *ant, *nova;
	Pont *auxLinha, *auxColuna;

	Posicionar(&*Linha,i,&auxLinha);
	Posicionar(&*Coluna,j,&auxColuna);
	
	// Linha
	VerificaOcupado(auxLinha,j,&aux);
	if(aux)
		aux->valor = valor;
	else {
		nova = (MatEsp*)malloc(sizeof(MatEsp));
		nova->linha = i;
		nova->coluna = j;
		nova->valor = valor;


		if(!auxLinha->Campo) { // Linha sem campo
			auxLinha->Campo = nova;
			nova->pL = NULL;
		} else {
			if(j < auxLinha->Campo->coluna) { // Menor que todos
				nova->pL = auxLinha->Campo;
				auxLinha->Campo = nova;
			} else { // Meio ou fim
				ant = auxLinha->Campo;
				aux = ant->pL;

				while(aux && j > aux->coluna) {
					ant = aux;
					aux = aux->pL;
				}
				ant->pL = nova;
				nova->pL = aux;
			}
		}
		if(!auxColuna->Campo) { // Sem coluna
			auxColuna->Campo = nova;
			nova->pC = NULL;
		} else {
			if(i < auxColuna->Campo->linha) {
				nova->pC = auxColuna->Campo;
				auxColuna->Campo = nova;
			} else {
				ant = auxColuna->Campo;
				aux = ant->pC;

				while(aux && i > aux->linha) {
					ant = aux;
					aux = aux->pC;
				}
				ant->pC = nova;
				nova->pC = aux;
			}
		}
	}
}

void ExibeMatriz (Pont *Linha) {
	int cont, exibe, i;
	Pont *aux = Linha;
	MatEsp *atual;

	while(aux) {
		atual = aux->Campo;
		cont = 0;
		while(atual) {
			if(atual->coluna > cont) {
				for(i = cont; i < atual->coluna; i++)
					printf("0");
			}
			cont = atual->coluna+1;
			printf("%c",atual->valor);
			atual = atual->pL;
		}
		printf("\n");
		aux = aux->prox;
	}
}
