#include <semaphore.h>
#include <stdlib.h>
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

sem_t *mutex;		/* controla acesso à área de memória compartilhada com os resultados */
sem_t *contador;	/* controla a criação de processos */
void *memCompartilhada; /* ponteiro para posição de memória compartilhada */
int idMemCompartilhada; /* ID da área de memória compartilhada */

void liberaRecursos();

int main(int argc, char *argv[]){	
	int pid, descritorErro;
	unsigned int portaInicial, portaFinal;
	rede_t rede;	
	struct sigaction acaoCHLD, acaoINT;
	ip_t ip;
	host_t hostAtual;
	resultado_t *ptResult;
	time_t inicio, fim;
	
	/* Ao receber SIGCHLD, ignorar e não criar zumbis */
	sigemptyset(&acaoCHLD.sa_mask);
	acaoCHLD.sa_handler = SIG_IGN;
	acaoCHLD.sa_flags = SA_NOCLDWAIT; /* não cria zumbis, vide man sigaction */
	sigaction(SIGCHLD, &acaoCHLD, NULL);

	/* Instalando um handler para SIGINT */
	sigemptyset(&acaoINT.sa_mask);
	acaoINT.sa_handler = liberaRecursos;
	sigaction(SIGINT, &acaoINT, NULL);
	
	/* Criando região de memória compartilhada que abrigará o resultado dos escaneamentos dos filhos */
	idMemCompartilhada = shmget(ftok("/tmp", 'a'), sizeof(resultado_t), IPC_CREAT | 0660);
	memCompartilhada = shmat(idMemCompartilhada, NULL, 0);
	
	/* Criando semáforos */
	mutex = sem_open("/mutex", O_CREAT, 0666, 1);		/* garante que apenas um processo filho escreverá na área compartilhada por vez */
	contador = sem_open("/contador", O_CREAT, 0666, NUMFILHOS);	/* limita a criação de filhos */
	
	/* Obtendo dados da rede */
	rede = obtemDadosRede(argv[1]);
	imprimeDadosRede2(rede);
	
	/* Obtendo portas inicial e final */
	portaInicial = atoi(argv[2]);
	portaFinal = atoi(argv[3]);
	
	/* Se a flag de erro for igual a 0, fechar a saída de erros */
	
	/* Funcionalidade replicada no executável de escaneamento seqüencial, por portabilidade
	 * Descritores fechados deveriam permanecer fechados apos um execve(), mas isso não é garantido 
	 * Vide man execve */
	if(atoi(argv[4])==0){
		descritorErro = open("/dev/null", O_WRONLY);
		close(2); /* fechando stderr */
		dup(descritorErro);
	}
	
	/* Zerando os resultados */
	ptResult=(resultado_t *)memCompartilhada; /* ptResult aponta para área de memória compartilhada */
	inicializaResultados(ptResult);
	
	inicio = time(NULL);
	
	/* Escaneando a rede */
	for(ip=rede.inicial; ip<=rede.final; ip++){
		sem_wait(contador);
		pid=fork();
		if(!pid){		
			hostAtual=escaneiaHost(ip, portaInicial, portaFinal);
			sem_wait(mutex);			
			imprimeDadosHost(hostAtual);
			atualizaResultados(ptResult, hostAtual);			
			sem_post(mutex);
			sem_post(contador);
			exit(0);
		}
	}
	
	/* Espera até que todos os filhos tenham terminado seus escaneamentos */
	while(ptResult->qtd!=rede.qtdHosts)
		sleep(1);
	
	fim = time(NULL);
		
	imprimeResultados(ptResult);
	printf("\nTempo gasto: %.0f segundos\n", difftime(fim, inicio));
	printf("Média de tempo gasto por máquina: %.2f segundos\n", difftime(fim, inicio)/ptResult->qtd);
	
	/* Liberando recursos */
	liberaRecursos();
	
	
}

/* Fecha e apaga os semáforos, desacopla o segmento de memória compartilhada e o libera */
void liberaRecursos(){
	sem_close(contador);
	sem_close(mutex);
	sem_unlink("/contador");
	sem_unlink("/mutex");

	shmdt(memCompartilhada);	
	shmctl(idMemCompartilhada, IPC_RMID, NULL);
	exit(0);
}
