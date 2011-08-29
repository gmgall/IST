Calculadora IP
==============

Dado um IP/mascara, imprime:

* o endereço da subrede;
* o endereço de broadcast;
* endereços disponíveis para os hosts.

Compilando
----------

`gcc main.c netcalc.c -o calcip -lm`

Exemplo de uso
--------------

	$ ./calcip 10.0.4.10/22
	REDE: 10.0.4.0
	BCST: 10.0.7.255
	HOST: 10.0.4.1 - 10.0.7.254
