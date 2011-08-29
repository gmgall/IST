#ifndef VALIDATION_H
#define VALIDATION_H

#include <regex.h>
#include "scan.h"
#define REGEXIP "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}/[0-9]{1,2}$"

/* Valida uma string representando um IP na notação decimal pontuada
 *
 * Retorna TRUE caso o IP seja válido ou FALSE caso não seja
 * FIXME Não é perfeita, considera alguns IPs inválidos como válidos
 */

int validaSubRede(const char *ip);

/* Valida uma porta
 *
 * Retorna TRUE ou FALSE
 */

int validaPorta(int porta);

#endif
