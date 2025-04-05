
# 🔄 Sistema de Gerenciamento de Dados — CRUD com Sequence Set

## 📌 Descrição

Este projeto implementa um **Sequence Set**, uma estrutura de dados projetada para armazenar e gerenciar grandes volumes de informações de forma organizada e eficiente em arquivos. Essa estrutura é especialmente útil em sistemas que exigem acesso direto e rápido aos dados.

---

## 🚀 Funcionalidades

1. **📥 Carregar Dados**  
   Importe registros de funcionários a partir de arquivos CSV.

2. **➕ Adicionar Registros**  
   Insira novos registros manualmente ou importe novos dados de outros arquivos CSV.

3. **🗑️ Excluir Registros**  
   Remova registros de funcionários fornecendo o ID correspondente.

4. **🔍 Localizar Registros**  
   Pesquise registros pelo ID e visualize seus dados completos.

5. **📄 Exibir Todos os Dados**  
   Visualize todos os blocos da estrutura, incluindo a quantidade de registros por bloco e suas conexões.

---

## 🛠️ Instruções de Uso

### 1️⃣ Pré-requisitos

- Ter um compilador C++ instalado (como o **g++**).
- Certificar-se de que os arquivos CSV estão no mesmo diretório do programa.

### 2️⃣ Compilação

Compile o programa utilizando o comando abaixo:

```bash
g++ -o ProjetoPrático ProjetoPrático.cpp
```

### 3️⃣ Execução

Após a compilação, execute o programa no terminal com o comando:

```bash
./ProjetoPrático
```

---

## 🧭 Navegação pelo Menu

Ao executar o programa, será exibido um menu interativo com as seguintes opções:

```
===== MENU =====
1. Adicionar registros (manual ou CSV)
2. Excluir registro
3. Buscar registro
4. Imprimir blocos
5. Sair
Escolha uma opção:
```

### Exemplo de navegação:

#### ✅ Inserção manual:
```
Escolha uma opção: 1
Deseja adicionar manualmente (1) ou importar de CSV (2)? 1

Digite o ID: 105
Digite o nome: João Mendes
Digite o cargo: Desenvolvedor Full Stack
```

#### ✅ Importação de CSV:
```
Escolha uma opção: 1
Deseja adicionar manualmente (1) ou importar de CSV (2)? 2

Digite o nome do arquivo CSV: dados.csv
Importação concluída com sucesso!
```

#### ✅ Exclusão:
```
Escolha uma opção: 2
Digite o ID do funcionário a ser removido: 102
Registro removido com sucesso.
```

#### ✅ Busca:
```
Escolha uma opção: 3
Digite o ID do funcionário: 103

Registro encontrado:
ID: 103
Nome: Mariana Costa
Cargo: Gerente de Projetos
```

#### ✅ Impressão dos blocos:
```
Escolha uma opção: 4

Bloco 0:
[101, Ana Silva, Engenheira de Software]
[103, Mariana Costa, Gerente de Projetos]

Bloco 1:
[105, João Mendes, Desenvolvedor Full Stack]

Total de blocos: 2
```

#### ✅ Saída:
```
Escolha uma opção: 5
Encerrando o programa...
```

---

## 📂 Estrutura de Diretórios

```
├── ProjetoPrático.cpp        # Código-fonte principal  
├── dados.csv                 # Arquivo CSV de exemplo com registros  
└── README.md                 # Documentação do projeto
```

---

## 🧪 Exemplo de Funcionamento

### 📄 Conteúdo do arquivo `dados.csv`:

```csv
101,Ana Silva,Engenheira de Software
102,Carlos Oliveira,Analista de Sistemas
103,Mariana Costa,Gerente de Projetos
```

### 🔍 Busca por ID:

```
Digite o ID do funcionário: 102

Registro encontrado:
ID: 102
Nome: Carlos Oliveira
Cargo: Analista de Sistemas
```

### 🖨️ Impressão dos blocos após importação:

```
Bloco 0:
  [101, Ana Silva, Engenheira de Software]
  [102, Carlos Oliveira, Analista de Sistemas]

Bloco 1:
  [103, Mariana Costa, Gerente de Projetos]

Total de blocos: 2
```

### ❌ Exclusão de Registro:

```
Digite o ID do funcionário a ser removido: 101
Registro removido com sucesso.

Bloco 0:
  [102, Carlos Oliveira, Analista de Sistemas]

Bloco 1:
  [103, Mariana Costa, Gerente de Projetos]
```

---

## 🧠 Conceitos Fundamentais

- **Sequence Set**: Técnica eficiente de indexação baseada em blocos ordenados e encadeados.
- **Encadeamento de Blocos**: Cada bloco mantém ponteiros para o bloco anterior e o próximo.
- **Ordenação por ID**: Registros mantidos em ordem crescente de ID.
- **Split de Bloco**: Quando o bloco fica cheio, ele é dividido.
- **Remoção Lógica**: O espaço pode ser mantido e reuso pode ser feito posteriormente.
- **Importação de CSV**: Agiliza testes com grandes volumes de dados.

---

## 👥 Autores

Projeto desenvolvido por:

- **Diego Oliveira** — [@diegocodehub](https://github.com/diegocodehub)  
- **Gustavo Martins**

🎓 Curso de Ciência da Computação — Universidade Federal de Lavras (UFLA)

---

## 📜 Licença

Distribuído sob a licença [MIT](LICENSE).  
Sinta-se à vontade para estudar, modificar, aprimorar e utilizar este projeto como desejar.

---

## 🤝 Contribuindo

Contribuições são bem-vindas!  
Abra uma *issue* com sugestões, relate problemas ou envie um *pull request* com melhorias.

---

> 💡 *"Soluções simples para problemas complexos começam com boas estruturas de dados."*
