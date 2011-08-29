#ifndef NETCALC_H
#define NETCALC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

/* Definindo um tipo que representa um IP como um inteiro longo (32 bits) */
typedef uint32_t ip_t;

/* Definindo uma estrutura que representa uma subrede */
typedef struct tipo_rede{
	ip_t broadcast, rede, inicial, final, mascara, qtdHosts;
} rede_t;

/* Recebe uma string no formato nnn.nnn.nnn.nnn/mm e retorna um ip_t com o valor da máscara */
ip_t geraMascaraDaEntrada(char *entrada);

/* Recebe uma string no formato nnn.nnn.nnn.nnn/mm e retorna um ip_t com o valor do endereço IP */
ip_t geraIPDaEntrada(char *entrada);

/* Recebe um ip_t e retorna uma string no formato nnn.nnn.nnn.nnn/mm */
char *IPparaString(ip_t ip);

/* Dado um IP e uma máscara, calcula o endereço de broadcast da subrede */
ip_t calculaEnderecoDeBroadcast(ip_t ip, ip_t masc);

/* Dado um IP e uma máscara, calcula o endereço de rede da subrede */
ip_t calculaEnderecoDeRede(ip_t ip, ip_t masc);

#endif
