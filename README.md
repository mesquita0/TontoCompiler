# TontoCompiler

### Analisador Léxico para a Linguagem **TONTO** (Textual Ontology Language)

---
## 📘 Descrição do Projeto
O **TontoCompiler** é um analisador léxico desenvolvido em **C++**, utilizando a ferramenta **Flex**, com o objetivo de reconhecer e classificar os elementos da linguagem **TONTO (Textual Ontology Language)**.  

A TONTO é uma linguagem textual para especificação de **ontologias computacionais**, ou seja, representações formais de conhecimento compostas por conceitos e suas relações. 

Este projeto foi desenvolvido como parte da disciplina **Compiladores** da **Universidade Federal Rural do Semi-Árido (UFERSA)**, sob orientação do professor **Pátricio de Alencar Silva**.
---

## 🎯 Objetivo
Implementar um **analisador léxico** capaz de:
- Ler arquivos escritos na linguagem TONTO;
- Identificar e classificar corretamente tokens válidos;
- Tratar erros léxicos com indicação de linha e coluna;
- Gerar uma **tabela de síntese** mostrando a contagem de cada tipo de token reconhecido.

---

## Principais Componentes
### 🔹 `lexer.l`
Contém as **regras léxicas** que definem os tokens reconhecidos pelo analisador, incluindo:
- **Palavras reservadas**: `genset`, `where`, `package`, etc.
- **Estereótipos de classe**: `event`, `process`, `kind`, `role`, ...
- **Estereótipos de relação**: `material`, `aggregation`, `composition`, ...
- **Tipos de dados**: `number`, `string`, `date`, ...
- **Identificadores** de classe, relação e instância (com regras específicas de formatação).
- **Símbolos**: `{`, `}`, `(`, `)`, `..`, `<>--`, etc.

Tokens inválidos são marcados como `INVALID` e reportados com o número da linha e coluna.

---

### 🔹 `token.h`
Define:
- O **enum `TokenClass`**, com as categorias de tokens (ex.: `RESERVED_KEYWORD`, `SYMBOL`, `CLASS_ID`, etc.);
- A **classe `Token`**, que armazena:
  - o tipo do token,
  - o lexema (texto original),
  - linha e coluna em que foi encontrado.

---

### 🔹 `scanner.h` e `scanner.cpp`
Implementam a classe **`Scanner`**, responsável por:
- Executar o analisador gerado pelo Flex;
- Armazenar todos os tokens encontrados;
- Tratar erros e gerar mensagens detalhadas.

---

### 🔹 `main.cpp`
É o programa principal:
- Instancia o `Scanner`;
- Executa a análise léxica;
- Imprime todos os tokens reconhecidos com tipo, lexema, linha e coluna;
- Exibe uma **tabela de síntese** com o total de cada categoria de token.

---

## 🧱 Como Compilar
 - Comando para compilar o projeto: cmake --build "./out/build/GCC 13.3.0 x86_64-linux-gnu"

 ## ▶️ Como Executar 
 - O analisador lê um arquivo .tonto ou texto da entrada padrão (stdin);
 - Comando para executar o projeto: "./out/build/GCC 13.3.0 x86_64-linux-gnu/src/TontoCompiler" < test.tonto


## 🧑‍💻 Autores
- João Pedro Souza Cavalcante;
- Kaynã Moreira Mesquita;