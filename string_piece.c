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
#include "string_piece.h"
#include "util.h"

struct InternalParser;

const StringPiece kEmptyString = { NULL, 0 };

bool string_equals(const StringPiece* str1, const StringPiece* str2) {
  return str1->length == str2->length && !memcmp(str1->data, str2->data, str1->length);
}

bool string_equals_ignore_case(const StringPiece* str1, const StringPiece* str2) {
  return str1->length == str2->length && !strncasecmp(str1->data, str2->data, str1->length);
}

void string_copy(struct InternalParser* parser, StringPiece* dest, const StringPiece* source) {
  dest->length = source->length;
  char* buffer = parser_allocate(parser, source->length);
  memcpy(buffer, source->data, source->length);
  dest->data = buffer;
}
