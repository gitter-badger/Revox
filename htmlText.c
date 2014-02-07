/*
	Este arquivo faz parte do BCM Revox Engine;

	BCM Revox Engine é Software Livre; você pode redistribui-lo e/ou
	modificá-lo dentro dos termos da Licença Pública Geru GNU como
	publicada pela Fundação do Software Livre (FSF); na versão 3 da Licença.
	Este programa é distribuído na esperança que possa ser util,
	mas SEM NENHUMA GARANTIA; sem uma garantia implicita de ADEQUAÇÂO a
	qualquer MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a Licença Pública Geral
	GNU para maiores detalhes.
	Você deve ter recebido uma cópia da Licença Pública Geral GNU junto com
	este programa, se não, escreva para a Fundação do Software Livre(FSF) Inc.,
	51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

	BCM Revox Engine v0.1
	BCM Revox Engine -> Ano: 2013|Tipo: WebEngine
*/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bonus.h"
#include "html.h"
#include "htmlText.h"

#pragma omp

char *CreateTag(char *inicio, char *tag, char *fim){
    char *element = (char *)malloc(sizeof(char)*(32));

    strcpy(element, inicio);
    strcat(element, tag);
    strcat(element, fim);

    return element;
}

char *SubString(char *str, int inicio, int fim){
    char *sub;
    int i, j, k;

    for(k=0; str[k]!='\0'; k++);
    if(inicio >= fim || fim>k){
        fprintf(stderr, "Erro! SubString nao Encontrada!\n");
        return "\0";
    }
    if(inicio<0){
        inicio = 0;
    }
    sub = (char *)malloc(sizeof(char)*((fim-inicio)+1));
    for(i=inicio, j=0; i<fim; i++, j++){
        sub[j] = str[i];
    }
    sub[j] = '\0';

    return sub;
}

char *SubString2(char *str, char *inicio, char *fim){
    int i, j, k, l = 0, m1, n1;
    char *result, *m, *n;
    result = (char *)malloc(sizeof(char)*(strlen(str)));
    m = (char *)malloc(sizeof(char)*(strlen(str)));
    n = (char *)malloc(sizeof(char)*(strlen(str)));

    for(i=0; result[i]!='\0'; i++){
        result[i] = '\0';
    }
    for(j=0; str[j]!='\0'; j++);
    for(k=0; inicio[k]!='\0'; k++);
    for(l=0; fim[l]!='\0'; l++);
    for(i=0; str[i]!='\0'; i++){
        m = strstr(str, inicio);
        n = strstr(str, fim);
        if(m==NULL){
            m = strstr(str, strupr(inicio));
        }
        if(n==NULL){
            n = strstr(str, strupr(fim));
        }
        m1 = m-str;
        n1 = n-str;
        if(m1<0){
            m1 = 0;
        }
        if(n1<0){
            n1 = 0;
        }
        if(m1==0 && n1==0){
            fprintf(stderr, "Nao tem mais %s ou %s!\n", inicio, fim);
            break;
        }
        else{
            strcat(result, SubString(str, m1+(k+1), n1-(l/8)));
            strcat(result, "\r\n");
            str = SubString(str, n1+l, j);
            for(j=0; str[j]!='\0'; j++);
        }
    }
    free(m);
    free(n);

    return result;
}

void RemvSubString(char *str, char *substr){
    while(*str){
        char *sub=substr, *s=str;
        while(*sub && *sub==*s){
            ++sub, ++s;
        }
        if(!*sub){
            while(*s){
                *str++ = *s++;
            }
            *str = 0;
            return;
        }
        ++str;
    }
}

char *InitHTMLText(char *content){
    int c, i, j, k, l, inicio, fim;
    char tags[][16] = {"html\0", "head\0", "title\0", "body\0", "p\0"};
    char cesp[][4] = {"<\0", ">\0", "</\0", "/>\0"};
    char *conthtml, aux[16], aux2[16], *intag, *intag2, html[BUF32KB], *head, *body, header[8192], *buffer, *a, *b;
    phtml cont;
    htr htr1;
    Fhtp *fhtp1 = aloca();

    //Alocando Memória
    conthtml = (char *)malloc(sizeof(char)*(strlen(content)));
    intag = (char *)malloc(sizeof(char)*(strlen(content)));
    intag2 = (char *)malloc(sizeof(char)*(strlen(content)));
    head = (char *)malloc(sizeof(char)*(strlen(content)));
    body = (char *)malloc(sizeof(char)*(strlen(content)));

    strncpy(header, content, strstr(content, "\r\n\r\n"));
    b = strstr(header, "Content-Length: ");
    inicio = b-header;
    a = strstr(header, "\r\n");
    fim = a-header;
    SubString(aux, inicio, fim);
    c = atoi(aux);
    #pragma omp parallel for schedule(guided)
    for(i=0; i<16; i++){
        aux[i] = "\0";
    }
    buffer = (char *)malloc(sizeof(char)*c);

    for(i=0; i<5; i++){
        if(i==0 || i==3){
            // Tag <html e <body
            strcpy(aux, CreateTag(cesp[0], tags[i], "\0"));
            // Tag </html> e </body>
            strcpy(aux2, CreateTag(cesp[2], tags[i], cesp[1]));

            for(k=0; aux[k]!='\0'; k++);
            for(l=0; aux2[l]!='\0'; l++);

            intag = strstr(content, aux);
            intag2 = strstr(content, aux2);
            if(intag==NULL){
                intag = strstr(content, strupr(aux));
            }
            if(intag2==NULL){
                intag2 = strstr(content, strupr(aux2));
            }
            inicio = intag-content;
            fim = intag2-content;
            if(inicio<0){
                inicio = 0;
            }
            if(i==0){
                conthtml = SubString(content, inicio+(k+1), fim-(l/8));
            }
            else{
                body = SubString(content, inicio, fim+l);
            }
        }
        if(i==1 || i==2){
            // Tag <head> e <title>
            strcpy(aux, CreateTag(cesp[0], tags[i], cesp[1]));
            // Tag </head> e </title>
            strcpy(aux2, CreateTag(cesp[2], tags[i], cesp[1]));

            for(k=0; aux[k]!='\0'; k++);
            for(l=0; aux2[l]!='\0'; l++);

            if(i==1){
                intag = strstr(content, aux);
                intag2 = strstr(content, aux2);
                if(intag==NULL){
                    intag = strstr(content, strupr(aux));
                }
                if(intag2==NULL){
                    intag2 = strstr(content, strupr(aux2));
                }
                inicio = intag-content;
                fim = intag2-content;
                if(inicio<0){
                    inicio = 0;
                }
                head = SubString(content, inicio, fim+l);
            }
            else{
                intag = strstr(head, aux);
                intag2 = strstr(head, aux2);
                if(intag==NULL){
                    intag = strstr(head, strupr(aux));
                }
                if(intag2==NULL){
                    intag2 = strstr(head, strupr(aux2));
                }
                inicio = intag-head;
                fim = intag2-head;
                if(inicio<0){
                    inicio = 0;
                }
                strcpy(cont.title, SubString(head, inicio+k, fim-(l/8)+1));
            }
        }
        if(i==4){
            // Tag <p...>
            strcpy(aux, CreateTag(cesp[0], tags[i], "\0"));
            // Tag </p>
            strcpy(aux2, CreateTag(cesp[2], tags[i], cesp[1]));
            conthtml = SubString2(body, aux, aux2);
            strcat(html, conthtml);
            strcat(html, "\r\n");
        }
    }

    printf("%s", html);
    strcpy(cont.content, html);

    //Liberando Memória Alocada
    /*free(conthtml);
    free(intag);
    free(intag2);
    free(head);
    free(body);*/

    //Adicionando Null aos ponteiros
    conthtml = NULL;
    intag = NULL;
    intag2 = NULL;
    head = NULL;
    body = NULL;

    return html;
}
