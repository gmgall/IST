#include <regex.h>
#include "validation.h"

/* Valida uma string representando um IP na notação decimal pontuada
 *
 * Retorna TRUE caso o IP seja válido ou FALSE caso não seja
 * FIXME Não é perfeita, considera alguns IPs inválidos como válidos
 */

int validaSubRede(const char *ip){
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
	if(porta>=0 && porta<=65535)
		retorno=TRUE;
	return retorno;
}
