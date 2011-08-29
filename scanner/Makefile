#
# Makefile para compilação do scanner de rede
# Criador: Guilherme Magalhães Gall
# Data: Qua Out  1 19:03:55 BRT 2008
#

# Definindo o compilador a ser usado
CC=gcc

# Definindo o nome dos arquivos com os fonte e dos arquivos de cabeçalho
SRCS	= netcalc.c scan.c validation.c
HEADERS	= netcalc.h scan.h validation.h

# Opção para linkeditar com biblioteca matemática
MATHLIB = -lm
# Opção para linkeditar com biblioteca de IPC Posix (necessário para uso dos semáforos)
SEMPLIB = -lrt

all: scanner
	@echo -e "\033[0;32mPronto\033[0m"

scanner: paralelo serial scanner.c
	gcc scanner.c -o scanner

paralelo: $(SRCS) $(HEADERS)
	gcc $(SRCS) paralelo.c -o paralelo $(MATHLIB) $(SEMPLIB)
	
serial: $(SRCS) $(HEADERS)
	gcc $(SRCS) serial.c -o serial $(MATHLIB) $(SEMPLIB)
	
clean:
	rm serial paralelo scanner

