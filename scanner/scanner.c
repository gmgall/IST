#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "validation.c"

int main(int argc, char *argv[]){

	char buffer[5];
	int numBits;
	
	if(argc != 6){
		printf("Forneça o número de argumentos correto.\n\n");
		printf("Uso: %s [subrede] [porta inicial] [porta final] [flag porta fechada] [flag paralelo]\n\n", argv[0]);
		printf("Exemplo de uso: %s 192.168.0.0/24 20 80 0 1\nEscaneia os hosts da rede 192.168.0.0/24, da porta 20 a 80 de forma paralela, sem exibir as portas fechadas.\n");
		exit(2);
	}
	
	if(!validaSubRede(argv[1])){
		printf("Subrede em formato inválido. Use formato endereco/mascara.\nEx: 192.168.0.0/24\n\n");
		printf("Uso: %s [subrede] [porta inicial] [porta final] [flag porta fechada] [flag paralelo]\n\n", argv[0]);
		printf("Exemplo de uso: %s 192.168.0.0/24 20 80 0 1\nEscaneia os hosts da rede 192.168.0.0/24, da porta 20 a 80 de forma paralela, sem exibir as portas fechadas.\n");
		exit(2);
	}
	
	if( !validaPorta(atoi(argv[2])) || !validaPorta(atoi(argv[3]))){
		printf("Uma das portas informadas está fora do intervalo possível (0-65535).\n\n");
		printf("Uso: %s [subrede] [porta inicial] [porta final] [flag porta fechada] [flag paralelo]\n\n", argv[0]);
		printf("Exemplo de uso: %s 192.168.0.0/24 20 80 0 1\nEscaneia os hosts da rede 192.168.0.0/24, da porta 20 a 80 de forma paralela, sem exibir as portas fechadas.\n");
		exit(2);
	}
	
	if(atoi(argv[2]) > atoi(argv[3])){
		printf("A porta inicial deve ser menor que a porta final.\n\n");
		printf("Uso: %s [subrede] [porta inicial] [porta final] [flag porta fechada] [flag paralelo]\n\n", argv[0]);
		printf("Exemplo de uso: %s 192.168.0.0/24 20 80 0 1\nEscaneia os hosts da rede 192.168.0.0/24, da porta 20 a 80 de forma paralela, sem exibir as portas fechadas.\n");
		exit(2);
	}
	
	if(!(atoi(argv[4]) == 0 || atoi(argv[4]) == 1 && atoi(argv[5]) == 0 || atoi(argv[5]) == 1)){
		printf("Valor inválido para as flags informadas (0 ou 1).\n\n");
		printf("Uso: %s [subrede] [porta inicial] [porta final] [flag porta fechada] [flag paralelo]\n\n", argv[0]);
		printf("Exemplo de uso: %s 192.168.0.0/24 20 80 0 1\nEscaneia os hosts da rede 192.168.0.0/24, da porta 20 a 80 de forma paralela, sem exibir as portas fechadas.\n");
		exit(2);
	}
	
	strcpy(buffer, argv[1]);
	strtok(buffer, "/");
	numBits=atoi(strtok(NULL, "/"));
	if( numBits>30 || numBits<8 ){
		printf("Entre com um número de bits válido para a máscara (8 a 30)\n\n");
		printf("31 e 32 bits de máscara produzem redes sem endereços para os hosts\n");
		exit(2);
	}
	
	if(strcmp(argv[5], "0")==0){
		if(execl("serial", "serial", argv[1], argv[2], argv[3], argv[4], (char *)0)==-1){
			fprintf(stderr, "\n\nErro ao rodar o executável 'serial'. Preciso estar no mesmo diretório que os executáveis 'serial' e 'paralelo'\n\n");
			exit(255);
		}
	}
	else{
		if(execl("paralelo", "paralelo", argv[1], argv[2], argv[3], argv[4], (char *)0)==-1){
			fprintf(stderr, "\n\nErro ao rodar o executável 'paralelo'. Preciso estar no mesmo diretório que os executáveis 'serial' e 'paralelo'\n\n");
			exit(255);
		}
	}
}
