# EXPLICAÇÃO DIDÁTICA DO LEXER - Minishell

## Índice
1. [Visão Geral](#visão-geral)
2. [Objetivo do Lexer](#objetivo-do-lexer)
3. [Arquitetura dos Ficheiros](#arquitetura-dos-ficheiros)
4. [Fluxo Completo de Processamento](#fluxo-completo-de-processamento)
5. [O Sistema de Parts (Segmentos)](#o-sistema-de-parts-segmentos)
6. [Extração de Substrings](#extração-de-substrings)
7. [Criação de Tokens (Nós)](#criação-de-tokens-nós)
8. [O Papel do Expander (etapa seguinte)](#o-papel-do-expander)
9. [Exemplos Práticos Passo a Passo](#exemplos-práticos-passo-a-passo)

---

## Visão Geral

O **Lexer** (Analisador Léxico) é a primeira etapa do processamento de comandos no Minishell. Ele transforma uma **string de texto** em uma **lista ligada de tokens** (unidades significativas).

O lexer **NÃO expande variáveis**. Ele apenas identifica, classifica e preserva a informação necessária para que o **Expander** (etapa seguinte) saiba o que expandir.

```
INPUT:  "echo hello | wc -l"
         ↓
LEXER:   Divide em tokens (sem expandir)
         ↓
OUTPUT: [WORD:echo] → [WORD:hello] → [PIPE:|] → [WORD:wc] → [WORD:-l]
         ↓
EXPANDER: Expande variáveis $
         ↓
PARSER:  Monta comandos
         ↓
EXECUTOR: Executa
```

---

## Objetivo do Lexer

**O que o Lexer faz:**
- Lê caractere por caractere do input
- Identifica diferentes tipos de elementos (palavras, operadores)
- Separa cada palavra em **parts** (segmentos) com seu tipo de aspas
- Concatena os parts para formar o `value` do token (sem expandir)
- Cria tokens e organiza tudo em uma lista ligada

**O que o Lexer NÃO faz:**
- **NÃO expande variáveis** (isso é trabalho do Expander)
- Não executa comandos (isso é trabalho do Executor)
- Não analisa sintaxe de comandos (isso é trabalho do Parser)

---

## Arquitetura dos Ficheiros

O lexer está dividido em **7 ficheiros**, respeitando a norminette (máx. 5 funções por ficheiro):

```
lexer/
├── lexer.c           → Função principal: lexer()
│                       Loop que chama tokenize_operator() ou tokenize_word()
│
├── lexer_quotes.c    → Tokenização de palavras (5 funções)
│                       extract_quoted(), extract_plain(), is_word_char(),
│                       build_word_parts(), tokenize_word()
│
├── lexer_tokens.c    → Criação e gestão de tokens (3 funções)
│                       new_token(), add_token(), tokenize_operator()
│
├── lexer_parts.c     → Criação e gestão de parts/segmentos (4 funções)
│                       new_part(), add_part(), free_parts(), concat_parts()
│
├── lexer_expand.c    → Expansão de variáveis $ (5 funções)
│                       ⚠ NÃO é chamado pelo lexer!
│                       Disponível para o EXPANDER usar no futuro.
│                       append_str(), get_env_value(), extract_env_text(),
│                       expand_env_in_str(), expand_parts()
│
├── lexer_utils.c     → Funções utilitárias (5 funções)
│                       skip_space(), is_operator(), is_quote(),
│                       is_env_char(), get_type_str()
│
└── lexer_print.c     → Impressão e libertação (3 funções)
                        get_quote_str(), print_tokens(), free_tokens()
```

---

## Fluxo Completo de Processamento

```
┌──────────────────────────────────────────────────────────────────┐
│                  PROCESSO COMPLETO DO LEXER                      │
├──────────────────────────────────────────────────────────────────┤
│                                                                  │
│  1. INPUT (string na memória)                                    │
│     "echo hello"world"$USER"                                     │
│      ↑                                                           │
│      i (índice atual)                                            │
│                                                                  │
│  2. SEGMENTAÇÃO EM PARTS (cada parte com seu tipo de aspas)      │
│     ┌─────────────────┬──────────────────┬──────────────────┐    │
│     │ QUOTE_NONE      │ QUOTE_DOUBLE     │ QUOTE_NONE       │    │
│     │ value: "hello"  │ value: "world"   │ value: "$USER"   │    │
│     └────────┬────────┴─────────┬────────┴────────┬─────────┘    │
│              └──────────────────┴─────────────────┘               │
│              Lista ligada de t_part                               │
│                                                                  │
│  3. CONCATENAÇÃO LITERAL (sem expandir!)                         │
│     concat_parts() junta os values:                              │
│     "hello" + "world" + "$USER" = "helloworld$USER"              │
│     ⚠ O $USER NÃO é expandido aqui!                             │
│                                                                  │
│  4. CRIAÇÃO DO TOKEN                                             │
│     ┌────────────────────────────────────────┐                   │
│     │ t_token *token                         │                   │
│     │ ├─ type:  TOKEN_WORD                   │                   │
│     │ ├─ quote: QUOTE_NONE                   │                   │
│     │ ├─ value: "helloworld$USER" (literal!) │                   │
│     │ ├─ parts: [NONE:"hello"]→              │                   │
│     │ │         [DOUBLE:"world"]→            │                   │
│     │ │         [NONE:"$USER"]→NULL          │                   │
│     │ └─ next:  NULL                         │                   │
│     └────────────────────────────────────────┘                   │
│                                                                  │
│  5. ADICIONAR À LISTA                                            │
│     list → [token1] → [token2] → [novo] → NULL                  │
│                                                                  │
│  ═══════════════════════════════════════════════════              │
│  DEPOIS → O EXPANDER percorre a lista de parts de                │
│  cada token e expande $ onde necessário (não em SINGLE)          │
│  ═══════════════════════════════════════════════════              │
│                                                                  │
└──────────────────────────────────────────────────────────────────┘
```

---

## O Sistema de Parts (Segmentos)

### O Problema Anterior

Antes, o lexer concatenava tudo e guardava apenas um `qtype` (o último tipo de aspas visto). Isso causava problemas:

```
Input: echo hello"world"'$USER'
                  ↑        ↑
             DOUBLE    SINGLE

Antes: qtype = QUOTE_SINGLE (último visto)
       value = "helloworld$USER"
       → O expander não sabia que "world" estava em double quotes
       → O expander não sabia que "$USER" estava em single quotes
       → COMPORTAMENTO ERRADO!
```

### A Solução: Lista de Parts

Agora cada palavra é dividida em **segmentos (parts)**, e cada segmento guarda o seu tipo de aspas:

```c
typedef struct s_part
{
    t_quote_type    quote;    // Tipo de aspas DESTE segmento
    char            *value;   // Conteúdo deste segmento (literal, sem expandir)
    struct s_part   *next;    // Próximo segmento
}   t_part;
```

**O token agora tem um ponteiro para a lista de parts:**

```c
typedef struct s_token
{
    t_token_type    type;     // Tipo do token
    t_quote_type    quote;    // (mantido por compatibilidade)
    char            *value;   // Valor concatenado (literal, sem expandir!)
    struct s_part   *parts;   // Lista de segmentos com contexto de aspas
    struct s_token  *next;    // Próximo token
}   t_token;
```

### Visualização

```
Input: hello"world"'$USER'

Parts criados pelo lexer:
┌───────────────────┐    ┌───────────────────┐    ┌───────────────────┐
│ part 1            │    │ part 2            │    │ part 3            │
│ quote: NONE       │───▶│ quote: DOUBLE     │───▶│ quote: SINGLE     │───▶ NULL
│ value: "hello"    │    │ value: "world"    │    │ value: "$USER"    │
└───────────────────┘    └───────────────────┘    └───────────────────┘

token->value = "helloworld$USER" (concatenação literal, sem expandir)

Depois, o EXPANDER percorrerá os parts:
  part1 (NONE):   "hello"  → expandir → "hello"   (sem $ = nada muda)
  part2 (DOUBLE): "world"  → expandir → "world"   (sem $ = nada muda)
  part3 (SINGLE): "$USER"  → NÃO EXPANDIR → "$USER" (single quote!)

Resultado final do expander: "helloworld$USER"
```

### Ficheiro: lexer_parts.c

```c
// Cria um novo segmento
t_part *new_part(t_quote_type quote, char *value)

// Adiciona ao fim da lista de parts
void add_part(t_part **list, t_part *new_p)

// Liberta toda a lista de parts
void free_parts(t_part *list)

// Concatena os values dos parts SEM expandir (usado pelo lexer)
char *concat_parts(t_part *parts)
```

---

## Extração de Substrings

### Dois Tipos de Extração (em lexer_quotes.c)

#### a) **extract_plain()** - Texto Normal (sem aspas)

Extrai texto até encontrar espaço, tab, operador ou aspas:

```c
Input: "echo hello"
        ↑   ↑
      start i (parou no espaço)

Resultado: "echo" com QUOTE_NONE
```

**Nota:** `extract_plain()` NÃO para ao encontrar `$`. O `$` faz parte do texto normal e será tratado pelo **Expander** depois.

#### b) **extract_quoted()** - Texto entre Aspas

Extrai texto entre aspas (sem incluir as aspas) e marca o tipo:

```c
Input: 'echo "hello world" done'
              ↑           ↑
         abre aspas   fecha aspas

Resultado: "hello world" com QUOTE_DOUBLE
```

```c
Input: "echo '$HOME' done"
              ↑    ↑
         abre '  fecha '

Resultado: "$HOME" com QUOTE_SINGLE
```

#### c) **build_word_parts()** - Monta a Lista de Parts

Esta função percorre a palavra e chama `extract_plain()` ou `extract_quoted()` para cada trecho, adicionando à lista de parts:

```c
static void build_word_parts(char *input, int *i, t_part **parts)
{
    while (is_word_char(input[*i]))
    {
        if (input[*i] == '\'' || input[*i] == '"')
            part = extract_quoted(input, i, &qtype);
            → add_part(parts, new_part(qtype, part));
        else
            part = extract_plain(input, i);
            → add_part(parts, new_part(QUOTE_NONE, part));
    }
}
```

---

## Criação de Tokens (Nós)

### Fluxo de tokenize_word()

```c
void tokenize_word(char *input, int *i, t_token **list)
{
    // 1. Construir lista de parts
    build_word_parts(input, i, &parts);

    // 2. Concatenar parts SEM expandir
    value = concat_parts(parts);

    // 3. Criar token com valor literal
    token = new_token(TOKEN_WORD, QUOTE_NONE, value);
    token->parts = parts;

    // 4. Adicionar à lista
    add_token(list, token);
}
```

**Importante:** O `value` do token é a concatenação literal dos parts. A expansão de `$` será feita depois pelo **Expander**, que percorrerá `token->parts` e respeitará o tipo de aspas de cada segmento.

---

## O Papel do Expander

O **Expander** é a etapa que vem DEPOIS do lexer e ANTES do parser. Ele percorre a lista de tokens e, para cada token que tem parts, expande as variáveis `$` respeitando o contexto de aspas.

### Regra Fundamental do Bash

```bash
echo $USER       # → aalbano     (sem aspas = EXPANDE)
echo "$USER"     # → aalbano     (double quotes = EXPANDE)
echo '$USER'     # → $USER       (single quotes = NÃO EXPANDE)
```

### Como o Expander Usará os Parts

A função `expand_parts()` (em `lexer_expand.c`) já está pronta para ser chamada pelo expander:

```c
char *expand_parts(t_part *parts)
{
    while (parts)
    {
        if (parts->quote == QUOTE_SINGLE)
            → NÃO expande (mantém literal)
        else
            → Expande $ chamando getenv()
        parts = parts->next;
    }
}
```

### Pipeline Completo

```
    LEXER                    EXPANDER                PARSER
 (tokeniza)              (expande $)           (monta comandos)

echo '$USER'"$HOME"     echo '$USER'"$HOME"     echo $USER/home/user
      ↓                       ↓                       ↓
[WORD:"$USER$HOME"]     parts:                  t_cmd:
 parts:                   SINGLE→"$USER"(skip)    args: ["echo",
  SINGLE:"$USER"          DOUBLE→"$HOME"(expand)        "$USER/home/user"]
  DOUBLE:"$HOME"         → "$USER/home/user"
```

---

## Exemplos Práticos Passo a Passo

### Exemplo 1: `echo $USER`

```
┌──────────────────────────────────────────────────────────┐
│ TOKENIZAÇÃO DE: echo $USER                               │
├──────────────────────────────────────────────────────────┤
│                                                          │
│ PALAVRA 1: "echo"                                        │
│ Parts: [NONE:"echo"]                                     │
│ Token: {WORD, NONE, value:"echo"}                        │
│                                                          │
│ PALAVRA 2: "$USER"                                       │
│ Parts: [NONE:"$USER"]                                    │
│ Token: {WORD, NONE, value:"$USER"}  ← literal!           │
│                                                          │
│ LISTA FINAL DO LEXER:                                    │
│ [WORD:"echo"] → [WORD:"$USER"] → NULL                    │
│                                                          │
│ Depois o EXPANDER verá NONE → expande → "aalbano"        │
│                                                          │
└──────────────────────────────────────────────────────────┘
```

### Exemplo 2: `echo "$HOME"`

```
┌──────────────────────────────────────────────────────────┐
│ TOKENIZAÇÃO DE: echo "$HOME"                             │
├──────────────────────────────────────────────────────────┤
│                                                          │
│ PALAVRA 2: "$HOME" (entre aspas duplas)                  │
│ Parts: [DOUBLE:"$HOME"]                                  │
│ Token: {WORD, NONE, value:"$HOME"}  ← literal!           │
│                                                          │
│ Depois o EXPANDER verá DOUBLE → expande → "/home/user"   │
│                                                          │
└──────────────────────────────────────────────────────────┘
```

### Exemplo 3: `echo '$HOME'`

```
┌──────────────────────────────────────────────────────────┐
│ TOKENIZAÇÃO DE: echo '$HOME'                             │
├──────────────────────────────────────────────────────────┤
│                                                          │
│ PALAVRA 2: '$HOME' (entre aspas simples)                 │
│ Parts: [SINGLE:"$HOME"]                                  │
│ Token: {WORD, NONE, value:"$HOME"}  ← literal!           │
│                                                          │
│ Depois o EXPANDER verá SINGLE → NÃO expande → "$HOME"   │
│                                                          │
│ ✅ Aspas simples BLOQUEIAM expansão de $                 │
│                                                          │
└──────────────────────────────────────────────────────────┘
```

### Exemplo 4: `echo hello"world"$USER`

```
┌──────────────────────────────────────────────────────────┐
│ TOKENIZAÇÃO DE: echo hello"world"$USER                   │
├──────────────────────────────────────────────────────────┤
│                                                          │
│ PALAVRA 2: hello"world"$USER (TUDO JUNTO!)               │
│                                                          │
│ Construção dos parts (build_word_parts):                  │
│   Iteração 1: 'h' → extract_plain() até encontrar '"'   │
│     → part: [NONE:"hello"]                               │
│   Iteração 2: '"' → extract_quoted()                     │
│     → part: [DOUBLE:"world"]                             │
│   Iteração 3: '$' → extract_plain() até fim              │
│     → part: [NONE:"$USER"]                               │
│                                                          │
│ Parts: [NONE:"hello"] → [DOUBLE:"world"] → [NONE:"$USER"]│
│                                                          │
│ concat_parts(): "hello" + "world" + "$USER"              │
│               = "helloworld$USER" (literal!)              │
│                                                          │
│ Token: {WORD, NONE, value:"helloworld$USER"}             │
│                                                          │
│ Depois o EXPANDER percorrerá os parts:                   │
│   NONE:"hello"   → "hello"                               │
│   DOUBLE:"world" → "world"                               │
│   NONE:"$USER"   → "aalbano"                             │
│   = "helloworldaalbano"                                   │
│                                                          │
└──────────────────────────────────────────────────────────┘
```

### Exemplo 5: `echo 'hello'"$USER"` (Misto)

```
┌──────────────────────────────────────────────────────────┐
│ TOKENIZAÇÃO DE: echo 'hello'"$USER"                      │
├──────────────────────────────────────────────────────────┤
│                                                          │
│ Parts:                                                   │
│   [SINGLE:"hello"] → [DOUBLE:"$USER"]                    │
│                                                          │
│ concat_parts(): "hello" + "$USER" = "hello$USER"         │
│ Token: {WORD, NONE, value:"hello$USER"}                  │
│                                                          │
│ Depois o EXPANDER:                                       │
│   SINGLE:"hello"  → "hello"  (literal, não expande)      │
│   DOUBLE:"$USER"  → "aalbano" (expande!)                 │
│   = "helloaalbano"                                        │
│                                                          │
│ ✅ Cada segmento expandido segundo SEU tipo de aspas     │
│                                                          │
└──────────────────────────────────────────────────────────┘
```

### Exemplo 6: `echo $?`

```
┌──────────────────────────────────────────────────────────┐
│ TOKENIZAÇÃO DE: echo $?                                  │
├──────────────────────────────────────────────────────────┤
│                                                          │
│ PALAVRA 2: "$?"                                          │
│ Parts: [NONE:"$?"]                                       │
│ Token: {WORD, NONE, value:"$?"}  ← literal!              │
│                                                          │
│ Depois o EXPANDER verá NONE → expande "$?"               │
│ → substitui pelo exit status do último comando           │
│                                                          │
└──────────────────────────────────────────────────────────┘
```

### Exemplo 7: `echo hello | wc -l > out.txt`

```
┌──────────────────────────────────────────────────────────┐
│ TOKENIZAÇÃO DE: echo hello | wc -l > out.txt             │
├──────────────────────────────────────────────────────────┤
│                                                          │
│ [WORD:"echo"] → [WORD:"hello"] → [PIPE:"|"]             │
│     → [WORD:"wc"] → [WORD:"-l"] → [REDIR_OUT:">"]      │
│     → [WORD:"out.txt"] → NULL                            │
│                                                          │
│ Nota: Operadores são tokenizados por tokenize_operator() │
│       em lexer_tokens.c, sem passar pelo sistema de parts│
│                                                          │
└──────────────────────────────────────────────────────────┘
```

---

## Resumo: Separação de Responsabilidades

```
┌─────────┐    ┌──────────┐    ┌────────┐    ┌──────────┐
│  LEXER  │ →  │ EXPANDER │ →  │ PARSER │ →  │ EXECUTOR │
│         │    │          │    │        │    │          │
│Tokeniza │    │Expande $ │    │Monta   │    │Executa   │
│Guarda   │    │Usa parts │    │t_cmd   │    │comandos  │
│parts    │    │p/ saber  │    │        │    │          │
│         │    │contexto  │    │        │    │          │
└─────────┘    └──────────┘    └────────┘    └──────────┘
```

### O que Mudou vs Versão Anterior

| Aspecto | Antes (v1) | Antes (v2) | Agora (v3) |
|---------|------------|------------|------------|
| **Aspas** | Um `qtype` global | Cada part tem `quote` | Cada part tem `quote` |
| **Expansão** | Não existia | Feita no lexer | **Feita pelo Expander** |
| **token->value** | Concatenação literal | Valor expandido | **Concatenação literal** |
| **token->parts** | Não existia | Guardados | **Guardados (para o Expander)** |
| **Ficheiros** | 4 | 7 | 7 (norminette OK) |

### Por que é Melhor

1. **Separação de responsabilidades:** Lexer tokeniza, Expander expande, Parser analisa
2. **Parts preservados:** O Expander tem toda a informação de aspas por segmento
3. **Expansão correta:** Cada segmento expandido segundo SEU tipo de aspas
4. **Norminette:** Máximo 5 funções por ficheiro, máximo 25 linhas por função
5. **Sem ternários:** Todos os `? :` substituídos por `if/else`

---

## Diagrama Final: Visão Completa

```
┌──────────────────────────────────────────────────────────────────┐
│                   VISÃO COMPLETA DO LEXER                        │
├──────────────────────────────────────────────────────────────────┤
│                                                                  │
│  INPUT STRING                                                    │
│  ┌──────────────────────────────────────────────┐                │
│  │ "echo hello"world"$USER | wc -l > out.txt"   │                │
│  └──────────────────────────────────────────────┘                │
│                        │                                         │
│                        ▼                                         │
│  ┌─────────────────────────────────────────────┐                 │
│  │          LOOP PRINCIPAL (lexer.c)           │                 │
│  │  while (input[i])                           │                 │
│  │    - skip_space()                           │                 │
│  │    - if (is_operator) → tokenize_operator() │                 │
│  │    - else → tokenize_word()                 │                 │
│  └─────────────────────────────────────────────┘                 │
│                        │                                         │
│              ┌─────────┴──────────┐                              │
│              ▼                    ▼                               │
│  ┌──────────────────┐  ┌──────────────────────┐                  │
│  │ tokenize_operator │  │   tokenize_word()    │                  │
│  │ (lexer_tokens.c)  │  │  (lexer_quotes.c)    │                  │
│  │                   │  │                      │                  │
│  │ Cria token direto │  │ 1. build_word_parts()│                  │
│  │ PIPE, REDIR, etc  │  │    ↓                 │                  │
│  └──────────────────┘  │ 2. concat_parts()    │                  │
│                         │    (lexer_parts.c)   │                  │
│                         │    ↓                 │                  │
│                         │ 3. new_token()       │                  │
│                         │    (lexer_tokens.c)  │                  │
│                         └──────────────────────┘                  │
│                        │                                         │
│                        ▼                                         │
│  LISTA LIGADA DE TOKENS (valores literais, SEM expandir)         │
│  ┌──────┬──────────────┬──────┬──────┬──────┬──────┬────────┐    │
│  │ WORD │     WORD     │ PIPE │ WORD │ WORD │REDIR │  WORD  │    │
│  │"echo"│"helloworld   │ "|"  │ "wc" │ "-l" │ ">"  │"out.txt│    │
│  │      │  $USER"      │      │      │      │      │   "    │    │
│  └──┬───┴──────┬───────┴───┬──┴───┬──┴───┬──┴───┬──┴────┬───┘    │
│     └──────────┴───────────┴──────┴──────┴──────┴───────┘         │
│                        │                                         │
│                        ▼                                         │
│            ┌───────────────────────┐                             │
│            │  EXPANDER (futuro)    │                             │
│            │  Percorre parts de    │                             │
│            │  cada token e expande │                             │
│            │  $ fora de SINGLE     │                             │
│            └───────────────────────┘                             │
│                                                                  │
└──────────────────────────────────────────────────────────────────┘
```

---

**Documento atualizado — Lexer sem expansão, parts para o Expander**
**Data:** 19 de Fevereiro de 2026
