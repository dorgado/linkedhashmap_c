//gcc -o lhm lhm.c -Wall
//Nome: Fellipe Sombra
//DRE: 110093323

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TAM_MAX 5

typedef struct _node{
	char* valor;
	int chave;
	struct _node* prox;
	struct _node* antr;
	struct _node* prox_geral;
	struct _node* antr_geral;
}NODE;

typedef struct _linked_list{
	NODE* primeiro;
	NODE* ultimo;
	int num_nodes;
}LINKEDLIST;

typedef struct _lhm{
	LINKEDLIST* buckets;
	int tamanho;
	int num_elementos;
	NODE* mais_antigo;
	NODE* mais_novo;
}LINKEDHASHMAP;

int hash(int chave, int tamanho){
	return chave%tamanho;
}

void LRU_put(LINKEDHASHMAP* mapa){
	int i = hash(mapa->mais_antigo->chave,mapa->tamanho);
	if(mapa->mais_antigo->antr != NULL){
		mapa->mais_antigo->antr->prox = mapa->mais_antigo->prox;
	}else{
		if(mapa->mais_antigo->prox != NULL){
			mapa->buckets[i].primeiro = mapa->mais_antigo->prox;
		}
	}
	if(mapa->mais_antigo->prox != NULL){
		mapa->mais_antigo->prox->antr = mapa->mais_antigo->antr;
	}else{
		mapa->buckets[i].ultimo = mapa->mais_antigo->antr;
	}	
	//print para debug
	printf("antigo(retirado) = %d ", mapa->mais_antigo->chave);	
	mapa->buckets[i].num_nodes--;
	if(mapa->buckets[i].num_nodes == 0){
		mapa->buckets[i].primeiro = NULL;
		mapa->buckets[i].ultimo = NULL;
	}	
	NODE* prox_antigo_geral = mapa->mais_antigo->prox_geral;
	//print para debug
	printf("| antigo_prox = %d\n",prox_antigo_geral->chave); 
	free(mapa->mais_antigo);
	mapa->mais_antigo = prox_antigo_geral;
	mapa->mais_antigo->antr_geral = NULL;

	mapa->num_elementos--;
}

void put(LINKEDHASHMAP* mapa,int chave, char* valor){
	int index = hash(chave, mapa->tamanho);
	int substituiu = 0;
	
	//verifica se já existe chave. Se ja existir substitui valor
	NODE* no = mapa->buckets[index].primeiro;
	while(no != NULL){
		if(no->chave == chave){
			no->valor = valor;
			substituiu = 1;
			break;
		}else{
			no = no->prox;
		}
	}
	
	if(substituiu == 0){	
		NODE* noNovo = (NODE*)malloc( sizeof( NODE ) );
		noNovo->valor = valor;
		noNovo->chave = chave;
		noNovo->prox = NULL;
		noNovo->antr = NULL;
		noNovo->prox_geral = NULL;
		noNovo->antr_geral = NULL;
		
		if(mapa->num_elementos == 0){
			mapa->mais_antigo = noNovo;
		}else{
			noNovo->antr_geral = mapa->mais_novo;
			mapa->mais_novo->prox_geral = noNovo;
		}
		//mapa cheio
		if(mapa->num_elementos == mapa->tamanho){
			LRU_put(mapa);
		}
	
		//se bucket estiver vazio, inicializa ele
		if(mapa->buckets[index].num_nodes == 0){
			if  ( (mapa->buckets[index].ultimo = malloc(sizeof(NODE*))) == NULL)
				return;
			if  ( (mapa->buckets[index].primeiro = malloc(sizeof(NODE*))) == NULL)
				return;
			mapa->buckets[index].primeiro = noNovo;
		}else{
			mapa->buckets[index].ultimo->prox = noNovo;
		}
		
		mapa->mais_novo = noNovo;
		mapa->buckets[index].ultimo =  noNovo;
		mapa->buckets[index].num_nodes++;
		mapa->num_elementos++;
	}	
	substituiu = 0;
}

void LRU_get(LINKEDHASHMAP* mapa, NODE* no, int chave){
	if(no->antr_geral == NULL){
		if(no->prox_geral != NULL){
			mapa->mais_antigo = no->prox_geral;
			mapa->mais_antigo->antr_geral = NULL;
			no->prox_geral->antr_geral = no->antr_geral;
			no->antr_geral = mapa->mais_novo;
			mapa->mais_novo->prox_geral = no;	
			mapa->mais_novo = no;
			mapa->mais_novo->prox_geral = NULL;
		}
	}
	else{
		if(mapa->mais_novo->chave != chave){
			no->antr_geral->prox_geral = no->prox_geral;
			no->prox_geral->antr_geral = no->antr_geral;
			no->antr_geral = mapa->mais_novo;
			mapa->mais_novo->prox_geral = no;	
			mapa->mais_novo = no;
			mapa->mais_novo->prox_geral = NULL;
		}
	}
	
	//print para debug
	if(mapa->mais_novo->antr_geral != NULL && mapa->mais_antigo->prox_geral != NULL){
		printf("mais antigo %d | prox do antigo %d | mais novo %d | anterior do novo %d\n",mapa->mais_antigo->chave,mapa->mais_antigo->prox_geral->chave,mapa->mais_novo->chave,mapa->mais_novo->antr_geral->chave);
	}
}

char* get(LINKEDHASHMAP* mapa,int chave){
	int index = hash(chave, mapa->tamanho);
	NODE* no = mapa->buckets[index].primeiro;
	printf("index: (chave: %d, tam: %d) = %d\n",chave,mapa->tamanho,index);
	while(no != NULL){
	printf("chave buscada: %d , chave no atual: %d\n",chave, no->chave);
		if(no->chave == chave){
			LRU_get(mapa, no, chave);
			return no->valor;
		}
		no = no->prox;
	}
	//print para debug
	printf("mais antigo %d | prox do antigo %d | mais novo %d | anterior do novo %d\n",mapa->mais_antigo->chave,mapa->mais_antigo->prox_geral->chave,mapa->mais_novo->chave,mapa->mais_novo->antr_geral->chave);
	return NULL;
}


LINKEDHASHMAP* inicializar_lhm(int capacidade){

	LINKEDHASHMAP* mapa = NULL;
	int i;
	if( ( mapa = (LINKEDHASHMAP*)malloc( sizeof( LINKEDHASHMAP ) ) ) == NULL ) {
		return NULL;
	}
	if( ( mapa->buckets = calloc(capacidade,sizeof(LINKEDLIST))) == NULL ) {
		return NULL;
	}
	for( i = 0; i < capacidade; i++ ) {
        mapa->buckets[i].num_nodes = 0;
	}
	mapa->tamanho = capacidade;
	mapa->num_elementos = 0;
	return mapa;
}



int main(){

	LINKEDHASHMAP* mapa = inicializar_lhm( TAM_MAX );

	put( mapa, 1, "um" );
	printf("resultado da chave 1 = %s\n\n",get( mapa ,1  ));
	put( mapa, 2, "dois" );
	printf("resultado da chave 2 = %s\n\n",get( mapa ,2  ));
	put( mapa, 22, "vinte e dois" );
	put( mapa, 3, "tres" );
	put( mapa, 3, "maisumtres" );
	put( mapa, 5, "cinco" );
	printf("resultado da chave 22 = %s\n\n",get( mapa ,22  ));
	printf("resultado da chave 3 = %s\n\n",get( mapa ,3  ));
	put( mapa, 13, "treze");
	put( mapa, 15, "quinze");
	put( mapa, 11, "onze");
	printf("resultado da chave 15 = %s\n\n",get( mapa ,15  ));
	printf("resultado da chave 13 = %s\n\n",get( mapa ,13  ));
	printf("resultado da chave 17 = %s\n\n",get( mapa ,17  ));
	put( mapa, 17, "dezessete");
	printf("resultado da chave 8 = %s\n\n",get( mapa ,8  ));
	printf("resultado da chave 11 = %s\n\n",get( mapa ,11  ));
	printf("resultado da chave 5 = %s\n\n",get( mapa ,5  ));	

	
	
	int i;
	for(i=0; i<TAM_MAX; i++){
		free(mapa->buckets[i].ultimo);
		free(mapa->buckets[i].primeiro);
	}
	free(mapa->buckets);
	free(mapa);
	return 0;
}
