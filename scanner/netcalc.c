#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "netcalc.h"

ip_t geraMascaraDaEntrada(char *entrada){
	int qtdBits = atoi(strstr(entrada, "/")+1);
	int qtdBitsASetarComo1 = 32 - qtdBits;
	ip_t retorno = pow(2, qtdBitsASetarComo1)-1;	
	retorno = ~retorno;	
	return retorno;
	
}

ip_t geraIPDaEntrada(char *entrada){	
	int byte1, byte2, byte3, byte4;
	char *ipSemNumDeBits;	
	ip_t retorno;
	char buffer[19];
	strcpy(buffer, entrada);
	ipSemNumDeBits = strtok(buffer, "/");	
	byte1 = atoi(strtok(ipSemNumDeBits, "."));    // mais significativo
	byte2 = atoi(strtok(NULL, "."));
	byte3 = atoi(strtok(NULL, "."));
	byte4 = atoi(strtok(NULL, "."));              //menos significativo
	retorno = byte4;
	retorno += (byte3 << 8);
	retorno += (byte2 << 16);
	retorno += (byte1 << 24);
	return retorno;
	
}

char *IPparaString(ip_t ip){
	int byte1, byte2, byte3, byte4;
	char string_ip[300], retorno[300];
	byte1 = ip >> 24;
	byte2 = (ip >> 16) & 255;
	byte3 = (ip >> 8) & 255;
	byte4 = ip & 255;
	sprintf(string_ip, "%u.%u.%u.%u", byte1,byte2,byte3,byte4);
	return strcpy(retorno, string_ip);
}

ip_t calculaEnderecoDeBroadcast(ip_t ip, ip_t masc){
	return ip | ~masc;
}

ip_t calculaEnderecoDeRede(ip_t ip, ip_t masc){
	return ip & masc;
}
