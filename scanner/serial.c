#include <semaphore.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include "scan.h"

void terminarPrograma();

int main(int argc, char *argv[]){
	rede_t rede;
	struct sigaction acaoINT;
	ip_t ip;
	host_t hostAtual;
	resultado_t resultado;
	time_t inicio, fim;
	unsigned int portaFinal, portaInicial;
	int descritorErro;
	
	/* Instalando um handler para SIGINT */
	sigemptyset(&acaoINT.sa_mask);
	acaoINT.sa_handler = terminarPrograma;
	sigaction(SIGINT, &acaoINT, NULL);
	
	/* Obtendo dados da rede */
	rede = obtemDadosRede(argv[1]);
	imprimeDadosRede2(rede);
	
	/* Obtendo portas inicial e final */
	portaInicial = atoi(argv[2]);
	portaFinal = atoi(argv[3]);
	
	/* Se a flag de erro for igual a 0, fechar a saída de erros */
	
	/* Funcionalidade replicada no executável de escaneamento paralelo, por portabilidade
	 * Descritores fechados deveriam permanecer fechados apos um execve(), mas isso não é garantido 
	 * Vide man execve */
	if(atoi(argv[4])==0){
		descritorErro = open("/dev/null", O_WRONLY);
		close(2); /* fechando stderr */
		dup(descritorErro);
	}
	
	/* Zerando os resultados */	
	inicializaResultados(&resultado);
	
	inicio = time(NULL);
	
	/* Escaneando a rede */
	for(ip=rede.inicial; ip<=rede.final; ip++){
		hostAtual=escaneiaHost(ip, portaInicial, portaFinal);
		imprimeDadosHost(hostAtual);
		atualizaResultados(&resultado, hostAtual);
	}
	
	fim = time(NULL);
	
	imprimeResultados(&resultado);
	
	printf("\nTempo gasto: %.0f segundos\n", difftime(fim, inicio));
	printf("Média de tempo gasto por máquina: %.2f segundos\n", difftime(fim, inicio)/resultado.qtd);
	
}

void terminarPrograma(){
	exit(0);
}
