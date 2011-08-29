/* Funções de auxílio aos trabalhos de TCD
 * 
 * Guilherme Magalhães Gall
 */
 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>
#include <regex.h>

/* Constante com regex que casa um IP.
 *
 * FIXME Deixa passar alguns IPs inválidos.
 */
#define REGEXIP "^[0-9]{1,3}[[:punct:]][0-9]{1,3}[[:punct:]][0-9]{1,3}[[:punct:]][0-9]{1,3}$"
#define TRUE 1
#define FALSE 0

/* Lista estática de descritores abertos pelas funções de comunicação com a rede
 * 
 * É criada para que seja possível liberar os recursos de uma só vez com uma chamada a
 * fechaDescritores()
 */
static int descritores[100], qtd;

/* Prepara um servidor TCP genérico para uma dada porta
 *
 * Retorna um descritor de arquivos
 */

int preparaServidorTCP(int porta){
	struct sockaddr_in enderecoLocal;
	int descritorLocal;
	
	descritorLocal=socket(AF_INET, SOCK_STREAM, 0);
	memset(&enderecoLocal, '\0', sizeof(struct sockaddr));
	enderecoLocal.sin_family=AF_INET;
	enderecoLocal.sin_port=htons(porta);
	enderecoLocal.sin_addr.s_addr=htonl(INADDR_ANY);
	bind(descritorLocal, (struct sockaddr *)&enderecoLocal, sizeof(struct sockaddr));
	listen(descritorLocal, 1);
	
	descritores[qtd++]=descritorLocal;
	
	return descritorLocal;
}

/* Aceita conexão de um cliente. Recebe o descritor do socket ouvinte como parâmetro
 *
 * Retorna um descritor de arquivos, o descritor do socket do cliente que se conectou.
 */

int aceitaConexao(int descritor){
	int tamanho, descritorRemoto;
	struct sockaddr_in enderecoRemoto;
	tamanho = sizeof(struct sockaddr);
	descritorRemoto = accept(descritor, (struct sockaddr *)&enderecoRemoto, &tamanho);
	
	descritores[qtd++]=descritorRemoto;
	
	return descritorRemoto;
}

/* Aceita conexão de um cliente. Recebe o descritor do socket ouvinte e um ponteiro para endereco como parâmetro
 * Escreve os dados de endereçamento do cliente na estrutura apontada por 'endereco'
 *
 * Retorna um descritor de arquivos, o descritor do socket do cliente que se conectou.
 */

int aceitaConexao2(int descritor, struct sockaddr_in *endereco){
	int tamanho, descritorRemoto;
	tamanho = sizeof(struct sockaddr);
	descritorRemoto = accept(descritor, (struct sockaddr *)endereco, &tamanho);
	
	descritores[qtd++]=descritorRemoto;
	
	return descritorRemoto;
}

/* Percorre a lista estática de descritores abertos e os fecha */

void fechaDescritores(void){
	int i;
	for(i=0; i<qtd; i++){
		close(descritores[i]);
	}	
}

/* Envia a hora por um socket passado por parâmetro
 *
 * Retorna a hora enviada
 */

time_t enviaHora(int descritor){
	time_t hora;
	hora=time(NULL);	
	send(descritor, (char *)&hora, sizeof(hora), 0);	
	return hora;
}

/* Recebe uma hora por um socket passado por parâmetro 
 *
 * Retorna a hora recebida
 */

time_t recebeHora(int descritor){
	time_t hora;
	recv(descritor, &hora, sizeof(time_t), 0);
	return hora;
}

/* Conecta a um servidor TCP, dados o IP e a porta. Uso em clientes.
 *
 * Retorna um descritor de arquivos
 */

int conectaAServidorTCP(const char *ip, int porta){
	int descritorRemoto;
	struct sockaddr_in enderecoRemoto;
	
	descritorRemoto = socket(AF_INET, SOCK_STREAM, 0);
	
	memset(&enderecoRemoto, '\0', sizeof(struct sockaddr));
	enderecoRemoto.sin_family=AF_INET;
	enderecoRemoto.sin_port=htons(porta);
	inet_aton(ip, &enderecoRemoto.sin_addr);
	
	connect(descritorRemoto, (struct sockaddr *)&enderecoRemoto, sizeof(struct sockaddr));
	
	descritores[qtd++] = descritorRemoto;
	
	return descritorRemoto;
}

/* Valida uma string representando um IP na notação decimal pontuada
 *
 * Retorna TRUE caso o IP seja válido ou FALSE caso não seja
 * FIXME Não é perfeita, considera alguns IPs inválidos como válidos
 */

int validaIP(const char *ip){
	int retorno;
	regex_t expressaoRegular;
	
	if(regcomp(&expressaoRegular, REGEXIP, REG_EXTENDED|REG_NOSUB) != 0){
		fprintf(stderr, "Erro na composição da regex");
		exit(1);
	}
	
	if((regexec(&expressaoRegular, ip, 0, NULL, 0)) == 0){
		regfree(&expressaoRegular);
		retorno = TRUE;
	}		
	else{
		regfree(&expressaoRegular);
		retorno = FALSE;
	}
	
	return retorno;
}

/* Valida uma porta
 *
 * Retorna TRUE ou FALSE
 */

int validaPorta(int porta){
	int retorno=FALSE;
	if(porta>=0 && porta<=65536)
		retorno=TRUE;
	return retorno;
}

