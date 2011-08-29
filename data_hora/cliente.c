/* Cliente que recebe a hora do servidor, mostra a diferença entre a 
 * hora local e a do servidor e altera a data/hora local de acordo com a
 * do servidor se o usuário possuir privilégios.
 * 
 * Guilherme Magalhães Gall
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tcd.c"

int validaParametros(char *ip, int porta){
	int retorno=FALSE;
	if(validaIP(ip) && validaPorta(porta))
		retorno=TRUE;
	return retorno;
}

int main(int argc, char *argv[]){
	int descritor;
	time_t horaLocal, horaRemota;

	/* FIXME A validação está atrelada a uma regex que considera alguns valores inválidos
	 * como válidos
	 */
	if(argc != 3 || !validaParametros(argv[1], (int)strtol(argv[2], (char **)NULL, 10))){
		fprintf(stderr, "Uso: %s <ip> <porta>\n", argv[0]);
		exit(1);
	}
	
	descritor = conectaAServidorTCP(argv[1], (int)strtol(argv[2], (char **)NULL, 10));
	horaLocal = time(NULL);	
	horaRemota=recebeHora(descritor);
	/* ctime imprime uma variável time_t em forma amigável, como na saída do comando date */
	printf("Timestamp recebido de %s:%d --> %ld = %s", argv[1], atoi(argv[2]), horaRemota, ctime(&horaRemota));
	printf("Timestamp local --> %ld = %s\n", horaLocal, ctime(&horaLocal));
	/* difftime retorna a diferença entre dois timestamps (time_t) em segundos */
	printf("Diferença (remoto-local) entre data/hora local e remota (em segundos): %.0f\n", difftime(horaRemota, horaLocal));
	/* stime recebe um ponteiro para time_t e atualiza o relógio do sistema com seu valor */
	if(stime(&horaRemota)!=0){
		fprintf(stderr, "A hora não foi alterada. Você é root?\n");
		fechaDescritores();
		exit(1);
	}
	fechaDescritores();
	
}
