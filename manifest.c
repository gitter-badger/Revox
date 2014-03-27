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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bonus.h"
#include "hex.h"
#include "strs.h"
#include "typeparser.h"
#include "urlparser.h"

char manisec[][16] = {"CACHE:", "FALLBACK:", "SETTINGS:", "NETWORK:"};
char setts[][16] = {"prefer-online", "fast"};

char *CopyManifst(char content[], int *i){
    char result[4096], c;
    int j = 0, k = *i;

    if(content[k]=='#'){
        while(content[k]!='\r' && content[k]!='\n'){
            c = content[k++];
        }
    }
    else{
        while(content[k]!='\r' && content[k]!='\n' && content[k]!=' '){
            result[j++] = content[k++];
        }
    }
    while(content[k]=='\r' || content[k]=='\n' || content[k]==' '){
        c = content[k++];
    }
    *i = k;

    return result;
}

char *IsCached(char url[]){
    char *cont, *contIndex = NULL, *aux, c, path[4096], date[64], *dr = NULL;
    int aux2, i = 0;
    FILE *index, *output;
    long tam;

    dr = (char *)malloc(sizeof(char)*2048);
    getcwd(dr, 2048);
    strcat(dr, "\\cache\\index");
    if((index = fopen(dr, "r+"))==NULL){
        fprintf(stderr, "Erro: Arquivo Invalido!\r\nEndereco: %s\r\n", dr);
        return NULL;
    }
    tam = TamFile(index);
    contIndex = (char *)malloc(sizeof(char)*tam);
    fscanf(index, "%s", contIndex);
    aux2 = SearchString(contIndex, url);
    if(aux2!=-1){
        fseek(index, aux2, SEEK_SET);
        do{
            fscanf(index, "%c", c);
        }while(c!='\n' || c!='\r' || c!=' ');
        do{
            fscanf(index, "%c", path[i]);
            i++;
        }while(path[i]!='\n' || path[i]!='\r' || path[i]!=' ');
        i = 0;
        do{
            fscanf(index, "%c", date[i]);
            i++;
        }while(date[i]!='\n' || date[i]!='\r' || date[i]!=' ');
    }
    else{
        fclose(index);
        return NULL;
    }
    fclose(index);
    if((output = fopen(path, "r+"))==NULL){
        fprintf(stderr, "Erro: Arquivo Invalido!\r\nEndereco: %s\r\n", path);
        return NULL;
    }
    tam = TamFile(output);
    cont = (char *)malloc(sizeof(char)*tam);
    fscanf(output, "%s", cont);
    fclose(output);
    free(dr);
    free(contIndex);
    dr = NULL;
    contIndex = NULL;

    return cont;
}

char *InitManifest(char content[], char url1[], char tipo[]){
    char *result = NULL, *aux = NULL, *aux2 = NULL, cache[4096], cache2[4096], hexUrl[8192], *cached = NULL, file[8500], Time[64], *dr = NULL, path[2200];
    char tipo2[strlen(tipo)];
    FILE *output, *index;
    int i, CacheManfst;
    struct tm *defTime;
    time_t currentTime;
    Type buffer;
    /**
    *   A varíavel defcache define baseado nas prefefências do manifesto, se deve ou não salvar os arquivos indicado no manifesto.
    *   defcache igual a 0 -> fast
    *   defcache igual a 1 -> prefer-online
    **/
    int defcache = 0;
    /**
    *   A varíavel state define o estado atual que está sendo utilizados os dados do arquivo recebido
    *   state igual a 0 -> Explicito
    *   state igual a 1 -> Cache
    *   state igual a 2 -> Fallback
    *   state igual a 3 -> Settings
    *   state igual a 4 -> Network
    **/
    int state = 0;

    dr = (char *)malloc(sizeof(char)*2048);
    getcwd(dr, 2048);
    strcat(dr, "\\cache");
    sprintf(path, "mkdir %s", dr);
    system(path);
    printf("Pasta criada em: %s\r\n", dr);
    CacheManfst = SearchString(content, "CACHE MANIFEST");
    strcpy(tipo2, tipo);
    //Gerar o tempo atual antes de grava-lo na index
    currentTime = time(NULL);
    defTime = localtime(&currentTime);
    strftime(Time, 64, "%d/%m/%Y", defTime);
    if((index = fopen("cache\\index", "a+"))==NULL){
        fprintf(stderr, "Erro: Arquivo Invalido!\r\nEndereco: %s\r\n", file);
        return NULL;
    }
    for(i=0; content[i]!='\0';){
        if(aux!=NULL){
            limpaVetor(aux);
            aux = NULL;
        }
        aux = CopyManifst(content, &i);
        if(i>CacheManfst+14){
            if(strcmp(aux, "MANIFEST")==0){
                if(aux!=NULL){
                    limpaVetor(aux);
                    aux = NULL;
                }
                aux = CopyManifst(content, &i);
            }
            if(strcmp(aux, "")==0){
                if(aux!=NULL){
                    limpaVetor(aux);
                    aux = NULL;
                }
                aux = CopyManifst(content, &i);
            }
            if(strcmp(aux, manisec[0])==0){
                if(aux!=NULL){
                    limpaVetor(aux);
                    aux = NULL;
                }
                aux = CopyManifst(content, &i);
                state = 1;
            }
            else{
                if(strcmp(aux, manisec[1])==0){
                    if(aux!=NULL){
                        limpaVetor(aux);
                        aux = NULL;
                    }
                    aux = CopyManifst(content, &i);
                    state = 2;
                }
                else{
                    if(strcmp(aux, manisec[2])==0){
                        if(aux!=NULL){
                            limpaVetor(aux);
                            aux = NULL;
                        }
                        aux = CopyManifst(content, &i);
                        state = 3;
                    }
                    else if(strcmp(aux, manisec[3])==0){
                        if(aux!=NULL){
                            limpaVetor(aux);
                            aux = NULL;
                        }
                        aux = CopyManifst(content, &i);
                        state = 4;
                    }
                }
            }
            switch(state){
            case 0: case 1:
                strcpy(cache, UrlConstructor(url1, aux));
                strcpy(hexUrl, HexCreater(cache));
                if((cached = IsCached(cache))!=NULL){
                    strcpy(result, cached);
                }
                else{
                    sprintf(file, "%s\\%s", dr, hexUrl);
                    //Abre/cria a index e o arquivo no cache
                    if((output = fopen(file, "w+"))==NULL){
                        fprintf(stderr, "Erro: Arquivo Invalido!\r\nEndereco: %s\r\n", file);
                        return NULL;
                    }
                    //Armazena o cache
                    buffer = UrlConnect(cache, 1, NULL, NULL);
                    fprintf(output, "Content-Type: %s\r\n%s\r\n", tipo2, buffer.content);
                    //Fecha o arquivo
                    fclose(output);
                    //Registra o cache na index
                    fprintf(index, "%s\r\n", cache);
                    fprintf(index, "%s\r\n", hexUrl);
                    fprintf(index, "%s\r\n\r\n", Time);
                }
                break;
            case 2:
                if(aux2!=NULL){
                    limpaVetor(aux2);
                    aux2 = NULL;
                }
                aux2 = CopyManifst(content, &i);
                strcpy(cache, UrlConstructor(url1, aux));
                strcpy(cache2, UrlConstructor(url1, aux2));
                strcpy(hexUrl, HexCreater(cache2));
                if((cached = IsCached(cache))!=NULL){
                    strcpy(result, cached);
                }
                else{
                    sprintf(file, "%s\\%s", dr, hexUrl);
                    //Abre/cria a index e o arquivo no cache
                    if((output = fopen(file, "w+"))==NULL){
                        fprintf(stderr, "Erro: Arquivo Invalido!\r\nEndereco: %s\r\n", file);
                        return NULL;
                    }
                    //Armazena o cache
                    buffer = UrlConnect(cache2, 1, NULL, NULL);
                    fprintf(output, "Content-Type: %s\r\n%s\r\n", tipo2, buffer.content);
                    //Fecha o arquivo
                    fclose(output);
                    //Registra o cache na index
                    fprintf(index, "%s\r\n", cache2);
                    fprintf(index, "%s\r\n", hexUrl);
                    fprintf(index, "%s\r\n\r\n", Time);
                }
                break;
            case 3:
                if(strcmp(aux, setts[0])==0){
                    defcache = 1;
                }
                else{
                    if(strcmp(aux, setts[1])==0){
                        defcache = 0;
                    }
                }
                break;
            case 4:
                break;
            default:
                fprintf(stderr, "Erro: Estado invalido!\r\n");
            }
        }
    }
    //Fecha o arquivo
    fclose(index);
    strcpy(result, "CACHED");
    free(dr);
    dr = NULL;

    return result;
}
