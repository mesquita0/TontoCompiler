# TontoCompiler

### Analisador Sintático e Semântico para a Linguagem **TONTO** (Textual Ontology Language)

---
## - Descrição do Projeto
Este projeto consiste em um compilador desenvolvido em **C++**, utilizando as ferramentas **Flex** (para análise léxica) e **Bison** (para análise sintática). O objetivo é validar a estrutura gramatical e lógica de arquivos escritos na linguagem **TONTO**.

**TONTO** é uma linguagem textual declarativa para especificação de **ontologias computacionais** baseadas em OntoUML, permitindo a definição de classes, relações, generalizações e outros construtos ontológicos.

Este projeto foi desenvolvido como parte da disciplina **Compiladores** da **Universidade Federal Rural do Semi-Árido (UFERSA)**, sob orientação do professor **Pátricio de Alencar Silva**.
---

## - Objetivo
1.  **Análise Léxica e Sintática:**
    * Validar a estrutura de especificações TONTO (Pacotes, Classes, Relações, Enums, Gensets).
    * **Recuperação de Erros:** Identificar erros sintáticos e continuar a análise sem abortar o programa (Panic Mode).
    * **Sugestão de Correção:** Fornecer mensagens de erro amigáveis com sugestões práticas.

2.  **Análise Semântica**
    * Identificação de **Padrões de Projeto de Ontologias (ODPs)**.
    * Aplicação de **Coerção** para correção automática de erros leves.
    * **Dedução** de estruturas implícitas (Overloading).

---

## - Funcionalidades Suportadas

### 1. Estruturas da Linguagem (Sintaxe)
O analisador verifica a corretude dos seguintes construtos da linguagem:
1.  **Pacotes:** Namespaces que agrupam os elementos da ontologia.
2.  **Classes:** Definições com estereótipos (ex: `kind`, `role`, `phase`) e atributos tipados.
3.  **Tipos de Dados:** Tipos nativos e criação de `datatype` customizados.
4.  **Enumerações:** Classes com valores finitos pré-definidos.
5.  **Generalizações (Gensets):** Estruturas de herança (`general`/`specifics`) com restrições de `disjoint` e `complete`.
6.  **Relações:** Declarações de relacionamentos internos e externos com cardinalidades e estereótipos (ex: `@mediation`, `@material`).

### 2. Validação Semântica e Padrões (ODPs)
O analisador identifica e valida semanticamente os seguintes padrões estruturais:
* **Subkind Pattern**
* **Role Pattern**
* **Phase Pattern**
* **Relator Pattern**
* **Mode Pattern**
* **RoleMixin Pattern**

## - Estrutura do Projeto

* `src/`: Código fonte C++ (`main.cpp`, `SemanticAnalyzer.cpp`, `AST.cpp`), Lexer (`lexer.l`) e Parser (`parser.y`).
* `tests/`: Arquivos de exemplo `.tonto` cobrindo todos os padrões e casos de teste.
* `CMakeLists.txt`: Configuração de build do projeto.

---

## - Pré-requisitos

Para compilar e rodar, você precisará das seguintes ferramentas (Linux/WSL):

* **C++ Compiler** (g++)
* **CMake** (versão 3.10+)
* **Flex**
* **Bison**

---

## - Como Compilar:
-O projeto utiliza **CMake**. Para compilar, execute o seguinte comando na raiz do projeto:
# 1. Gere os arquivos de configuração na pasta 'build'
cmake -S . -B build
# 2. Compile o executável
cmake --build build


 ## - Como Executar: 
 - O analisador lê arquivos .tonto;
 #  Comando para executar o projeto: 
 ./build/src/TontoCompiler tests/nome_do_arquivo.tonto
 

## 📄 Exemplo de Saída

Abaixo está o resultado real da execução do analisador sobre o arquivo de teste `tests/relator.tonto`:

```text
===================================================
 ARQUIVO: tests/relator.tonto

Pattern Identified: Relator
Element: Relator_Name
Relations: (mediation -> Role_Name1), (mediation -> Role_Name2)

===================================================


## - Autores:
- João Pedro Souza Cavalcante;
- Kaynã Moreira Mesquita;
