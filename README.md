# TontoCompiler

### Analisador Sntático para a Linguagem **TONTO** (Textual Ontology Language)

---
## - Descrição do Projeto
Este projeto consiste em um analisador sintático desenvolvido em **C++**, utilizando as ferramentas **Flex** (para análise léxica) e **Bison** (para análise sintática). O objetivo é validar a estrutura gramatical de arquivos escritos na linguagem **TONTO (Textual Ontology Language)** 

**TONTO** é uma linguagem textual declarativa para especificação de **ontologias computacionais** baseadas em OntoUML, permitindo a definição de classes, relações, generalizações e outros construtos ontológicos

Este projeto foi desenvolvido como parte da disciplina **Compiladores** da **Universidade Federal Rural do Semi-Árido (UFERSA)**, sob orientação do professor **Pátricio de Alencar Silva**.
---

## - Objetivo
Implementar um **analisador sintático** capaz de:
- **Validar a estrutura** de especificações TONTO (Pacotes, Classes, Relações, Enums, Gensets)
- **Recuperação de Erros:** Identificar erros sintáticos e continuar a análise sem abortar o programa
- **Sugestão de Correção:** Fornecer mensagens de erro amigáveis com sugestões práticas para o usuário
- Gerar uma **tabela de síntese** mostrando a contagem de cada tipo de token reconhecido (quantidade de classes, pacotes, enums, etc).

---

## - Funcionalidades Suportadas
O analisador verifica a corretude dos seguintes construtos da linguagem:
1.  **Pacotes:** Namespaces que agrupam os elementos da ontologia.
2.  **Classes:** Definições com estereótipos (ex: `kind`, `role`, `phase`) e atributos tipados.
3.  **Tipos de Dados:** Tipos nativos e criação de `datatype` customizados.
4.  **Enumerações:** Classes com valores finitos pré-definidos.
5.  **Generalizações (Gensets):** Estruturas de herança (`general`/`specifics`) com restrições de `disjoint` e `complete`.
6.  **Relações:** Declarações de relacionamentos internos e externos com cardinalidades e estereótipos (ex: `@mediation`, `@material`).

## - Principais Componentes

### `parser.y` (Bison)
Arquivo do Bison com a gramática e lógica de tratamento de erros.

### `lexer.l` (Flex)
Define as regras léxicas e expressões regulares para reconhecer os tokens da linguagem.

---
###  `AST.h` e `AST.cpp`
Implementação da Árvore Sintática Abstrata e tabelas de símbolos.
---

###  `main.cpp`
Configura o parser, inicia a análise e exibe os relatórios finais.

---

## - Como Compilar:
 -O projeto utiliza **CMake**. Para compilar, execute o seguinte comando na raiz do projeto:
 ``cmake --build "./out/build/GCC 13.3.0 x86_64-linux-gnu"``

 ## - Como Executar: 
 - O analisador lê até dois arquivos .tonto ou texto da entrada padrão (stdin);
 - Comando para executar o projeto: ``"./out/build/GCC 13.3.0 x86_64-linux-gnu/src/TontoCompiler" < test.tonto``
 


## - Autores:
- João Pedro Souza Cavalcante;
- Kaynã Moreira Mesquita;