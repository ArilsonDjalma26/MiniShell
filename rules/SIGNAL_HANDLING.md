# Tratamento de Sinais no Minishell

## Pré-requisitos: O que é preciso entender antes

### O que é um Descritor de Ficheiro (File Descriptor)?
Um descritor de ficheiro é um número inteiro que o sistema operacional usa para identificar
um ficheiro ou canal de comunicação aberto por um processo. Existem 3 descritores padrão:

| Constante        | Valor | Significado              |
|------------------|-------|--------------------------|
| `STDIN_FILENO`   | `0`   | Entrada padrão (teclado) |
| `STDOUT_FILENO`  | `1`   | Saída padrão (terminal)  |
| `STDERR_FILENO`  | `2`   | Saída de erros (terminal)|

### O que é um Sinal?
Sinais são notificações assíncronas enviadas pelo kernel a um processo. Quando um sinal
chega, o processo interrompe o que está a fazer e executa um **handler** (tratador).

Cada sinal tem um **comportamento padrão** (`SIG_DFL`):
- `SIGINT` (Ctrl+C) → **termina** o processo
- `SIGQUIT` (Ctrl+\) → **termina** o processo + gera core dump

Usando `sigaction()`, podemos **substituir** esse comportamento por uma função nossa
ou ignorar o sinal completamente (`SIG_IGN`).

### Porque `write()` e não `printf()` dentro de handlers?
Dentro de um signal handler, só podemos usar funções **async-signal-safe**.
`printf()` usa buffers internos que podem estar corrompidos no momento do sinal.
`write()` escreve diretamente no descritor de ficheiro, sem buffers intermediários.

```c
// ❌ Perigoso dentro de um handler
printf("\n");

// ✅ Seguro dentro de um handler
write(STDOUT_FILENO, "\n", 1);
```

### A variável global `g_signal_received`
O projeto da 42 permite **uma única variável global**. Usamo-la para comunicar ao loop
principal que um sinal foi recebido (já que o handler não pode fazer muito mais):

```c
// Declarada em signals/signals.c
int g_signal_received = 0;

// Declarada em minishell.h como extern
extern int g_signal_received;
```

---

## Como funcionava ANTES (sem tratamento)

```
Ctrl+C → kernel envia SIGINT → SIG_DFL → TERMINA o processo (shell morre)
```

O shell e qualquer comando a correr recebiam `SIGINT` e **morriam todos**.

---

## Como funciona AGORA (com tratamento)

O shell tem **4 contextos diferentes**, cada um com handlers diferentes:

### 1. Prompt Interativo (esperando input)
**Ficheiro**: `main.c` → chama `setup_signals_interactive()` antes do loop

```
Ctrl+C → SIGINT → handle_sigint_interactive()
                      → g_signal_received = SIGINT
                      → write("\n")
                      → rl_on_new_line()
                      → rl_replace_line("", 0)
                      → rl_redisplay()
                      → NÃO termina

Ctrl+\ → SIGQUIT → SIG_IGN → ignorado
```

**No loop do main.c**, após `readline()`, verificamos `g_signal_received`:
```c
if (g_signal_received)
{
    shell->last_exit = 130;   // código de saída para SIGINT
    g_signal_received = 0;    // reset
}
```

### 2. Execução — Processo Pai (esperando filhos)
**Ficheiro**: `executor/executor.c` → chama `setup_signals_exec()` antes dos forks

```
Ctrl+C → SIGINT  → SIG_IGN → pai ignora, continua no waitpid()
Ctrl+\ → SIGQUIT → SIG_IGN → pai ignora, continua no waitpid()
```

O pai **não morre** — espera os filhos terminarem e depois restaura
sinais interativos com `setup_signals_interactive()`.

### 3. Execução — Processo Filho (antes de execve)
**Ficheiro**: `executor/exec_cmd.c` → chama `setup_signals_child()` no início de `exec_child()`

```
Ctrl+C → SIGINT  → SIG_DFL → filho morre (comportamento padrão)
Ctrl+\ → SIGQUIT → SIG_DFL → filho morre + core dump
```

O filho **restaura o comportamento padrão** para que comandos como `sleep`
possam ser interrompidos normalmente.

### 4. Heredoc
**Ficheiro**: (quando implementado) → chama `setup_signals_heredoc()`

```
Ctrl+C → SIGINT  → handle_sigint_heredoc()
                      → g_signal_received = SIGINT
                      → write("\n")
                      → close(STDIN_FILENO) → readline sai do loop
                      → volta ao prompt

Ctrl+\ → SIGQUIT → SIG_IGN → ignorado
```

---

## Tabela resumo

| Contexto          | SIGINT (Ctrl+C)                | SIGQUIT (Ctrl+\)               |
|-------------------|--------------------------------|--------------------------------|
| **Prompt**        | Nova linha + reprompt          | Ignorado                       |
| **Pai (exec)**    | Ignorado (espera filhos)       | Ignorado (espera filhos)       |
| **Filho (exec)**  | Morre (SIG_DFL)                | Morre + core dump (SIG_DFL)    |
| **Heredoc**       | Cancela heredoc, volta ao prompt | Ignorado                     |

---

## Estrutura dos ficheiros

### `signals/signals.c`
Contém as **4 funções de configuração** que mudam o handler conforme o contexto:
- `setup_signals_interactive()` — handlers para o prompt
- `setup_signals_exec()` — ignora tudo (pai espera)
- `setup_signals_child()` — restaura SIG_DFL (filho executa)
- `setup_signals_heredoc()` — handler especial para heredoc

E a variável global:
- `int g_signal_received = 0;`

### `signals/signals_handler.c`
Contém os **2 handlers** (funções chamadas quando o sinal chega):
- `handle_sigint_interactive()` — redesenha o prompt
- `handle_sigint_heredoc()` — fecha stdin para sair do heredoc

### Onde são chamadas no projeto

| Função                        | Ficheiro              | Quando                                |
|-------------------------------|-----------------------|---------------------------------------|
| `setup_signals_interactive()` | `main.c`              | Antes do loop principal               |
| `setup_signals_interactive()` | `executor/executor.c` | Após `wait_all()` (filhos terminaram) |
| `setup_signals_exec()`        | `executor/executor.c` | Antes dos `fork()` no pipeline        |
| `setup_signals_child()`       | `executor/exec_cmd.c` | Início de `exec_child()` (no filho)   |
| `setup_signals_heredoc()`     | (futuro heredoc)      | Antes do loop de leitura do heredoc   |

---

## Fluxo visual completo

```
main()
  │
  ├─ setup_signals_interactive()        ← SIGINT: reprompt, SIGQUIT: ignorado
  │
  └─ while (1)
       │
       ├─ readline("minishell$ ")
       │   ├─ Ctrl+C → handle_sigint_interactive() → "\n" + reprompt
       │   ├─ Ctrl+\ → ignorado
       │   └─ Ctrl+D → readline retorna NULL → shell sai
       │
       ├─ verifica g_signal_received → last_exit = 130
       │
       ├─ lexer → parser → execute()
       │    │
       │    ├─ setup_signals_exec()            ← pai ignora tudo
       │    │
       │    ├─ fork()
       │    │   └─ filho:
       │    │       ├─ setup_signals_child()   ← restaura SIG_DFL
       │    │       └─ execve(comando)
       │    │           ├─ Ctrl+C → filho morre → waitpid recolhe status
       │    │           └─ Ctrl+\ → filho morre + "Quit (core dumped)"
       │    │
       │    ├─ waitpid() → recolhe exit status
       │    │   └─ se filho morreu por SIGINT → last_exit = 130
       │    │   └─ se filho morreu por SIGQUIT → last_exit = 131
       │    │
       │    └─ setup_signals_interactive()     ← restaura handlers do prompt
       │
       └─ volta ao readline
```

---

## Testes para verificar

1. **Ctrl+C no prompt** → nova linha + reprompt (shell não sai)
2. **Ctrl+\ no prompt** → nada acontece
3. **`sleep 10` + Ctrl+C** → sleep morre, `echo $?` = `130`
4. **`sleep 10` + Ctrl+\\** → sleep morre, `echo $?` = `131`
5. **Ctrl+C várias vezes** → sempre reprompt, shell nunca sai
6. **Ctrl+D no prompt** → shell sai normalmente
7. **`cat | cat | cat` + Ctrl+C** → todos morrem, volta ao prompt