#include "netcalc.h"

int main(int argc, char *argv[]){
	ip_t masc, ip, bcast, rede;

	ip = geraIPDaEntrada(argv[1]);
	masc = geraMascaraDaEntrada(argv[1]);

	rede = calculaEnderecoDeRede(ip, masc);
	bcast = calculaEnderecoDeBroadcast(ip, masc);

	printf("REDE: %s\n", IPparaString(rede));
	printf("BCST: %s\n", IPparaString(bcast));

	printf("HOST: %s - ", IPparaString(rede+1));
	printf("%s \n", IPparaString(bcast-1));
	return 0;
}
