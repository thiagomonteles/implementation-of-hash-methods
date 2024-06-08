#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashingfunctions_series_hash_1r.h"

/*
	* Esse programa ira ultilizar o metodo de DOBRA para criar o HASH e armazenado
	ultilizando o metodo de encadeamento externo em que o 1o registro é considerado na tabela
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

	char status ;
	int proximo;
}Registro;

void inicializar_tabela(){
	/* Essa função é responsabel por incializar a tabela com quantidade
	 maxima de Primeiro registro que ela pode ter (em uma situação que não teve colisão),
	 e indicar posteriomente se ela está 'L' livre */
	FILE * datahash = fopen("data.bin","wb+");
	if(!datahash){
		printf("erro ao abrir o arquivo datahash ao incializar a tabela \n");
		return;
	}

	int i;
	Registro aux;
	aux.status = 'L'; //Indica que ainda não houve registro
	aux.proximo = -1; //-1 é semelhante a NULL (indica o fim da "Lista")

	for (i = 0; i < M; i++){
        fseek(datahash,0,SEEK_END);
		fwrite(&aux,sizeof(Registro),1,datahash);
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

int quant_total_de_colisoes=0;
int quant_total_de_Raux=0;
int numero_de_operacoes=0;


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

char ler_status(int hash, FILE * datahash){
	/*Essa função irá auxiliar a decisão para verificar se o status do registro indicado pelo hash é 'L'livre , 'O'ocupado ou 'D'deletado*/
   	Registro aux;
    int tamR = sizeof(Registro);
    fseek(datahash,(tamR*hash),SEEK_SET);
	fread(&aux,tamR,1,datahash);
    return aux.status;
}

void gravar_1s_registro(int hash,FILE*datahash, Registro * RegAux){
	/* Essa função irá gravar a primeira ocorrencia de registro no Hash indicado */

	int tamR = sizeof(Registro);
	quant_total_de_Raux++;
	//Muda o ponteiro para o começo do Registro indicado
	fseek(datahash,tamR*hash,SEEK_SET);
	fwrite(RegAux,tamR,1,datahash);
}

void gravar_colisao(int hash,FILE*datahash, Registro * RegAux){
	/* Essa função irá gravar as colisões no Hash indicado */

	int tamR = sizeof(Registro);
	quant_total_de_colisoes++;

	//Muda o ponteiro para o final do arquivo
	fseek(datahash,0,SEEK_END);
	int final = ftell(datahash); //salva a posicao do novo registro
	fwrite(RegAux,tamR,1,datahash);

	//Muda o ponteiro para o 1s_registro que foi inserido e muda a variavel Proximo para indicar sua proxima colisao  (Semelhante a função PUSH em lista encadeada)
	fseek(datahash,(tamR*hash) + (tamR-4),SEEK_SET);
	fwrite(&final,sizeof(int),1,datahash);
}
void processar_arquivo(){
	/* A função Processar_arquivo é responsavel por abrir o arquivo de dados, 
	ler os dados linha por linha, receber uma Estrutura Registro contendo os dados da pessoa, receber o Hash e adicionar na tabela no arquivo */
   
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

	char linha[MAX],status;
    Registro  * RegAux;
	int hash,i = 0;
	
	fgets(linha, sizeof linha, arq);
	while((i<M) &&(fgets(linha, sizeof linha, arq) != NULL)){
	    RegAux = criarregistro(linha);
		hash = hashDobra(RegAux->id,8,4);
		status = ler_status(hash,datahash);

		if(status == 'L') gravar_1s_registro(hash,datahash,RegAux);	
		else if(status == 'O') gravar_colisao(hash,datahash,RegAux);

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
	int hash = hashDobra(id,8,4);
	int tamR = sizeof(Registro);
	int local_atual;

    FILE * datahash = fopen("data.bin","rb+");
	if(datahash==NULL){
		printf("erro ao abrir o arquivo binario \n");
		return ;
	}

	fseek(datahash,tamR*hash,SEEK_SET);
	fread(&Raux,tamR,1,datahash);
	numero_de_operacoes++;

	//verifico se o Primeiro registro é o indicado e se ele está deletado
	if(Raux.status == 'D' && Raux.id == id){
		printf("\nRegistro com ID %d deletado \n\n",id);
		fclose(datahash);
		return;
	}

	if(Raux.id == id) printPessoa(&Raux);
	else{
		// se o primeiro registro não for o que eu procuro, eu procuro o id desejado nas colisoes
		if(Raux.proximo != -1){
			RegColisoes.proximo = Raux.proximo;

			while(RegColisoes.proximo != -1 ){
				fseek(datahash,RegColisoes.proximo,SEEK_SET);
				local_atual = ftell(datahash);
				fread(&RegColisoes,tamR,1,datahash);
				
				numero_de_operacoes++;
				
				if( RegColisoes.status == 'D' && RegColisoes.id == id){
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

	Registro Raux;
	Registro RegColisoes;
	int hash = hashDobra(id,8,4);
    int tamR = sizeof(Registro);

	fseek(datahash,tamR*hash,SEEK_SET);
	fread(&Raux,tamR,1,datahash);

	//verifica se existe um Primeiro registro
	if(Raux.status=='O'){
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
	else printf("Hash (%d) sem registro\n",hash);

	fclose(datahash);
	return;
}
void deletar_registro(int id){
	/* Essa função irá para o local indicado do registro atravez do Hash, verifica se o registro é o indicado pelo ID
	se sim pular para parte onde indica o Status do registro e alterar ele para 'D' deletado, 
	Caso contrario começa a buscar o ID indicado nas colisões. */
	Registro Raux,RegColisoes;
	int tamR = sizeof(Registro);
	int hash = hashDobra(id,8,4);
	int local_atual;
	char att_status = 'D';

	FILE * datahash = fopen("data.bin","rb+");
	if(datahash==NULL){
		printf("erro ao abrir o arquivo binario \n");
		return ;
	}

	fseek(datahash,tamR*hash,SEEK_SET);
	fread(&Raux,tamR,1,datahash);

	if(Raux.id == id){ 
		//Se o primeiro registro for o que quero eu altero eu vou no local onde fica o Status e altero para Deletado
		fseek(datahash,(tamR*hash)+184,SEEK_SET);
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
		printf("\n(1) Processar Arquivo\n(2) ler Registro pelo ID\n(3) ler todos os Registros do HASH\n(4) Remover registro\n(5)Para procurar usando 1000 chaves aleatorias\n(0) Para sair\n");
		scanf("%d",&op);
		if(op==1){
			processar_arquivo();
			printf("\nFoi processado %d Registros sendo %d Registrados e %d colisoes\n\n",M,quant_total_de_Raux,quant_total_de_colisoes);
		}
		else if(op==2){
			printf("\nDigite o ID que deseja encontrar\n");
			printf("ID: ");
			scanf("%d",&op);
			procurar_registro( op);
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
		}
		else if(op==5){
			simular_1000_chaves();
		}
		else break;
	}
	
	return 0;
}

