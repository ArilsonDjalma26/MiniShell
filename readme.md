# 🐚 SUPASHELL — Minishell

*This project has been created as part of the 42 curriculum by elfranco, aalbano.*

## Description

This project is a custom implementation of a simple shell, called **minishell**, developed as part of the 42 curriculum. The goal is to create a functional shell that can interpret and execute user commands, handle built-in commands, manage environment variables, and support features such as piping and redirections, mimicking the behavior of GNU Bash.

## Instructions

### Compilation
To compile the project, run:
```
make
```
This will generate the `minishell` executable in the project directory.

### Execution
To start the shell, run:
```
./minishell
```

### Example Usage
```
$ ./minishell
minishell$ echo Hello World
Hello World
minishell$ pwd
/home/user/minishell
minishell$ exit
```

## Resources
- [GNU Bash Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [UNIX Shell Programming](https://www.tutorialspoint.com/unix/unix-shell.htm)
- [42 Subject PDF](https://cdn.intra.42.fr/pdf/pdf/198638/en.subject.pdf)
- [man bash](https://man7.org/linux/man-pages/man1/bash.1.html)

## Authors
- elfranco
- aalbano

---

# 📊 Progress Report — ~65%

---

## ✅ O que está FEITO e funcionando

| Módulo | Status | Nota |
|--------|--------|------|
| **Lexer (Tokenizer)** | ✅ Completo | Tokeniza palavras, operadores, aspas, `$vars` |
| **Parser** | ✅ Completo | Gera lista de comandos com args, redireções e pipes |
| **Expander** | ✅ Completo | Expande `$VAR`, `$?`, `$0`, aspas simples/duplas |
| **Executor — Comandos simples** | ✅ Completo | `fork` + `execve` com busca em PATH |
| **Executor — Pipes** | ✅ Completo | Pipeline com múltiplos pipes, `waitpid` correto |
| **Executor — Redireções** | ✅ Completo | `<`, `>`, `>>` implementados |
| **Heredoc** | ⚠️ Parcial | Apenas escreve conteúdo via pipe, **não lê input interativamente** |
| **Built-in: echo** | ✅ Completo | Suporta `-n` e múltiplos `-nnn` |
| **Built-in: cd** | ✅ Completo | Com HOME e tratamento de erros |
| **Built-in: pwd** | ✅ Completo | |
| **Built-in: env** | ✅ Completo | |
| **Built-in: export** | ✅ Completo | Com `sort`, validação de identificadores |
| **Built-in: unset** | ✅ Completo | |
| **Built-in: exit** | ✅ Completo | Com validação numérica |
| **Free/cleanup de structs** | ✅ Completo | `free_tokens`, `free_cmd_list`, etc. |

---

## ❌ O que está FALTANDO

| Funcionalidade | Impacto | Detalhes |
|---------------|---------|----------|
| **Sinais (SIGINT, SIGQUIT, SIGEOF)** | 🔴 Alto | `Ctrl+C`, `Ctrl+\`, `Ctrl+D` — nenhum signal handler implementado |
| **Heredoc interativo (`<<`)** | 🔴 Alto | O heredoc nunca lê linhas do stdin até encontrar o delimiter |
| **`cd` não atualiza PWD/OLDPWD** | 🟡 Médio | Após `chdir()`, as variáveis de ambiente `PWD` e `OLDPWD` não são atualizadas |
| **`cd -` (voltar ao diretório anterior)** | 🟡 Médio | Não implementado |
| **`export` não atualiza var existente** | 🟡 Médio | `push_env` sempre adiciona ao final — variáveis ficam duplicadas |
| **`export` sem `=` não deve criar valor** | 🟡 Baixo | `export VAR` (sem `=`) deveria apenas marcar como exported |
| **Validação de sintaxe do lexer/parser** | 🟡 Médio | Erros de sintaxe chamam `exit(1)` no processo pai em vez de setar `last_exit = 2` |
| **Memory leaks no expander** | 🟡 Médio | Várias chamadas a `ft_strjoin` sem `free` do ponteiro antigo |
| **Aspas não fechadas** | ⚠️ Parcial | Atualmente chama `exit(1)` — deveria mostrar erro e continuar |
| **Variável global para sinais** | 🔴 Alto | O subject exige no máximo 1 variável global (para signal received) — não existe |
| **Free geral ao sair** | 🟡 Médio | Ao fazer `Ctrl+D`, o `shell` e `envs` não são liberados |

---

## 🐛 Bugs Identificados

1. **`parse_redirs` / `parse_pipe` chamam `exit(1)`** no processo pai — deveriam retornar erro e continuar o prompt
2. **Memory leaks** em `ft_strjoin` encadeados (ex: `cd.c`, `export.c`, `exit.c`, `print_error`)
3. **`export` duplica variáveis** — `push_env` não verifica se a key já existe
4. **`is_valid_identifier`** não aceita dígitos no meio do nome (deveria aceitar `ft_isalnum`, não só `ft_isalpha`)
5. **Heredoc** não funciona como esperado — precisa de leitura interativa com `readline` ou `get_next_line`

---

## 📈 Resumo por Área

| Área | Peso no projeto | Progresso |
|------|----------------|-----------|
| Lexer/Tokenizer | 15% | ✅ 95% |
| Parser | 15% | ✅ 85% |
| Expander | 15% | ✅ 85% |
| Executor + Pipes | 20% | ✅ 90% |
| Built-ins (7) | 15% | ✅ 80% |
| Sinais | 10% | ❌ 0% |
| Heredoc interativo | 5% | ❌ 10% |
| Robustez/Leaks | 5% | ⚠️ 40% |

---

## 🎯 Prioridades para chegar a 100%

1. **Sinais** — `signal(SIGINT, ...)`, `signal(SIGQUIT, SIG_IGN)` — obrigatório
2. **Heredoc interativo** — ler do stdin até encontrar o delimiter
3. **Corrigir `exit(1)` no parser** — erros de sintaxe não devem matar o shell
4. **Corrigir `export`** — atualizar vars existentes + validação de identifier com dígitos
5. **Atualizar PWD/OLDPWD no `cd`**
6. **Corrigir memory leaks** — especialmente `ft_strjoin` sem free
7. **Free ao sair** com `Ctrl+D`