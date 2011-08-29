#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include "scan.h"
#include "netcalc.h"

static void connect_alarm(int signo){
	return;
}

host_t criaDadosHost(){
	host_t retorno;
	int i;
	
	strcpy((char *)&retorno.fqdn, "Falha");
	retorno.ip=0;
	retorno.status=TRUE;
	for(i=0; i<=65535; i++)
		retorno.portasAbertas[i]=DESC;
	retorno.qtdPortasAbertas=0;
	retorno.percentualPortasAbertas=0.0;
	
	return retorno;
}

char *resolveNome(ip_t ip){
	struct hostent *infoDNS;
	struct in_addr end;
	
	end.s_addr=htonl(ip);
	infoDNS = gethostbyaddr(&end, sizeof(struct in_addr), AF_INET);
	if(infoDNS!=NULL)
		return infoDNS->h_name;
	else
		return "???";
}

host_t escaneiaHost(ip_t ip, int portaInicial, int portaFinal){
	int portaAtual;
	host_t retorno;
	time_t horaInicial;
	char *nome;
	
	retorno=criaDadosHost();
	retorno.ip=ip;
	horaInicial=time(NULL);
	nome=resolveNome(ip);
	strcpy((char *)&retorno.fqdn, nome);
	if(strcmp(nome, "???")==0){
		retorno.status=DESC;
		retorno.tempoGasto = difftime(time(NULL),horaInicial);
		return retorno;
	}
	
	if(!ehHostAtivo(ip)){
		retorno.status=FALSE;
		retorno.tempoGasto = difftime(time(NULL), horaInicial);
		return retorno;
	}
	
	
	else{
		for(portaAtual=portaInicial; portaAtual<=portaFinal; portaAtual++){
			if(ehPortaAtiva(ip, portaAtual)){
				retorno.portasAbertas[portaAtual]=TRUE;
				retorno.qtdPortasAbertas++;
			} else{
				retorno.portasAbertas[portaAtual]=FALSE;
			}
		}
		retorno.percentualPortasAbertas=100.0*retorno.qtdPortasAbertas/(portaFinal-portaInicial+1);
	}
	
	retorno.tempoGasto = difftime(time(NULL), horaInicial);
	return retorno;
		
}

void imprimeDadosHost(host_t host){
	int i;
	switch(host.status){
		case TRUE:
		printf("FQDN:\t\t\t%s\n", host.fqdn);
		printf("IP:\t\t\t%s\n", IPparaString(host.ip));
		printf("Status:\t\t\tativo\n");
		printf("Tempo gasto:\t\t%.0f segundos\n", host.tempoGasto);
		printf("Qtd. portas abertas:\t%d (%.2f%% do intervalo pedido/%.5f do total de portas possíveis em um host)\n", host.qtdPortasAbertas, host.percentualPortasAbertas, 100.0*host.qtdPortasAbertas/65535);
		printf("Portas abertas:\t\t");
		for(i=0; i<=65535; i++){
			if(host.portasAbertas[i]==TRUE)
				printf("%d ", i);
		}
		puts("");
		fprintf(stderr, "Portas fechadas:\t");
		for(i=0; i<=65535; i++){
			if(host.portasAbertas[i]==FALSE)
				fprintf(stderr, "%d ", i);
		}
		puts("");
		puts("\n---------\n");
		break;
		
		case FALSE:
		printf("FQDN:\t\t\t%s\n", host.fqdn);
		printf("IP:\t\t\t%s\n", IPparaString(host.ip));
		printf("Status:\t\t\tinativo\n");
		printf("Tempo gasto:\t\t%.0f segundos\n", host.tempoGasto);
		puts("\n---------\n");
		break;
		
		case DESC:
		printf("FQDN:\t\t\t%s\n", host.fqdn);
		printf("IP:\t\t\t%s\n", IPparaString(host.ip));
		printf("Status:\t\t\tdesconhecido\n");
		printf("Tempo gasto:\t\t%d segundos\n", host.tempoGasto);
		puts("\n---------\n");
		break;
	}

}


/* Retorna TRUE para porta ativa, FALSE para inativa e -1 para erro */
int ehPortaAtiva(ip_t ip, int porta){
	int descritorAlvo, retorno;
	struct sockaddr_in enderecoAlvo;
	
	if((descritorAlvo = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("Socket:");
		retorno = -1;
	}
	
	memset(&enderecoAlvo, '\0', sizeof(struct sockaddr));
	enderecoAlvo.sin_family=AF_INET;
	enderecoAlvo.sin_port=htons(porta);
	enderecoAlvo.sin_addr.s_addr=htonl(ip);
	
	if((connect_timeout(descritorAlvo, (struct sockaddr *)&enderecoAlvo, sizeof(struct sockaddr), TIMEOUT)) == -1) 
		retorno=FALSE;
	else
		retorno=TRUE;
		
	if(close(descritorAlvo)==-1){
		perror("Erro ao fechar socket");
	}
	
	return retorno;
}

/* Retorna TRUE para máquina ativa, FALSE para máquina inativa */
int ehHostAtivo(ip_t ip){
	int i;
	
	for(i=0; i<qtdPortasPrincipais; i++){
		if(ehPortaAtiva(ip, portasPrincipais[i])==TRUE)
			return TRUE;
	}
	
	return FALSE;
}



void imprimeDadosRede2(rede_t rede){

	
	printf("Dados da rede\n\n");
	printf("Endereço de broadcast: %s\n", IPparaString(rede.broadcast));
	printf("Endereço de rede: %s\n", IPparaString(rede.rede));
	printf("Endereço de inicial: %s\n", IPparaString(rede.inicial));
	printf("Endereço de final: %s\n", IPparaString(rede.final));
	printf("Máscara: %s\n", IPparaString(rede.mascara));
	printf("Quantidade total de hosts: %d\n", rede.qtdHosts);
	printf("\n---------\n\n");
}

rede_t obtemDadosRede(char *ip_string){
	ip_t ip, broadcast, rede, inicial, final, mascara, qtd;
	rede_t retorno;
	
	mascara = geraMascaraDaEntrada(ip_string);
	ip = geraIPDaEntrada(ip_string);
	
	broadcast = calculaEnderecoDeBroadcast(ip, mascara);
	rede = calculaEnderecoDeRede(ip, mascara);
	final = broadcast-1;
	inicial = rede+1;
	qtd = final-rede;
	
	retorno.mascara = mascara;
	
	
	retorno.broadcast = broadcast;
	retorno.rede = rede;
	retorno.final = final;
	retorno.inicial = inicial;
	retorno.qtdHosts = qtd;
	
	return retorno;
}

/* Connect com timeout
 * Adaptado de proposta do Stevens no volume 1 de UNIX Network Programming */
 
int connect_timeout(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen, int nsec){
	int n;
	struct sigaction newaction;
	
	sigemptyset(&newaction.sa_mask);
	newaction.sa_handler=connect_alarm;
	newaction.sa_flags=SA_RESETHAND;
	sigaction(SIGALRM, &newaction, NULL);
	
	alarm(nsec);
	if( (n=connect(sockfd, serv_addr, addrlen)) <0 ){
		if(errno==EINTR) errno=ETIMEDOUT;
	}
	alarm(0);
	return(n);
}

void inicializaResultados(resultado_t *resultado){
	int i;
	resultado->qtd=0;
	resultado->ativas=0;
	resultado->inativas=0;
	resultado->desconhecidas=0;
	for(i=0; i<=65535; i++)
		resultado->portas[i]=0;
}

void imprimeResultados(const resultado_t *resultado){
	int i;
	unsigned long somaPortasAbertas=0;
	printf("Resultado da análise:\n\n");
	printf("\nTotais:\n");
	printf("\tMáquinas:\t\t%d\n", resultado->qtd);
	printf("\tMáquinas ativas:\t%d (%.2f%%)\n", resultado->ativas, 100.0*resultado->ativas/resultado->qtd);
	printf("\tMáquinas inativas:\t%d (%.2f%%)\n", resultado->inativas, 100.0*resultado->inativas/resultado->qtd);
	printf("\tMáquinas sem FQDN:\t%d (%.2f%%)\n", resultado->desconhecidas, 100.0*resultado->desconhecidas/resultado->qtd);
	printf("\nPortas abertas:\n");
	
	for(i=0; i<=65535; i++){
		if(resultado->portas[i]!=0){
			printf("\t%d em %d host(s)\n", i,resultado->portas[i]);
			somaPortasAbertas+=resultado->portas[i];
		}
		
	}
	
	
	printf("\nMédias:\n");
	printf("\tPortas abertas por máquina ativa:\t\t%.2f\n", (float)somaPortasAbertas/resultado->ativas);
	printf("\tPortas abertas por intervalo de escaneamento:\t%.2f\n", (float)somaPortasAbertas/resultado->qtd);
}

void atualizaResultados(resultado_t *resultado, host_t host){
	int i;
	switch(host.status){
		case TRUE:
		resultado->ativas++;
		break;
		
		case FALSE:
		resultado->inativas++;
		break;
		
		case DESC:
		resultado->desconhecidas++;
		break;
	}
	
	for(i=0; i<=65535; i++){
		if(host.portasAbertas[i]==TRUE)
			resultado->portas[i]++;
	}
	resultado->qtd++;
}


