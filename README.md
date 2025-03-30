# Sistema de Gerenciamento de Registros

## Descrição
O projeto teve como objetivo fazer um Sequence Set, estrutura de dados projetada para armazenar e gerenciar informações de maneira organizada e eficiente em arquivos, geralmente utilizados para acesso direto a grandes volumes de dados.

## Recursos
1. **Carregar Dados**  
   Importe registros de funcionários a partir de um arquivo CSV.

2. **Adicionar Registros**  
   Insira novos dados manualmente ou importe arquivos CSV adicionais.

3. **Excluir Registros**  
   Remova informações de funcionários fornecendo o ID correspondente.

4. **Localizar Registros**  
   Pesquise dados específicos pelo ID e exiba os detalhes armazenados.

5. **Exibir Todos os Dados**  
   Visualize os blocos com os registros, incluindo informações como o número de registros por bloco e suas conexões.

## Instruções de Uso

### Passo 1: Verifique se tem as ferramentas necessárias
- Certifique-se de que possui um compilador C++ instalado (como o **g++**).
- A base de dados (arquivo CSV) deve estar no mesmo diretório que o programa.

### Passo 2: Compile o Programa
- Gere o executável a partir do código-fonte utilizando o compilador C++:
  ```bash
  g++ -o gerenciar_registros gerenciar_registros.cpp
  
### Passo 3: Execute o Programa

Inicie o software pelo terminal ou linha de comando:

```bash
./gerenciar_registros
```
### Passo 4: Interaja com o Menu

O programa oferece um menu interativo com as seguintes opções:

### Opções do Menu

1. **Adicionar**  
   - Insira novos registros no sistema.  
   - Escolha entre adicionar manualmente ou carregar dados de um arquivo CSV.

2. **Excluir**  
   - Remova registros existentes fornecendo o ID correspondente.

3. **Buscar**  
   - Encontre registros específicos por meio do ID e veja seus detalhes.

4. **Imprimir**  
   - Liste todos os blocos e registros, exibindo informações como a quantidade de registros por bloco e as conexões entre eles.

5. **Encerrar**  
   - Finalize o programa.
