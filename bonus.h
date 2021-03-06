/*
	Este arquivo faz parte do BCM Revox Engine;

	BCM Revox Engine é Software Livre; você pode redistribui-lo e/ou
	modificá-lo dentro dos termos da Licença Pública Geral GNU como
	publicada pela Fundação do Software Livre (FSF); na versão 3 da Licença.
	Este programa é distribuído na esperança que possa ser util,
	mas SEM NENHUMA GARANTIA; sem uma garantia implicita de ADEQUAÇÂO a
	qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral
	GNU para maiores detalhes.
	Você deve ter recebido uma cópia da Licença Pública Geral GNU junto com
	este programa, se não, escreva para a Fundação do Software Livre(FSF) Inc.,
	51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

	BCM Revox Engine v0.2
	BCM Revox Engine -> Ano: 2014|Tipo: WebEngine
*/
#ifndef BONUS_H_
#define BONUS_H_

#include <stdio.h>

//Buffer de saída de 512KB
#define BUF32KB 524288
//Tamanho do Vetor p.v
#define VKB 256
//Tamanho de 1024 bytes
#define TKB 1024
/*
    Tamanho de 1025 bytes.
    Utilizado para arquivos char incluindo o \0.
*/
#define STKB 1025
/*
    Versão do Revox em diferentes formatos
*/
#define VERSION "v0.2"
#define RVXVERSION "Revox/0.2"
#define RVXVERSIONFULL "BCM Revox Engine/0.2"

//limpa vetores
void limpaVetor(char *vet);
int inet_pton4(const char *src, char *dst);
int inet_pton6(const char *src, char *dst);
int inet_pton(int af, const char *src, char *dst);
long TamFile(FILE *arq);

#endif // BONUS_H_
