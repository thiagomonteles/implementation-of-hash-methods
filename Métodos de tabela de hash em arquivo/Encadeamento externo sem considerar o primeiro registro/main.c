#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashingfunctions.h"

/*
	* Esse programa ira ultilizar o metodo de DOBRA para criar o HASH e armazenar
	ultilizando o método de encadeamento externo em que o nenhum registro é considerado na tabela;
 */

#define MAX  190
#define M  10000 //Numero de usuarios

typedef struct registro{
	int linha;
	char endereco[50];
	int id;	
	char datanasci[30];
	char nome[40];
	char email[40];
	char celular[14];

	char status ; // O para ocupado e D para deletado
	int proximo; //-1 indica que não existe proximo
}Registro;

int quant_total_de_colisoes=0;
int quant_primeiro_registro=0;
int numero_de_operacoes = 0;

void inicializar_tabela(){
	/* Essa função é responsavel por incializar a tabela com quantidade
	 maxima de Hash que a tabela pode receber e indicar incialmente que não existe registro cadastrado  */
	FILE * datahash = fopen("data.bin","wb+");
	if(!datahash){
		printf("erro ao abrir o arquivo datahash ao incializar a tabela \n");
		return;
	}
	int i, proximo = -1;
	
	for (i = 0; i < M; i++){
        fseek(datahash,0,SEEK_END);
		fwrite(&proximo,sizeof(int),1,datahash);
	}
	fclose(datahash);
}
void separar_dados(Registro * current,char*linha){
	/*Essa função é responsavel por separar os dados atravez de uma String vinda do paramentro LINHA,
	retornando os dados da pessoa registrado no Registro current passado pelo parametro*/
	char * token;
	
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
	strncpy(current->celular ,token,14);

}

Registro * criarregistro(char*linha){
	/* Essa função é responsavel por inicializar um Registro contendo uma Pessoa e retorna o Registro da pessoa criado */
	Registro * novo = (Registro*)malloc(sizeof(Registro));
	separar_dados(novo,linha);
	novo->proximo = -1;
	novo->status='O';
	return novo;
}


void printPessoa(Registro *p){
	//Irá mostrar os dados do Registro indicado pelo paramentro
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

void gravar_registro(int hash,FILE*datahash, Registro * RegAux){
	/* Essa função irá gravar o registro no Hash indicado criando uma lista encadeada por meio de um PUSH */

	int tamR = sizeof(Registro);
	int tamInt = sizeof(int);
	int proximo;
	
	// Vou no local do hash na tabela, salvo o local para onde aponta e salvo ele no novo registro
	fseek(datahash,tamInt*hash,SEEK_SET);
	fread(&proximo,tamInt,1,datahash);
	RegAux->proximo = proximo;

	if(proximo == -1) quant_primeiro_registro++; // se for o primeiro registro a gravar
	else quant_total_de_colisoes++;

	//Muda o ponteiro para o final do arquivo
	fseek(datahash,0,SEEK_END);
	int final = ftell(datahash); //salva a posicao do novo registro
	fwrite(RegAux,tamR,1,datahash);

	//Muda o ponteiro para o local do hash na tabela e altero para indicar o primeiro registro
	fseek(datahash,tamInt*hash,SEEK_SET);
	fwrite(&final,tamInt,1,datahash);
}
void processar_arquivo(){
	/* A função Processar_arquivo é responsavel por abrir o arquivo de dados, 
	ler os dados linha por linha, receber uma Estrutura Registro contendo os dados da pessoa, receber o Hash e adicionar no arquivo */
   
    inicializar_tabela();
	
	FILE * arq = fopen("dados-500000-lin.csv","r");
	if(!arq){
		printf("Erro ao carregar o arquivo\n");
		return;
	}
    FILE * datahash = fopen("data.bin","r+b");
    if(!datahash){
        printf("erro ao abrir o datahash\n");
        return;
    }

	char linha[MAX];
    Registro  * RegAux;
	int hash,i = 0;
	
	fgets(linha, sizeof linha, arq);
	while((i<M) &&(fgets(linha, sizeof linha, arq) != NULL)){
	    RegAux = criarregistro(linha); //crio o registro
		hash = hashDobra(RegAux->id,8,4); //crio o hash
		gravar_registro(hash,datahash,RegAux); //gravo no arquivo
        free(RegAux);   
		i++;
	}

	fclose(datahash);
	fclose(arq);
	return;
}
void procurar_registro(int id){
	/* Dado o ID pelo usuario, a função irá recriar o Hash do id e assim mostrar os dados do ID caso exista e não esteja deledado*/
   
    Registro Raux,RegColisoes;
	int tamR = sizeof(Registro);
	int tamInt = sizeof(int);
	int hash = hashDobra(id,8,4);
	int local_atual, situacao;

    FILE * datahash = fopen("data.bin","rb+");
	if(datahash==NULL){
		printf("erro ao abrir o arquivo binario \n");
		return ;
	}

	fseek(datahash,tamInt*hash,SEEK_SET);
	fread(&situacao,tamInt,1,datahash);
	numero_de_operacoes++;

	//verifico se existe registro no hash indicado pelo ID
	if(situacao == -1){
		printf("\n Nao existe registro no Hash feito atravez do ID desejado \n\n");
		fclose(datahash);
		return;
	}

	fseek(datahash,situacao,SEEK_SET);
	fread(&Raux,tamR,1,datahash);
	numero_de_operacoes++;
	if(Raux.id == id){
		if(Raux.status == 'D'){
			printf("\nRegistro com ID %d deletado \n\n",id);
			fclose(datahash);
			return;
		}else printPessoa(&Raux);

	}else{
		// se não for o que eu procuro, eu procuro o id desejado nas colisoes
		if(Raux.proximo != -1){
			RegColisoes.proximo = Raux.proximo;

			while(RegColisoes.proximo != -1 ){
				fseek(datahash,RegColisoes.proximo,SEEK_SET);
				local_atual = ftell(datahash);
				fread(&RegColisoes,tamR,1,datahash);

				numero_de_operacoes++;
				
				if( RegColisoes.status == 'D' && RegColisoes.id == id){ //Se for o id que eu quero e ele estiver deletado
					printf("\nRegistro com ID %d foi deletado\n",id);
					break;
				}else if(RegColisoes.status == 'O' && RegColisoes.id == id){
					printPessoa(&RegColisoes);
					break;					
				}
				
				if(RegColisoes.proximo == -1)
					printf("\nRegistro com ID %d não existe\n",id);
			}
		}else printf("Registro com ID %d não encontrado\n",id);
	}
	fclose(datahash);
	return;
}
void procurar_registros_hash(int id){
	/* Essa função ira recriar o hash atravez do id indicado pelo usuario 
	e da um Print em todos os Registros indicado no HASH (incluindo colisoes caso exista).
	OBS. Ele irá mostrar inclusive registros deletados */
    FILE * datahash = fopen("data.bin","rb+");
	if(datahash==NULL){
		printf("erro ao abrir o arquivo binario \n");
		return ;
	}

	Registro Raux, RegColisoes;
	int hash = hashDobra(id,8,4);
	int tamInt = sizeof(int);
    int tamR = sizeof(Registro);
	int situacao;

	fseek(datahash,tamInt*hash,SEEK_SET);
	fread(&situacao,tamInt,1,datahash);
	//verifico se existe registro no hash indicado pelo ID
	if(situacao == -1){
		printf("\n Nao existe registro no Hash desejado \n\n");
		fclose(datahash);
		return;
	}else{
		fseek(datahash,situacao,SEEK_SET);
		fread(&Raux,tamR,1,datahash);
		printPessoa(&Raux);

		if(Raux.proximo != -1){// -1 indica que não há proximo
			printf("Existem colisões nesse hash! \n");
			RegColisoes.proximo = Raux.proximo;
			
			while(RegColisoes.proximo != -1){
				fseek(datahash,RegColisoes.proximo,SEEK_SET);
				fread(&RegColisoes,tamR,1,datahash);
				printPessoa(&RegColisoes);
			}
		}
	}

	fclose(datahash);
	return;
}
void deletar_registro(int id){
	/* Essa função irá para o local indicado do registro atravez do Hash, verifica se o registro é o indicado pelo ID
	se sim pular para parte onde indica o Status do registro e alterar ele para 'D' deletado, 
	Caso contrario começa a buscar o ID indicado nas colisões. */
	Registro Raux,RegColisoes;
	int tamR = sizeof(Registro);
	int tamInt = sizeof(int);
	int hash = hashDobra(id,8,4);
	int local_atual,situacao;
	char att_status = 'D';

	FILE * datahash = fopen("data.bin","rb+");
	if(datahash==NULL){
		printf("erro ao abrir o arquivo binario \n");
		return ;
	}

	fseek(datahash,tamInt*hash,SEEK_SET);
	fread(&situacao,tamInt,1,datahash);
	//verifico se existe registro no hash indicado pelo ID
	if(situacao == -1){
		printf("\n Nao existe registro no Hash feito atravez do ID desejado \n\n");
		fclose(datahash);
		return;
	}

	fseek(datahash,situacao,SEEK_SET);
	fread(&Raux,tamR,1,datahash);
	if(Raux.id == id){ 
		//Se o primeiro registro for o que quero eu altero eu vou no local onde fica o Status e altero para Deletado
		fseek(datahash,situacao+184,SEEK_SET);
		fwrite(&att_status,sizeof(char),1,datahash);
		printf("Registro com ID: %d deletado",id);
	}else{
		// se nao eu procuro o registro do id desejado nas colisoes
		if(Raux.proximo != -1){
			RegColisoes.proximo = Raux.proximo;
			
			while(RegColisoes.proximo != -1 ){	
				fseek(datahash,RegColisoes.proximo,SEEK_SET);
				local_atual = ftell(datahash);
				fread(&RegColisoes,tamR,1,datahash);

				if(RegColisoes.id == id){
					fseek(datahash,local_atual+184,SEEK_SET); //local_atual+184 é a quantidade de bytes para pular na parte onde indica o status
					fwrite(&att_status,sizeof(char),1,datahash); //altero para Deletado
					printf("Registro com ID %d deletado\n",id);
					break;
				}
				if(RegColisoes.proximo == -1){ //se não existe um proximo
					printf("Registro com ID %d não encontrado\n",id);
					break;
				}
			}
		}else printf("Registro com ID %d não encontrado\n",id);
	}
	fclose(datahash);
}

void simular_1000_chaves(){
	/* Essa função salva os 15mil primeiros IDs que estão no arquivo csv em um vetor, gera um numero de 0 a 15 mil e esse numero
	é usado pra pegar o id e mandar fazer uma procura.
	obs. Foram contabilizados toda vez que ele acessou um local no arquivo binario com os registros salvos */

	/*--------------- Pego os 15 mil Id iniciais e salvo em um vetor de int */
	FILE * arq = fopen("dados-500000-lin.csv","r");
	if(!arq){
		printf("Erro ao carregar o arquivo\n");
		return;
	}
	char linha[MAX];
    Registro  * RegAux;
	int tabela_de_chaves[15000],i;
	
	fgets(linha, sizeof linha, arq);
	while((i<15000) &&(fgets(linha, sizeof linha, arq) != NULL)){
	    RegAux = criarregistro(linha);
		tabela_de_chaves[i] = RegAux->id;
		i++;
		free(RegAux);
	}
	fclose(arq);
	/*---------------------------------------------------------------- */
	/* comeco a simulacao gerando numeros de 0 a 15000 mil e acessando o id referente e mandondo para uma
	procura */
	int linha_random,total_oper=0;

	for(i=0 ; i < 1000 ; i++){
				numero_de_operacoes=0;
				linha_random = rand()%15000;
				procurar_registro(tabela_de_chaves[linha_random]);
				total_oper+=numero_de_operacoes;
			}
		printf("a media de operacoes foi %f por registro",((float)total_oper/1000));
}

int main(int argc, char const *argv[]){
	//Para auxiliar na busca pelo registro ou deletar, o registro de ID 64674981 é uma colisão
	int op;

	while(1){
		printf("\n(1) Processar Arquivo\n(2) ler Registro pelo ID\n(3) ler todos os Registros do HASH\n(4) Remover registro\n(5)Para procurar usando 1000 chaves aleatorias\\n(0) Para sair\n");
		scanf("%d",&op);
		if(op==1){
			processar_arquivo();
			printf("\nFoi processado %d Registros sendo %d Registrados e %d colisoes\n\n",M,quant_primeiro_registro,quant_total_de_colisoes);
		}
		else if(op==2){
			printf("\nDigite o ID que deseja encontrar\n");
			printf("ID: ");
			scanf("%d",&op);
			procurar_registro(op);
		}else if(op==3){
			printf("\nDigite o HASH que deseja encontrar\n");
			printf("HASH: ");
			scanf("%d",&op);
			procurar_registros_hash(hashDobra(op,8,4));
		}
		else if(op==4){
			printf("\nDigite o ID para ser deletado\n");
			scanf("%d",&op);
			deletar_registro(op);
		}else if(op==5){
			simular_1000_chaves();
		}else break;
	}
	
	return 0;
}

