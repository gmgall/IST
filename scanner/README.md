Scanner de Rede em C
====================

A [página do professor](http://lrodrigo.lncc.br/) possui o [enunciado desse trabalho](http://lrodrigo.lncc.br/index.php/TCD:Scanner_de_Rede_em_C_-_2008.2).

O interessante desse trabalho é que eu fui um pouco além do pedido e usei:

* semáforos;
* memória compartilhada;
* tratamento de sinais;
* expressões regulares

Os headers possuem comentários explicando o que cada função faz. Dê uma olhada nos `*.h`.

Pré compilação
--------------

No arquivo `scan.h` é possível alterar a quantidade de filhos simultâneos mudando o valor da macro `NUMFILHOS`. Ela foi definida como 19 para atender os requisitos do trabalho (no máximo 20 processos rodando ao mesmo tempo), mas obtive bom desempenho com mais filhos.

Outro valor que pode ser interessante alterar é o valor da macro `TIMEOUT`, também em scan.h. Ela define, em segundos, o tempo do timeout do `connect()`.

Compilando
----------

1. extraia o pacote

`tar xzvf scanner.tar.gz`
	
2. entre no diretório criado

`cd scanner_tcd`
	
3. compile os fontes

`make`
	
4. executando

`./scanner [parâmetros]`
	
**OBS**: `scanner` depende dos executáveis `paralelo` e `serial` gerados durante a compilação e devem estar sempre no mesmo diretório

###Excluindo os executáveis###

Dentro do diretório `scanner_tcd` rode:

`make clean`
