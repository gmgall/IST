/* Servidor de hora
 * 
 * Guilherme Magalhães Gall
 */

#include <stdio.h>
#include <signal.h>
#include "tcd.c"

void ctrl_c(){
	fechaDescritores();
	exit(0);
}

int main(int argc, char *argv[]){
	int descritorLocal, descritorRemoto;
	struct sockaddr_in enderecoRemoto;
	time_t hora;
	
	/* Desvia o fluxo de execução para a função ctrl_c caso o programa receba SIGINT */
	signal(SIGINT, ctrl_c);
	
	/* Validando parâmetros de linha de comando 
	 * 
	 * strtol é equivalente a atoi quando usada dessa maneira (vide man atoi)
	 */
	if(argc != 2 || !validaPorta((int)strtol(argv[1], (char **)NULL, 10))){
		fprintf(stderr, "Uso: %s <porta>\n", argv[0]);
		exit(1);
	}

	descritorLocal = preparaServidorTCP((int)strtol(argv[1], (char **)NULL, 10));	
	
	printf("IP Cliente\tPorta\tTimestamp\n");
	while(1){
		descritorRemoto = aceitaConexao2(descritorLocal, &enderecoRemoto);
		printf("%s\t%d\t%ld\n", inet_ntoa(enderecoRemoto.sin_addr), ntohs(enderecoRemoto.sin_port), enviaHora(descritorRemoto));
	}
}


