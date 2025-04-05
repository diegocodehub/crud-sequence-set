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

bash
g++ -o ProjetoPrático ProjetoPrático.cpp


### 3️⃣ Execução

Após a compilação, execute o programa no terminal com o comando:

bash
./ProjetoPrático.cpp


---

## 🧭 Navegação pelo Menu

Ao executar o programa, será exibido um menu interativo com as seguintes opções:

1. **Adicionar Registros**  
   - Permite a inserção manual de novos funcionários ou a importação a partir de um novo arquivo CSV.

2. **Excluir Registros**  
   - Remove um registro existente ao informar o ID correspondente.

3. **Buscar Registro**  
   - Pesquisa um registro específico com base no ID e exibe os detalhes completos.

4. **Imprimir Blocos**  
   - Mostra todos os blocos da estrutura Sequence Set, incluindo:
     - Registros armazenados em cada bloco
     - Número de registros por bloco
     - Conexões entre os blocos (próximo/anterior)

5. **Encerrar Programa**  
   - Finaliza a execução do sistema de forma segura.

---

## 📂 Estrutura de Diretórios

```markdown
├── ProjetoPrático.cpp        # Código-fonte principal  
├── dados.csv                 # Arquivo CSV de exemplo com registros  
└── README.md                 # Documentação do projeto
```


---

## 🧪 Exemplo de Funcionamento

### 📄 Exemplo de arquivo dados.csv

csv
101,Ana Silva,Engenheira de Software
102,Carlos Oliveira,Analista de Sistemas
103,Mariana Costa,Gerente de Projetos


### 🔍 Exemplo de busca por ID no terminal

plaintext
Digite o ID do funcionário: 102

Registro encontrado:
ID: 102
Nome: Carlos Oliveira
Cargo: Analista de Sistemas


### 🖨️ Exemplo de impressão de blocos

plaintext
Bloco 0:
  [101, Ana Silva, Engenheira de Software]
  [102, Carlos Oliveira, Analista de Sistemas]

Bloco 1:
  [103, Mariana Costa, Gerente de Projetos]

Total de blocos: 2


---

## 🧠 Conceitos Fundamentais

O projeto utiliza diversos conceitos importantes de estruturas de dados e organização de arquivos:

- **Sequence Set**: Técnica eficiente de indexação baseada em blocos ordenados e encadeados, ideal para armazenar registros com chave de busca (ID).
- **Encadeamento de Blocos**: Cada bloco mantém ponteiros para o bloco anterior e para o próximo, facilitando a navegação sequencial.
- **Ordenação por ID**: Os registros são mantidos em ordem crescente de ID dentro de cada bloco, o que acelera as operações de busca e inserção.
- **Divisão de Blocos (Split)**: Quando um bloco atinge sua capacidade máxima, ele é dividido em dois para preservar o balanceamento da estrutura.
- **Remoção Lógica**: A exclusão de registros pode manter o espaço no bloco até que reorganizações futuras sejam feitas.
- **Importação de Arquivos CSV**: Permite carregar dados externos de forma rápida, ideal para testes com grandes volumes.

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
