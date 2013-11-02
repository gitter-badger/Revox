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
// Copyright 2010 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Author: jdtang@google.com (Jonathan Tang)
//
// We use Gumbo as a prefix for types, gumbo_ as a prefix for functions, and
// GUMBO_ as a prefix for enum constants (static constants get the Google-style
// kGumbo prefix).
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdarg.h>
#include <stdio.h>
#include "html.h"
#include "parser.h"
#include "util.h"

const SrcPosition kEmptySrcPosition = {0, 0, 0};

void* parser_allocate(struct InternalParser* parser, size_t num_bytes){
    return parser->_options->allocator(parser->_options->userdata, num_bytes);
}
void parser_deallocate(struct InternalParser* parser, void* ptr){
    return parser->_options->deallocator(parser->_options->userdata, ptr);
}
char* copy_stringz(struct InternalParser* parser, const char* str){
    char* buffer = parser_allocate(parser, strlen(str)+1);
    strcpy(buffer, str);
    return buffer;
}
void debug(const char* format, ...){
    #ifdef DEBUG
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    fflush(stdout);
    #endif // DEBUG
}
