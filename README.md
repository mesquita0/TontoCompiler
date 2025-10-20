# TontoCompiler

### Analisador Léxico para a Linguagem **TONTO** (Textual Ontology Language)

---
## - Descrição do Projeto
Projeto de um analisador léxico desenvolvido em **C++**, utilizando a ferramenta **Flex**, com o objetivo de reconhecer e classificar os elementos da linguagem **TONTO (Textual Ontology Language)**.  

 TONTO é uma linguagem textual para especificação de **ontologias computacionais**, ou seja, representações formais de conhecimento compostas por conceitos e suas relações. 

Este projeto foi desenvolvido como parte da disciplina **Compiladores** da **Universidade Federal Rural do Semi-Árido (UFERSA)**, sob orientação do professor **Pátricio de Alencar Silva**.
---

## - Objetivo
Implementar um **analisador léxico** capaz de:
- Ler arquivos escritos na linguagem TONTO;
- Identificar e classificar corretamente tokens válidos;
- Tratar erros léxicos com indicação de linha e coluna;
- Gerar uma **tabela de síntese** mostrando a contagem de cada tipo de token reconhecido.

---

## - Principais Componentes
### - `lexer.l`
 Define as regras léxicas (padrões de tokens da linguagem TONTO).

---

### - `token.h`
 Define o tipo e estrutura dos tokens (TokenClass, Token).
---

###  `scanner.h` e `scanner.cpp`
Implementa a classe Scanner, que executa o Flex e armazena tokens.
---

###  `main.cpp`
Programa principal: executa o scanner, imprime tokens e tabela de síntese.

---

## - Como Compilar:
 - Comando para compilar o projeto: ``cmake --build "./out/build/GCC 13.3.0 x86_64-linux-gnu"``

 ## - Como Executar: 
 - O analisador lê um arquivo .tonto ou texto da entrada padrão (stdin);
 - Comando para executar o projeto: ``"./out/build/GCC 13.3.0 x86_64-linux-gnu/src/TontoCompiler" < test.tonto``


## - Autores:
- João Pedro Souza Cavalcante;
- Kaynã Moreira Mesquita;