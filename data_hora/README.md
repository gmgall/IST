Cliente e servidor de data e hora
=================================

A [página do professor](http://lrodrigo.lncc.br/) possui o [enunciado desse trabalho](http://lrodrigo.lncc.br/index.php/TCD:Cliente_e_Servidor_de_Hora_-_2008.2).

Compilando
----------

* Cliente

	`gcc cliente.c -o cliente`
	
* Servidor

	`gcc servidor.c -o servidor`

Exemplo de uso
--------------

###Servidor###
	$ ./servidor 8888
	IP Cliente	Porta	Timestamp
	127.0.0.1	59910	1314632888

###Cliente###
	$ ./cliente 127.0.0.1 8888
	Timestamp recebido de 127.0.0.1:8888 --> 1314632888 = Mon Aug 29 12:48:08 2011
	Timestamp local --> 1314632888 = Mon Aug 29 12:48:08 2011

	Diferença (remoto-local) entre data/hora local e remota (em segundos): 0
	A hora não foi alterada. Você é root?
