#ifndef SCAN_H
#define SCAN_H

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
#include "netcalc.h"

#define TRUE 1
#define FALSE 0
#define DESC 2

#define NUMFILHOS 19
#define TIMEOUT 1

/* Definindo as que serão escaneadas para determinar se uma máquina é ativa (será escaneada) ou inativa */
static const int portasPrincipais[] = {21, 22, 37, 53, 80, 111, 113, 139, 443, 3389};
static const int qtdPortasPrincipais = 10;

/* Estrutura que representa um host */
typedef struct tipo_host{
	char fqdn[256];
	ip_t ip;
	char status;			/* TRUE, FALSE ou DESC */
	char portasAbertas[65536];	/* TRUE ou FALSE */
	int qtdPortasAbertas;
	float percentualPortasAbertas;
	double tempoGasto;
} host_t;

/* Estrutura que armazena os dados usados para exibir o relatório final */
typedef struct tipo_resultado{
	unsigned long ativas, inativas, desconhecidas, qtd;
	unsigned long portas[65536];
} resultado_t;

/* Handler do connect() com timeout. Apenas retorna, interrompendo o connect() */
static void connect_alarm(int signo);

/* Retorna uma estrutura que representa os dados de um host com todos os campos zerados */
host_t criaDadosHost();

/* Obtém um FQDN dado um IP */
char *resolveNome(ip_t ip);

/* Varre o host de IP ip de portaInicial a portaFinal */
host_t escaneiaHost(ip_t ip, int portaInicial, int portaFinal);

/* Recebe uma estrutura que representa um host e imprime seus dados */
void imprimeDadosHost(host_t host);

/* Retorna TRUE para porta ativa, FALSE para inativa e -1 para erro */
int ehPortaAtiva(ip_t ip, int porta);

/* Retorna TRUE para máquina ativa, FALSE para máquina inativa */
int ehHostAtivo(ip_t ip);

/* Recebe uma estrutura que representa a rede e imprime seus dados */
void imprimeDadosRede2(rede_t rede);

/* Recebe uma string no formato nnn.nnn.nnn.nnn/mm e retorna uma estrutura que representa uma rede */
rede_t obtemDadosRede(char *ip_string);

/* Connect com timeout
 * Adaptado de proposta do Stevens no volume 1 de UNIX Network Programming */
int connect_timeout(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen, int nsec);

void inicializaResultados(resultado_t *resultado);

/* Recebe o _endereço_ de uma estrutura que representa os resultados do escaneamento e imprime os seus dados */
void imprimeResultados(const resultado_t *resultado);

/* Função que adiciona os dados de uma estrutura que representa um host em uma estrutura que representa os resultados do escaneamento */
void atualizaResultados(resultado_t *resultado, host_t host);

#endif

