#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashingfunctions.h"

/*
	* Usando o metódo Resto com base no tamanho da tabela para criar o Hash
	e salvando o primeiro registro na tabela e encadeando suas colisões
*/

#define MAX  180
#define M  10000

typedef struct pessoa{
	int linha;
	char endereco[50];
	int id;	
	char datanasci[30];
	char nome[40];
	char email[40];
	char celular[13];
}Pessoa;

typedef struct registro{
	struct pessoa * per;
	char status ;
	int quant_colisoes;
}Registro;


Registro * criarregistro(Pessoa *pessoa){
	/* Essa função é responsavel por inicializar um Registro contendo uma Pessoa e retorna o Registro da pessoa criado */
	Registro * novo = (Registro*)malloc(sizeof(Registro));
	novo->per=pessoa;
	novo->status='O';
	novo->quant_colisoes=0;
	return novo;
}

int quant_total_de_colisoes=0;
int quant_total_de_1sRegistro=0;

//O vetor tabela é do tipo Registro e será responsavel por amarzenar os registros com os dados da pessoa
Registro *tabela[M];


void verificar_espacos_entre_registros(){
	/* essa funcão ira falar como esta distribuido os dados na tabela.
	Sera contabilizado a quantidade de locais Livres, Ocupados e colisoes a cada Mil registros
	*/
	int i;
	int P0a1,P1a2,P2a3,P3a4,P4a5,P5a6,P6a7,P7a8,P8a9,P9a10; // contabiliza se esta livre 
	int P0c1,P1c2,P2c3,P3c4,P4c5,P5c6,P6c7,P7c8,P8c9,P9c10; // contabiliza as colisoes

	P0a1=P1a2=P2a3=P3a4=P4a5=P5a6=P6a7=P7a8=P8a9=P9a10=0;
	P0c1=P1c2=P2c3=P3c4=P4c5=P5c6=P6c7=P7c8=P8c9=P9c10=0;
	for (i = 0; i < M; i++){
		if(!tabela[i]) continue;
		if(tabela[i]->status == 'O' || tabela[i]->status == 'D'){
			if(i>=0 && i<=1000){
				P0a1++;
				P0c1 += tabela[i]->quant_colisoes;
			}
			if(i>1000 && i<=2000){
				P1a2++;
				P1c2 += tabela[i]->quant_colisoes;
			}
			if(i>2000 && i<=3000){
				P2a3++;
				P2c3 += tabela[i]->quant_colisoes;
			}
			if(i>3000 && i<=4000){
				P3a4++;
				P3c4 += tabela[i]->quant_colisoes;
			}
			if(i>4000 && i<=5000){
				P4a5++;
				P4c5 += tabela[i]->quant_colisoes;
			}
			if(i>5000 && i<=6000){
				P5a6++;
				P5c6 += tabela[i]->quant_colisoes;
			}
			if(i>6000 && i<=7000){
				P6a7++;
				P6c7 += tabela[i]->quant_colisoes;
			}
			if(i>7000 && i<=8000){
				P7a8++;
				P7c8 += tabela[i]->quant_colisoes;
			}
			if(i>8000 && i<=9000){
				P8a9++;
				P8c9 += tabela[i]->quant_colisoes;
			}
			if(i>9000 && i<=10000){
				P9a10++;
				P9c10 += tabela[i]->quant_colisoes;
			}
		}
	}
	printf("\n De %d a %d existem %d registros e %d colisoes \n ",0,1000,P0a1,P0c1);
	printf("\n De %d a %d existem %d registros e %d colisoes \n ",1000,2000,P1a2,P1c2);
	printf("\n De %d a %d existem %d registros e %d colisoes \n ",2000,3000,P2a3,P2c3);
	printf("\n De %d a %d existem %d registros e %d colisoes \n ",3000,4000,P3a4,P3c4);
	printf("\n De %d a %d existem %d registros e %d colisoes \n ",4000,5000,P4a5,P4c5);
	printf("\n De %d a %d existem %d registros e %d colisoes \n ",5000,6000,P5a6,P5c6);
	printf("\n De %d a %d existem %d registros e %d colisoes \n ",6000,7000,P6a7,P6c7);
	printf("\n De %d a %d existem %d registros e %d colisoes \n ",7000,8000,P7a8,P7c8);
	printf("\n De %d a %d existem %d registros e %d colisoes \n ",8000,9000,P8a9,P8c9);
	printf("\n De %d a %d existem %d registros e %d colisoes \n ",9000,10000,P9a10,P9c10);
}


Registro * inserirregistro(Registro * registro, Pessoa *pessoa){
	/* Essa função é responsavel por verificar a situação do Registro e contabilizar a quantidade de colições e de Primeiros Registros,
	caso o registro ainda não exista na tabela ele retorna com um novo registro criado*/
	if(!registro){
		quant_total_de_1sRegistro++;
		return criarregistro(pessoa);
	}
	if(registro->status == 'O'){
		registro->quant_colisoes++;
		quant_total_de_colisoes++;
	}
	return registro;
}

void printPessoa(Pessoa *p){
	printf("----------------------------------\n");
	//printf("linha %d\n",p->linha );
	printf("Endereço: %s\n",p->endereco );
	printf("ID: %d\n",p->id );
	printf("Data de nascimento: %s\n",p->datanasci );
	printf("Nome: %s\n",p->nome );
	printf("Email: %s\n",p->email );
	printf("Celular: %s\n",p->celular );
	printf("----------------------------------\n");
}

Pessoa * separar_dados(char*linha){
	/*Essa função é responsavel por separar os dados atravez de uma String vinda do paramentro LINHA,
	retornando uma estrutura Pessoa com seus dados*/
	char * token;
	Pessoa *current = (Pessoa*)malloc(sizeof(Pessoa));
	
	token = strtok(linha,",");
	current->linha = atoi(token);

	token = strtok(NULL,",");
	strncpy(current->endereco, token,50);

	token = strtok(NULL,",");
	current->id = atoi(token);
	
	token = strtok(NULL,",");
	strncpy(current->datanasci, token,30);

	token = strtok(NULL,",");
	strncpy(current->nome, token,40);

	token = strtok(NULL,",");
	strncpy(current->email, token,40);

	token = strtok(NULL,"\n");
	strncpy(current->celular ,token,13);
	
	return current;

}
void freetabela(){
	/* Função responsavel por liberar a memória ocupada pela tabela */
	int i;
	for (i = 0; i < M; i++){
		if(!tabela[i])continue;
		else{
			free(tabela[i]->per);
			free(tabela[i]);
		}
	}
}

void processar_arquivo(){
	/* A função Processar_arquivo é responsavel por abrir o arquivo de dados, 
	ler os dados linha por linha, receber uma Estrutura Pessoa, receber o Hash e adicionar na tabela */
	char * enderfile = "dados-500000-lin.csv";
	FILE * arq = fopen(enderfile,"r");
	if(arq == NULL){
		printf("Erro ao carregar o arquivo\n");
		return;
	}

	char linha[MAX];
	Pessoa * pessoa = NULL;
	int hash,i = 0;
	
	fgets(linha, sizeof linha, arq);
	while((i<M) &&(fgets(linha, sizeof linha, arq) != NULL)){
		pessoa = separar_dados(linha);
		hash = hashRestoTam(pessoa->id,M);
		tabela[hash] = inserirregistro(tabela[hash],pessoa);
		i++;
	}

	fclose(arq);
	return;
}
void procurar_registro(int id){
	/* Dado o ID pelo usuario, a função irá recriar o Hash do id e assim mostrar os dados do ID caso exista*/
	int hash = hashRestoTam(id,M);
	if(!tabela[hash] || tabela[hash]->status == 'D' )
		printf("Registro com o id %d não foi encontrado.",id);
	else
		printPessoa(tabela[hash]->per);
}
void remover(int id){
	/* Dado o ID pelo usuario, a função irá recriar o Hash do id e assim atualizar os status para deletado caso exista*/
	int hash = hashRestoTam(id,M);
	if(!tabela[hash] || tabela[hash]->status == 'D' )
		printf("Registro com o id %d não foi encontrado.\n",id);
	else{
		tabela[hash]->status = 'D';
		printf("\nRegistro com ID %d deletado.\n",id);
	}
}


int main(int argc, char const *argv[]){

	int op;

	while(1){
		printf("(1) Processar Arquivo\n(2) ler Registro\n(3) Remover registro\n(0) Para sair\n");
		scanf("%d",&op);
		if(op==1){
			processar_arquivo();
			printf("\nFoi processado %d Registros sendo %d Registrados e %d colisoes\n\n",M,quant_total_de_1sRegistro,quant_total_de_colisoes);
		}
		else if(op==2){
			printf("\nDigite o id \n");
			scanf("%d",&op);
			procurar_registro(op);
		}
		else if(op==3){
			printf("\nDigite o id \n");
			scanf("%d",&op);
			remover(op);
		}
		else break;
	}

	// Alguns informacoes extras sobre a tabela
	verificar_espacos_entre_registros();
	
	freetabela();
	return 0;
}

