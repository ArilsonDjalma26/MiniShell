# EXPLICAÇÃO DIDÁTICA DO LEXER - Minishell

## Índice
1. [Visão Geral](#visão-geral)
2. [Objetivo do Lexer](#objetivo-do-lexer)
3. [Fluxo Completo de Processamento](#fluxo-completo-de-processamento)
4. [Extração de Substrings](#extração-de-substrings)
5. [Tratamento de Variáveis ($)](#tratamento-de-variáveis)
6. [Concatenação de Partes](#concatenação-de-partes)
7. [Criação de Tokens (Nós)](#criação-de-tokens-nós)
8. [Exemplos Práticos Passo a Passo](#exemplos-práticos-passo-a-passo)

---

## Visão Geral

O **Lexer** (Analisador Léxico) é a primeira etapa do processamento de comandos no Minishell. Ele transforma uma **string de texto** em uma **lista ligada de tokens** (unidades significativas).

```
INPUT:  "echo hello | wc -l"
         ↓
LEXER:   Divide em tokens
         ↓
OUTPUT: [WORD:echo] → [WORD:hello] → [PIPE:|] → [WORD:wc] → [WORD:-l]
```

---

## Objetivo do Lexer

**O que o Lexer faz:**
- Lê caractere por caractere do input
- Identifica diferentes tipos de elementos (palavras, operadores, variáveis)
- Extrai substrings da memória
- Cria estruturas (tokens/nós) para cada elemento
- Organiza tudo em uma lista ligada

**O que o Lexer NÃO faz:**
- Não expande variáveis (isso é trabalho do Expander)
- Não executa comandos (isso é trabalho do Executor)
- Apenas **identifica** e **classifica** os elementos

---

## Fluxo Completo de Processamento

```
┌─────────────────────────────────────────────────────────────────┐
│                   PROCESSO COMPLETO DO LEXER                    │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  1. INPUT (string na memória)                                   │
│     "echo hello"world"$USER"                                    │
│      ↑                                                          │
│      i (índice atual)                                           │
│                                                                 │
│  2. EXTRAÇÃO (criar substrings)                                 │
│     ┌──────────┬────────┬────────┬────────┐                    │
│     │ "hello"  │ "world"│ "$USER"│        │                    │
│     └──────────┴────────┴────────┴────────┘                    │
│     Partes individuais alocadas na memória                      │
│                                                                 │
│  3. CONCATENAÇÃO (juntar partes adjacentes)                     │
│     "hello" + "world" + "$USER" = "helloworld$USER"            │
│     Resultado final alocado na memória                          │
│                                                                 │
│  4. CLASSIFICAÇÃO (determinar tipo e aspas)                     │
│     - Tipo: TOKEN_WORD ou TOKEN_ENV?                            │
│     - Aspas: QUOTE_NONE, QUOTE_SINGLE ou QUOTE_DOUBLE?          │
│                                                                 │
│  5. CRIAÇÃO DO NÓ (token)                                       │
│     ┌──────────────────────────────────┐                       │
│     │ t_token *novo                    │                       │
│     │ ├─ type:  TOKEN_WORD             │                       │
│     │ ├─ quote: QUOTE_DOUBLE           │                       │
│     │ ├─ value: "helloworld$USER"      │                       │
│     │ └─ next:  NULL                   │                       │
│     └──────────────────────────────────┘                       │
│                                                                 │
│  6. ADICIONAR À LISTA                                           │
│     list → [token1] → [token2] → [novo] → NULL                 │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

---

## Extração de Substrings

### 1. O que é uma Substring?

Uma **substring** é uma cópia de uma parte da string original, alocada em um novo espaço de memória.

```c
char *input = "echo hello world";
//             01234567890123456

// Extrair "hello" (posição 5, tamanho 5)
char *substring = ft_substr(input, 5, 5);
// substring agora aponta para: "hello" (novo espaço de memória)
```

### 2. Três Tipos de Extração

#### a) **extract_plain()** - Texto Normal

Extrai texto até encontrar:
- Espaço ou tab
- Operador (`|`, `<`, `>`)
- Aspas (`'` ou `"`)
- Variável (`$`)
- Fim da string

```c
Input: "echo hello"
        ↑   ↑
      start i (parou no espaço)

Resultado: substring = "echo"
```

#### b) **extract_quoted()** - Texto entre Aspas

Extrai texto entre aspas (sem incluir as aspas):

```c
Input: "echo "hello world" done"
             ↑           ↑
        abre aspas   fecha aspas

Resultado: substring = "hello world" (sem as aspas!)
```

**Importante:** Marca `has_dquote = 1` se for aspas duplas.

#### c) **extract_env()** - Variáveis de Ambiente

Extrai variável **incluindo o `$`**:

```c
Input: "echo $USER done"
             ↑    ↑
           start  i

Resultado: substring = "$USER"
```

---

## Tratamento de Variáveis ($)

### Por que Preservamos o `$`?

O lexer **não expande** variáveis. Ele apenas **identifica** e **preserva** para que o **Expander** faça a substituição depois.

```
LEXER:     Input "$USER" → Token {value: "$USER"}
           (preserva o $)

EXPANDER:  Token {value: "$USER"} → Substitui por "aalbano"
           (faz a expansão)

EXECUTOR:  Usa "aalbano" para executar
```

### Casos Especiais de Variáveis

#### 1. Variável Normal: `$VAR`

```c
Input: "$HOME"
        ↑   ↑
      start i

Processo:
1. Detecta '$' na posição 0
2. Avança i++ para posição 1
3. Lê caracteres válidos: H, O, M, E
4. Para quando encontra caractere inválido ou fim
5. Extrai: "$HOME" (com o $)
6. Marca: has_env = 1
```

**Caracteres válidos:** `a-z`, `A-Z`, `0-9`, `_`

#### 2. Exit Status: `$?`

```c
Input: "$?"
        ↑ ↑
      start i

Processo:
1. Detecta '$' na posição 0
2. Avança i++ para posição 1
3. Detecta '?' → caso especial!
4. Avança i++ para posição 2
5. Extrai: "$?" (com o $)
6. Marca: has_env = 1
```

**O que significa `$?`:**
- Retorna o código de saída do último comando executado
- `0` = sucesso
- Outro valor = erro

**Exemplo:**
```bash
ls /pasta_existente    # comando bem-sucedido
echo $?                # imprime: 0

ls /pasta_inexistente  # comando falhou
echo $?                # imprime: 2 (ou outro código de erro)
```

#### 3. `$` Sozinho ou Inválido

```c
Input: "$ "  ou  "$|"  ou  "$"
        ↑
      start

Processo:
1. Detecta '$'
2. Avança i++ 
3. Próximo char é ' ' ou '|' ou '\0' (inválido!)
4. Retorna apenas: "$" (literal)
5. has_env = 0 (não é variável)
```

---

## Concatenação de Partes

### O Grande Diferencial: Palavras Adjacentes

No bash, partes adjacentes (sem espaços entre elas) formam **uma única palavra**:

```bash
echo hello"world"'!'$USER
#    └────────────────────┘
#    Tudo isso é UMA palavra só!
```

### Como Funciona a Concatenação

```c
void tokenize_word(char *input, int *i, t_token **list)
{
    char *result = NULL;   // Resultado final
    char *part;            // Cada parte individual
    
    // LOOP: enquanto for parte da mesma palavra
    while (is_word_char(input[*i]))
    {
        // Extrair próxima parte
        if (é_aspas)
            part = extract_quoted(...);
        else if (é_$)
            part = extract_env(...);
        else
            part = extract_plain(...);
        
        // CONCATENAR ao resultado
        result = join_and_free(result, part);
    }
    
    // Criar token com resultado concatenado
    add_token(..., result);
}
```

### Exemplo Detalhado de Concatenação

```
Input: hello"world"'!'$USER
       ↑
       i

┌────────────────────────────────────────────────────────────┐
│ ITERAÇÃO 1: char='h' → extract_plain()                    │
├────────────────────────────────────────────────────────────┤
│ Lê: "hello"                                                │
│ result = NULL + "hello" = "hello"                          │
│ i agora aponta para: "                                     │
└────────────────────────────────────────────────────────────┘

┌────────────────────────────────────────────────────────────┐
│ ITERAÇÃO 2: char='"' → extract_quoted()                   │
├────────────────────────────────────────────────────────────┤
│ Lê: "world" (sem aspas)                                    │
│ result = "hello" + "world" = "helloworld"                  │
│ has_dquote = 1                                             │
│ i agora aponta para: '                                     │
└────────────────────────────────────────────────────────────┘

┌────────────────────────────────────────────────────────────┐
│ ITERAÇÃO 3: char='\'' → extract_quoted()                  │
├────────────────────────────────────────────────────────────┤
│ Lê: "!" (sem aspas)                                        │
│ result = "helloworld" + "!" = "helloworld!"                │
│ i agora aponta para: $                                     │
└────────────────────────────────────────────────────────────┘

┌────────────────────────────────────────────────────────────┐
│ ITERAÇÃO 4: char='$' → extract_env()                      │
├────────────────────────────────────────────────────────────┤
│ Lê: "$USER"                                                │
│ result = "helloworld!" + "$USER" = "helloworld!$USER"      │
│ has_env = 1                                                │
│ i agora aponta para: espaço ou fim                         │
└────────────────────────────────────────────────────────────┘

┌────────────────────────────────────────────────────────────┐
│ FIM DO LOOP: is_word_char(' ') = 0                        │
├────────────────────────────────────────────────────────────┤
│ result final = "helloworld!$USER"                          │
│ has_dquote = 1                                             │
│ has_env = 1                                                │
└────────────────────────────────────────────────────────────┘
```

### A Função `join_and_free()`

Esta função é crucial para evitar **memory leaks**:

```c
static char *join_and_free(char *result, char *part)
{
    char *new_result;
    
    // CASO 1: Não há parte → retornar resultado
    if (!part)
        return (result);
    
    // CASO 2: Não há resultado ainda → retornar parte
    if (!result)
        return (part);
    
    // CASO 3: Concatenar
    new_result = ft_strjoin(result, part);  // Cria nova string
    free(result);  // Libera antiga
    free(part);    // Libera parte
    return (new_result);
}
```

**Por que liberamos as strings antigas?**

```
Memória ANTES:
result → "hello"
part   → "world"

Memória DEPOIS de ft_strjoin:
result     → "hello"      (ainda existe!)
part       → "world"      (ainda existe!)
new_result → "helloworld" (novo!)

Se não liberarmos result e part = MEMORY LEAK!

Memória CORRETA:
result     → (freed)
part       → (freed)
new_result → "helloworld" (único sobrevivente)
```

---

## Criação de Tokens (Nós)

### Estrutura de um Token

```c
typedef struct s_token
{
    t_token_type    type;     // Tipo do token
    t_quote_type    quote;    // Tipo de aspas usado
    char            *value;   // Valor/conteúdo
    struct s_token  *next;    // Próximo token
}   t_token;
```

### Processo de Criação

#### Passo 1: Determinar o Tipo

```c
static t_token_type get_word_type(char *value, int has_env)
{
    // Se teve variável E começa com $ E tem mais de 1 char
    if (has_env && value && value[0] == '$' && ft_strlen(value) > 1)
        return (TOKEN_ENV);
    
    // Senão, é palavra normal
    return (TOKEN_WORD);
}
```

**Exemplos:**
- `"hello"` → `TOKEN_WORD` (não tem $)
- `"$USER"` → `TOKEN_ENV` (tem $ e mais chars)
- `"hello$USER"` → `TOKEN_WORD` (não começa com $)
- `"$"` → `TOKEN_WORD` (só tem $, sem nome de var)

#### Passo 2: Determinar o Tipo de Aspas

```c
if (result && !has_dquote)
    // Não teve double quote → QUOTE_NONE
    add_token(list, new_token(..., QUOTE_NONE, result));
else if (result)
    // Teve double quote → QUOTE_DOUBLE
    add_token(list, new_token(..., QUOTE_DOUBLE, result));
```

**Por que isso importa?**

No bash, aspas simples e duplas têm comportamentos diferentes:

```bash
echo '$USER'   # Imprime: $USER (literal)
echo "$USER"   # Imprime: aalbano (expandido)
```

O campo `quote` indica ao **Expander** se deve expandir variáveis:
- `QUOTE_NONE` → Expandir
- `QUOTE_DOUBLE` → Expandir
- `QUOTE_SINGLE` → NÃO expandir (manter literal)

#### Passo 3: Criar o Nó

```c
t_token *new_token(t_token_type type, t_quote_type quote, char *value)
{
    t_token *tok;
    
    // 1. Alocar memória para o nó
    tok = malloc(sizeof(t_token));
    if (!tok)
        return (NULL);
    
    // 2. Preencher campos
    tok->type = type;
    tok->quote = quote;
    tok->value = value;  // A string já foi alocada antes!
    tok->next = NULL;
    
    return (tok);
}
```

#### Passo 4: Adicionar à Lista

```c
void add_token(t_token **list, t_token *new_tok)
{
    t_token *tmp;
    
    if (!new_tok)
        return;
    
    // Lista vazia → novo é o primeiro
    if (!*list)
    {
        *list = new_tok;
        return;
    }
    
    // Percorrer até o fim
    tmp = *list;
    while (tmp->next)
        tmp = tmp->next;
    
    // Adicionar no fim
    tmp->next = new_tok;
}
```

**Visualização:**
```
ANTES:  list → [A] → [B] → NULL

DEPOIS: list → [A] → [B] → [novo] → NULL
```

---

## Exemplos Práticos Passo a Passo

### Exemplo 1: Variável Simples

**Input:** `echo $USER`

```
┌─────────────────────────────────────────────────────────┐
│ TOKENIZAÇÃO DE: echo $USER                              │
├─────────────────────────────────────────────────────────┤
│                                                         │
│ PALAVRA 1: "echo"                                       │
│ ──────────────────                                      │
│ 1. extract_plain() → "echo"                             │
│ 2. result = "echo"                                      │
│ 3. has_dquote = 0, has_env = 0                          │
│ 4. Token: {WORD, NONE, "echo"}                          │
│                                                         │
│ PALAVRA 2: "$USER"                                      │
│ ──────────────────                                      │
│ 1. extract_env() → "$USER"                              │
│ 2. result = "$USER"                                     │
│ 3. has_dquote = 0, has_env = 1                          │
│ 4. get_word_type() → TOKEN_ENV (começa com $)           │
│ 5. Token: {ENV, NONE, "$USER"}                          │
│                                                         │
│ LISTA FINAL:                                            │
│ [WORD:"echo"] → [ENV:"$USER"] → NULL                    │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

### Exemplo 2: Variável entre Aspas Duplas

**Input:** `echo "$HOME"`

```
┌─────────────────────────────────────────────────────────┐
│ TOKENIZAÇÃO DE: echo "$HOME"                            │
├─────────────────────────────────────────────────────────┤
│                                                         │
│ PALAVRA 1: "echo"                                       │
│ ──────────────────                                      │
│ Token: {WORD, NONE, "echo"}                             │
│                                                         │
│ PALAVRA 2: "$HOME" (com aspas duplas)                   │
│ ──────────────────────────────────                      │
│ 1. extract_quoted() → "$HOME" (sem as aspas "")         │
│ 2. has_dquote = 1 (marcado!)                            │
│ 3. result = "$HOME"                                     │
│ 4. has_env = 0 (não chamou extract_env)                 │
│ 5. get_word_type() → TOKEN_WORD                         │
│ 6. Token: {WORD, DQUOTE, "$HOME"}                       │
│                                                         │
│ Nota: O Expander verá DQUOTE e expandirá a variável!    │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

### Exemplo 3: Variável entre Aspas Simples

**Input:** `echo '$HOME'`

```
┌─────────────────────────────────────────────────────────┐
│ TOKENIZAÇÃO DE: echo '$HOME'                            │
├─────────────────────────────────────────────────────────┤
│                                                         │
│ PALAVRA 1: "echo"                                       │
│ ──────────────────                                      │
│ Token: {WORD, NONE, "echo"}                             │
│                                                         │
│ PALAVRA 2: '$HOME' (com aspas simples)                  │
│ ──────────────────────────────────                      │
│ 1. extract_quoted() → "$HOME" (sem as aspas '')         │
│ 2. has_dquote = 0 (foi aspas simples!)                  │
│ 3. result = "$HOME"                                     │
│ 4. Token: {WORD, NONE, "$HOME"}                         │
│                                                         │
│ Nota: quote=NONE porque foi aspas SIMPLES              │
│       O Expander NÃO expandirá (aspas simples = literal)│
│                                                         │
│ PROBLEMA: Nossa implementação atual não distingue       │
│           QUOTE_SINGLE de QUOTE_NONE!                   │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

### Exemplo 4: Concatenação Complexa

**Input:** `echo hello"world"'!'$USER`

```
┌─────────────────────────────────────────────────────────┐
│ TOKENIZAÇÃO DE: echo hello"world"'!'$USER               │
├─────────────────────────────────────────────────────────┤
│                                                         │
│ PALAVRA 1: "echo"                                       │
│ ──────────────────                                      │
│ Token: {WORD, NONE, "echo"}                             │
│                                                         │
│ PALAVRA 2: hello"world"'!'$USER (TUDO JUNTO!)           │
│ ─────────────────────────────────────────               │
│                                                         │
│ Loop de Concatenação:                                   │
│                                                         │
│ Iteração 1: extract_plain()                             │
│   → part = "hello"                                      │
│   → result = "hello"                                    │
│                                                         │
│ Iteração 2: extract_quoted(")                           │
│   → part = "world"                                      │
│   → has_dquote = 1                                      │
│   → result = "hello" + "world" = "helloworld"           │
│                                                         │
│ Iteração 3: extract_quoted(')                           │
│   → part = "!"                                          │
│   → result = "helloworld" + "!" = "helloworld!"         │
│                                                         │
│ Iteração 4: extract_env()                               │
│   → part = "$USER"                                      │
│   → has_env = 1                                         │
│   → result = "helloworld!" + "$USER" = "helloworld!$USER"│
│                                                         │
│ Fim do loop (encontrou espaço ou fim)                   │
│                                                         │
│ Criação do Token:                                       │
│   - value = "helloworld!$USER"                          │
│   - has_dquote = 1 → quote = DQUOTE                     │
│   - has_env = 1, mas não começa com $ → type = WORD     │
│                                                         │
│ Token: {WORD, DQUOTE, "helloworld!$USER"}               │
│                                                         │
│ LISTA FINAL:                                            │
│ [WORD:"echo"] → [WORD:"helloworld!$USER"] → NULL        │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

### Exemplo 5: Exit Status `$?`

**Input:** `echo $?`

```
┌─────────────────────────────────────────────────────────┐
│ TOKENIZAÇÃO DE: echo $?                                 │
├─────────────────────────────────────────────────────────┤
│                                                         │
│ PALAVRA 1: "echo"                                       │
│ ──────────────────                                      │
│ Token: {WORD, NONE, "echo"}                             │
│                                                         │
│ PALAVRA 2: "$?"                                         │
│ ──────────────────                                      │
│ 1. extract_env() detecta '$'                            │
│ 2. Avança e detecta '?' → caso especial!                │
│ 3. result = "$?"                                        │
│ 4. has_env = 1                                          │
│ 5. get_word_type() → TOKEN_ENV (começa com $ e len>1)   │
│ 6. Token: {ENV, NONE, "$?"}                             │
│                                                         │
│ LISTA FINAL:                                            │
│ [WORD:"echo"] → [ENV:"$?"] → NULL                       │
│                                                         │
│ O Expander depois substituirá "$?" pelo código de saída │
│ do último comando executado.                            │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

---

## Resumo dos Conceitos Principais

### 1. Extração de Substrings
- Copia partes da string original para novos espaços de memória
- Três tipos: `extract_plain()`, `extract_quoted()`, `extract_env()`

### 2. Preservação do `$`
- O lexer **não expande** variáveis
- Apenas **identifica** e **preserva** o `$` no value
- O **Expander** fará a substituição depois

### 3. Concatenação
- Partes adjacentes (sem espaços) formam uma única palavra
- Usa `join_and_free()` para evitar memory leaks
- Loop continua enquanto `is_word_char()` retornar true

### 4. Flags de Controle
- `has_dquote`: Indica se teve double quote (afeta expansão)
- `has_env`: Indica se teve variável $ (ajuda a classificar tipo)

### 5. Classificação
- **Tipo:** `TOKEN_WORD` ou `TOKEN_ENV`
- **Aspas:** `QUOTE_NONE` ou `QUOTE_DOUBLE`

### 6. Criação de Nós
- Aloca memória para `t_token`
- Preenche campos (type, quote, value, next)
- Adiciona à lista ligada

---

## Diagrama Final: Visão Completa

```
┌─────────────────────────────────────────────────────────────────────┐
│                    VISÃO COMPLETA DO LEXER                          │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  INPUT STRING                                                       │
│  ┌───────────────────────────────────────────────┐                 │
│  │ "echo hello"world"$USER | wc -l > out.txt"    │                 │
│  └───────────────────────────────────────────────┘                 │
│                         │                                           │
│                         ▼                                           │
│  ┌──────────────────────────────────────────────┐                  │
│  │           LOOP PRINCIPAL (lexer)             │                  │
│  │  while (input[i])                            │                  │
│  │    - skip_space()                            │                  │
│  │    - if (is_operator) → tokenize_operator()  │                  │
│  │    - else → tokenize_word()                  │                  │
│  └──────────────────────────────────────────────┘                  │
│                         │                                           │
│                         ▼                                           │
│  ┌──────────────────────────────────────────────┐                  │
│  │         tokenize_word() LOOP                 │                  │
│  │  while (is_word_char)                        │                  │
│  │    1. Extrair parte (plain/quoted/env)       │                  │
│  │    2. Concatenar ao resultado                │                  │
│  │    3. Marcar flags (has_dquote, has_env)     │                  │
│  └──────────────────────────────────────────────┘                  │
│                         │                                           │
│                         ▼                                           │
│  ┌──────────────────────────────────────────────┐                  │
│  │         CRIAÇÃO DO TOKEN                     │                  │
│  │  1. get_word_type() → type                   │                  │
│  │  2. has_dquote → quote                       │                  │
│  │  3. new_token(type, quote, result)           │                  │
│  │  4. add_token(list, token)                   │                  │
│  └──────────────────────────────────────────────┘                  │
│                         │                                           │
│                         ▼                                           │
│  LISTA LIGADA DE TOKENS                                             │
│  ┌──────┬─────────┬──────┬──────┬──────┬──────┬──────────┐         │
│  │ WORD │  WORD   │ PIPE │ WORD │ WORD │REDIR │   WORD   │         │
│  │"echo"│"hello..│ "|"  │ "wc" │ "-l" │ ">"  │"out.txt" │         │
│  └──┬───┴────┬────┴───┬──┴───┬──┴───┬──┴───┬──┴─────┬────┘         │
│     └────────┴────────┴──────┴──────┴──────┴────────┘               │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

---

**Documento criado para explicação didática do Lexer - Minishell**
**Data:** 17 de Fevereiro de 2026
