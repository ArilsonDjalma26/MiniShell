*Este projeto foi criado como parte do currículo da 42 por aalbano, elfranco.*

# 🐚 Minishell

## Descrição

O Minishell é uma shell Unix simplificada escrita em C, desenvolvida como parte do currículo da escola 42. O objetivo deste projeto é implementar um interpretador de linha de comandos funcional que replica os comportamentos principais do **GNU Bash**, proporcionando uma compreensão mais profunda de como as shells funcionam por dentro — incluindo criação de processos, gestão de file descriptors, tratamento de sinais e expansão de variáveis de ambiente.

A shell lê o input do utilizador através de um prompt interativo, tokeniza e faz o parsing da linha de comandos, expande variáveis e aspas, e depois executa os comandos resultantes — seja como operações built-in ou criando processos filhos com `execve`.

### Funcionalidades

- **Prompt interativo** com histórico de comandos (usando `readline`)
- **Lexer/Tokenizer** — divide o input em tokens (palavras, operadores, aspas, variáveis de ambiente)
- **Parser** — constrói uma lista ligada de comandos com argumentos, redireções e cadeias de pipes
- **Expansão de variáveis** — expande `$VAR`, `$?` (último exit status), e trata a semântica de aspas simples/duplas
- **Pipes** — suporta pipelines com múltiplos comandos (`cmd1 | cmd2 | cmd3`)
- **Redireções** — input (`<`), output (`>`), append (`>>`), e heredoc (`<<`)
- **Tratamento de sinais** — `Ctrl+C`, `Ctrl+\`, e `Ctrl+D` comportam-se de forma semelhante ao Bash
- **Comandos built-in:**
  | Comando  | Descrição                                      |
  |----------|-------------------------------------------------|
  | `echo`   | Imprime texto (suporta flag `-n`)               |
  | `cd`     | Muda o diretório de trabalho                    |
  | `pwd`    | Imprime o diretório de trabalho atual           |
  | `export` | Define/mostra variáveis de ambiente             |
  | `unset`  | Remove variáveis de ambiente                    |
  | `env`    | Mostra o ambiente atual                         |
  | `exit`   | Sai da shell com código de saída opcional       |

### Arquitetura

O projeto está organizado nos seguintes módulos:

```
.
├── main.c                 # Ponto de entrada e loop principal
├── initializers.c         # Inicialização da shell e do ambiente
├── validators.c           # Validação de argumentos
├── minishell.h            # Header com todas as structs, enums e protótipos
├── lexer/                 # Tokenização do input bruto
├── parser/                # Parsing de comandos e redireções
├── expander/              # Expansão de variáveis e aspas
├── executor/              # Execução de comandos, pipes e redireções
│   └── built-in/          # Implementações dos comandos built-in
├── signals/               # Configuração e handlers de sinais
└── utils/                 # Funções utilitárias próprias (ft_split, ft_strdup, etc.)
```

## Instruções

### Pré-requisitos

- Um sistema operativo Unix-like (Linux ou macOS)
- `cc` (compilador C, ex: `gcc` ou `clang`)
- `make`
- Biblioteca `readline` instalada
  - No Debian/Ubuntu: `sudo apt install libreadline-dev`
  - No macOS (Homebrew): `brew install readline`

### Compilação

Clona o repositório e compila o projeto:

```sh
git clone <url-do-repositorio> minishell
cd minishell
make
```

Regras disponíveis no Makefile:

| Regra       | Descrição                                  |
|-------------|--------------------------------------------|
| `make`      | Compila o projeto                          |
| `make clean`  | Remove ficheiros objeto                  |
| `make fclean` | Remove ficheiros objeto e o binário      |
| `make re`   | Recompilação completa (fclean + all)       |

### Execução

```sh
./minishell
```

Serás recebido com um prompt interativo:

```
minishell$
```

### Exemplos de Utilização

```sh
minishell$ echo "Hello, World!"
Hello, World!

minishell$ ls -la | grep minishell
-rwxr-xr-x 1 user user 42000 Mar  6 12:00 minishell

minishell$ export MY_VAR="42 Porto"
minishell$ echo $MY_VAR
42 Porto

minishell$ cat < input.txt > output.txt

minishell$ echo "Exit status: $?"
Exit status: 0

minishell$ exit
```

## Recursos

### Referências

- [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html) — documentação oficial sobre o comportamento e sintaxe do Bash
- [bash(1) — Linux man page](https://man7.org/linux/man-pages/man1/bash.1.html) — página de manual detalhada do Bash
- [The Open Group Base Specifications — Shell Command Language](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html) — especificação POSIX da shell
- [Writing a Shell in C (Stephen Brennan)](https://brennan.io/2015/01/16/write-a-shell-in-c/) — tutorial introdutório sobre como construir uma shell Unix de raiz
- [Pipes, Forks, & Dups: Understanding Command Execution and I/O](https://www.rozmichelle.com/pipes-forks-dups/) — guia visual sobre pipes e file descriptors
- [readline(3) — GNU Readline Library](https://man7.org/linux/man-pages/man3/readline.3.html) — documentação da biblioteca readline usada para o prompt interativo

### Uso de IA

Ferramentas de IA (GitHub Copilot) foram utilizadas durante o desenvolvimento deste projeto da seguinte forma:

- **Assistência em debugging** — identificação de memory leaks, tratamento incorreto de file descriptors e edge cases na gestão de sinais.
- **Revisão de código e refactoring** — sugestões de melhorias na estrutura do código, convenções de nomes e modularização.
- **Documentação** — assistência na escrita e formatação deste ficheiro README.

Toda a lógica principal — incluindo o lexer, parser, expander, executor e comandos built-in — foi desenhada, implementada e compreendida pelos autores. A IA não foi utilizada para gerar a arquitetura fundamental ou as soluções algorítmicas do projeto.

## Autores

- **aalbano** — [42 Intra](https://profile.intra.42.fr/users/aalbano)
- **elfranco** — [42 Intra](https://profile.intra.42.fr/users/elfranco)
