//gcc -o lhm lhm.c -Wall

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct _node{
	char* value;
	int chave;
	struct _node* prox;
}NODE;

typedef struct _linked_list{
	NODE* primeiro;
	NODE* ultimo;
	int node_count;
}LINKEDLIST;

typedef struct LINKEDLIST bucketao;

typedef struct _hml{
	LINKEDLIST* buckets;
	int tamanho; //m
	int elements_count; //n
}LINKEDHASHMAP;

int hash(int chave, int tamanho){
	printf("hash: (chave: %d, tam: %d) = %d\n",chave,tamanho,chave%tamanho);
	return chave%tamanho;
}

void put(LINKEDHASHMAP* mapa,int chave, char* valor){

	int index = hash(chave, mapa->tamanho);
	
	//verifica se  o mapa ta full.. se tiver por enqnt só diz que ta full e da return

	//se nao tiver adiciona

	NODE* noNovo = (NODE*)malloc( sizeof( NODE ) );
	noNovo->value = valor;
	noNovo->chave = chave;
	noNovo->prox = NULL;
	printf( "1" );
	if(mapa->elements_count == 0){
		printf( "2" );
		mapa->buckets[index].primeiro =  noNovo;
		printf( "3" );
	}else{
		printf( "4" );
		mapa->buckets[index].ultimo->prox = noNovo;
		printf("5");
	}
	printf("6");
	mapa->buckets[index].ultimo =  noNovo;
	printf("7\n");
	mapa->elements_count++; 
}


char* get(LINKEDHASHMAP* mapa,int chave){
	int bucket_idx = hash(chave, mapa->tamanho);
	NODE* no = mapa->buckets[bucket_idx].primeiro;
	printf("a %d\n",no->chave);
	while(no != NULL){
		printf("b %d\n",no->chave);
		if(no->chave == chave){
			return no->value;
		}
		no=no->prox;
		printf("c %d\n",no->chave);
	}
	return NULL;
}


LINKEDHASHMAP* inicializar_lhm(int capacidade){
	
	LINKEDHASHMAP* mapa = NULL;
	int i;
	if( ( mapa = (LINKEDHASHMAP*)malloc( sizeof( LINKEDHASHMAP ) ) ) == NULL ) {
		return NULL;
	}

	if( ( mapa->buckets = malloc(capacidade*sizeof(bucketao*))) == NULL ) {
		return NULL;
	}
	
	for( i = 0; i < capacidade; i++ ) {
		if  ( (mapa->buckets[i].ultimo = malloc(sizeof(NODE*))) == NULL)
			return NULL;
		if  ( (mapa->buckets[i].primeiro = malloc(sizeof(NODE*))) == NULL)
			return NULL;
	}
	mapa->tamanho= capacidade;
	mapa->elements_count = 0;

	return mapa;
	
}
// m = n / fator_de_carga
//fator_de_carga = #chaves/#buckets

//antes de dar free buckets .. tem que ir em cada bucket e free em cada lista.



int main(){

	LINKEDHASHMAP* mapa = inicializar_lhm( 10 );

	put( mapa, 1, "um" );
	put( mapa, 2, "dois" );

	printf( "%s\n", get( mapa ,1  ) );
	printf( "%s\n", get( mapa ,2  ) );


return 0;
}