#include <cstdlib>
 #include <cstring>
 #include <fstream>
 #include <sstream>
 #include <iostream>
 #include <string>
 #include <cstdio>
 
 using namespace std;
 
 const int tamanho = 50;
 
 //Armazena as informações de cada funcionário.
 struct dado {
     int id;
     char name[50];
     char job[50];
     float basePay;
     float overtimePay;
     float otherPay;
     float benefits;
     float totalPay;
     float totalPayBenefits;
     int year;
 };
 
 //Gerencia os pedaços que armazenam os registros.
 struct sequenceSet {
     int numeroDePedacos = 6;
     int primeiroPedaco = 1;
     int ultimoPedaco = 6;
     int proximoPedacoDisponivel = -1;
 };
 
 //Gerencia informações de um bloco específico.
 struct pedaco {
     int numeroDeRegistros = 0;
     int pedacoAnterior = -1;
     int proximoPedaco = -1;
 };
 //cria os arquivos binarios
 void criarPedaco(int &count) {
     count++;
     pedaco pedaco;
     pedaco.numeroDeRegistros = 0;
     pedaco.pedacoAnterior = count - 1;
     pedaco.proximoPedaco = count + 1;
 
     string numArquivo = to_string(count);
     ofstream arquivo(numArquivo + ".bin", ios::binary);
 
     if (arquivo.is_open()) {
 
     arquivo.write(reinterpret_cast<const char *>(&pedaco.numeroDeRegistros), sizeof(int));
     arquivo.write(reinterpret_cast<const char *>(&pedaco.pedacoAnterior), sizeof(int));
     arquivo.write(reinterpret_cast<const char *>(&pedaco.proximoPedaco), sizeof(int));
 
     } else {
         cout << "Erro ao abrir arquivo" << endl;
     }
     arquivo.close();
 }
 //Funçao para adicionar um dado ao arquivo binario de forma ordenada
 void insereNoPedaco(dado novoDado, sequenceSet &sequenceSet) {
     dado primeiro;
     pedaco blocoAtual;
     bool achou = false;
 
     int i = 1;
     int cont = 0;
     while (cont < sequenceSet.numeroDePedacos && achou == false) {
           ifstream arq(to_string (i) + ".bin", ios::binary);
 
           arq.seekg(0);
           arq.read(reinterpret_cast<char*>(&blocoAtual.numeroDeRegistros), sizeof(blocoAtual.numeroDeRegistros));
           arq.read(reinterpret_cast<char*>(&blocoAtual.pedacoAnterior ), sizeof(blocoAtual.pedacoAnterior));
           arq.read(reinterpret_cast<char*>(&blocoAtual.proximoPedaco), sizeof(blocoAtual.proximoPedaco));
           arq.read(reinterpret_cast<char*>(&primeiro), sizeof(dado));
 
           if(novoDado.id < primeiro.id) {
               achou = true;
           }
 
           arq.close();
           i = blocoAtual.proximoPedaco;
           cont++;
     }
 
     int pedacoEncontrado;
     if (achou == false) {
            pedacoEncontrado = sequenceSet.ultimoPedaco;
     } else {
         pedacoEncontrado =blocoAtual.pedacoAnterior;
     }
 
     fstream arqBlocoAtual(to_string (pedacoEncontrado) + ".bin", ios::binary | ios::in | ios::out);
 
     arqBlocoAtual.read(reinterpret_cast<char*>(&blocoAtual.numeroDeRegistros), sizeof(blocoAtual.numeroDeRegistros));
     arqBlocoAtual.read(reinterpret_cast<char*>(&blocoAtual.pedacoAnterior ), sizeof(blocoAtual.pedacoAnterior));
     arqBlocoAtual.read(reinterpret_cast<char*>(&blocoAtual.proximoPedaco), sizeof(blocoAtual.proximoPedaco));
 
     dado vetorD[blocoAtual.numeroDeRegistros+2];
     for (int i = 0; i < blocoAtual.numeroDeRegistros; i++) {
         arqBlocoAtual.read(reinterpret_cast<char*>(&vetorD[i]), sizeof(dado));
     }
 
     ofstream arqLimparAtual(to_string (pedacoEncontrado) + ".bin", ios::binary | ios::out |ios::trunc);
 
     int j = blocoAtual.numeroDeRegistros - 1;
     while (j >= 0 && vetorD[j].id > novoDado.id) {
         vetorD[j + 1] = vetorD[j];
         j--;
     }
     vetorD[j+1] = novoDado;
 
 
     if (blocoAtual.numeroDeRegistros == tamanho) {
         int metade = (blocoAtual.numeroDeRegistros+1)/ 2;
         pedaco novoBloco;
 
         novoBloco.numeroDeRegistros = metade;
         novoBloco.pedacoAnterior = pedacoEncontrado;
         novoBloco.proximoPedaco = blocoAtual.proximoPedaco;
 
 
         int diff = sequenceSet.numeroDePedacos+1;
         ofstream arqNovoBloco (to_string(diff) + ".bin", ios::binary | ios::out);
 
         if (arqNovoBloco.is_open()) {
             arqNovoBloco.seekp(0);
             arqNovoBloco.write(reinterpret_cast<const char *>(&novoBloco.numeroDeRegistros), sizeof(novoBloco.numeroDeRegistros));
             arqNovoBloco.write(reinterpret_cast<const char *>(&novoBloco.pedacoAnterior), sizeof(novoBloco.pedacoAnterior));
             arqNovoBloco.write(reinterpret_cast<const char *>(&novoBloco.proximoPedaco), sizeof(novoBloco.proximoPedaco));
 
             for (int i = metade+1; i <= tamanho; i++) {
                 arqNovoBloco.write(reinterpret_cast<char*>(&vetorD[i]), sizeof(dado));
             }
         }
         int atualProxIndice = blocoAtual.proximoPedaco;
 
         blocoAtual.numeroDeRegistros = metade +1;
         blocoAtual.proximoPedaco = sequenceSet.numeroDePedacos+ 1;
 
         if (arqLimparAtual.is_open()){
             arqLimparAtual.seekp(0);
             arqLimparAtual.write(reinterpret_cast<char*>(&blocoAtual.numeroDeRegistros), sizeof(blocoAtual.numeroDeRegistros));
             arqLimparAtual.write(reinterpret_cast<char*>(&blocoAtual.pedacoAnterior), sizeof(blocoAtual.pedacoAnterior));
             arqLimparAtual.write(reinterpret_cast<char*>(&blocoAtual.proximoPedaco), sizeof(blocoAtual.proximoPedaco));
 
             for (int i = 0; i <= metade; i++) {
                 arqLimparAtual.write(reinterpret_cast<char*>(&vetorD[i]), sizeof(dado));
             }
         }
 
 
         pedaco atualProx;
 
         fstream arqAtualProx (to_string (atualProxIndice) + ".bin", ios:: binary| ios:: in |ios :: out);
 
         arqAtualProx.read(reinterpret_cast<char*>(&atualProx.numeroDeRegistros), sizeof(atualProx.numeroDeRegistros));
         arqAtualProx.read(reinterpret_cast<char*>(&atualProx.pedacoAnterior ), sizeof(atualProx.pedacoAnterior));
 
         atualProx.pedacoAnterior = sequenceSet.ultimoPedaco+1;
         arqAtualProx.seekp(0);
         arqAtualProx.write(reinterpret_cast<char*>(&atualProx.numeroDeRegistros), sizeof(atualProx.numeroDeRegistros));
         arqAtualProx.write(reinterpret_cast<char*>(&atualProx.pedacoAnterior), sizeof(atualProxIndice));
 
 
         sequenceSet.numeroDePedacos++;
         if(achou == false){
             sequenceSet.ultimoPedaco++;
         }
     } else {
         blocoAtual.numeroDeRegistros ++;
 
         if (arqLimparAtual.is_open()){
             arqLimparAtual.seekp(0);
             arqLimparAtual.write(reinterpret_cast<const char *>(&blocoAtual.numeroDeRegistros), sizeof(int));
             arqLimparAtual.write(reinterpret_cast<const char *>(&blocoAtual.pedacoAnterior), sizeof(int));
             arqLimparAtual.write(reinterpret_cast<const char *>(&blocoAtual.proximoPedaco), sizeof(int));
 
             for(int i = 0; i < blocoAtual.numeroDeRegistros; i++) {
             arqLimparAtual.write(reinterpret_cast<char*>(&vetorD[i]), sizeof(dado));
             }
         }
     }
 }
 
 //Funçao para transformar o primeiro arquivo csv em binario
 void transformaBinario(dado umdado, int &cont) {
     int arqIndice;
     if (cont == 0) {
         arqIndice = 1;
     } else {
         arqIndice = cont / tamanho + 1;
     }
 
     // Abrir o arquivo em modo de adição
     ofstream arquivo(to_string(arqIndice) + ".bin", ios::binary | ios::app);
     if (arquivo.is_open()) {
         arquivo.write(reinterpret_cast<const char *>(&umdado), sizeof(umdado));
         cont++;
 
         // Atualizar o número de registros no pedaço
         pedaco blocoAtual;
         ifstream arq(to_string(arqIndice) + ".bin", ios::binary);
         arq.read(reinterpret_cast<char*>(&blocoAtual.numeroDeRegistros), sizeof(blocoAtual.numeroDeRegistros));
         blocoAtual.numeroDeRegistros++;
         arq.close();
 
         // Reabrir o arquivo para atualizar o número de registros
         ofstream arqAtualiza(to_string(arqIndice) + ".bin", ios::binary | ios::in | ios::out);
         arqAtualiza.seekp(0); // Voltar ao início do arquivo
         arqAtualiza.write(reinterpret_cast<const char *>(&blocoAtual.numeroDeRegistros), sizeof(blocoAtual.numeroDeRegistros));
         arqAtualiza.close();
     } else {
         cout << "Erro ao abrir arquivo para escrita" << endl;
     }
 }
 
 //Busca binaria implementada de forma recursiva, para ser mais eficiente
 int buscaBinariaRecursiva(dado vetorD[],int inicio, int fim, int chave) {
     if (inicio > fim) {
         return -1;
     }
     int meio = inicio + (fim - inicio) / 2;
 
     if (vetorD[meio].id == chave) {
         return meio;
     }
     if (vetorD[meio].id < chave) {
         return buscaBinariaRecursiva(vetorD, meio + 1, fim, chave);
     } else {
         return buscaBinariaRecursiva(vetorD, inicio, meio - 1, chave);
     }
 }
 //Funçao para buscar um dado
 void buscarDado(sequenceSet &sequenceSet,dado &umdado){
     dado primeiro;
     pedaco blocoAtual;
     bool achou = false;
     int cont = 0;
     int i = 1;
     while (cont < sequenceSet.numeroDePedacos && achou == false) {
         ifstream arq(to_string (i) + ".bin", ios::binary);
 
         arq.seekg(0);
         arq.read(reinterpret_cast<char*>(&blocoAtual.numeroDeRegistros), sizeof(blocoAtual.numeroDeRegistros));
         arq.read(reinterpret_cast<char*>(&blocoAtual.pedacoAnterior ), sizeof(blocoAtual.pedacoAnterior));
         arq.read(reinterpret_cast<char*>(&blocoAtual.proximoPedaco), sizeof(blocoAtual.proximoPedaco));
         arq.read(reinterpret_cast<char*>(&primeiro), sizeof(dado));
 
         if(umdado.id < primeiro.id) {
             achou = true;
         }
 
         arq.close();
         i = blocoAtual.proximoPedaco;
         cont++;
     }
     int pedacoEncontrado;
     if (achou == false) {
         pedacoEncontrado = sequenceSet.ultimoPedaco;
     } else {
         pedacoEncontrado =blocoAtual.pedacoAnterior;
     }
 
     fstream arqBlocoAtual(to_string(pedacoEncontrado) + ".bin", ios::binary | ios::in | ios::out);
 
     arqBlocoAtual.read(reinterpret_cast<char*>(&blocoAtual.numeroDeRegistros), sizeof(blocoAtual.numeroDeRegistros));
     arqBlocoAtual.read(reinterpret_cast<char*>(&blocoAtual.pedacoAnterior ), sizeof(blocoAtual.pedacoAnterior));
     arqBlocoAtual.read(reinterpret_cast<char*>(&blocoAtual.proximoPedaco), sizeof(blocoAtual.proximoPedaco));
 
     dado vetorD[blocoAtual.numeroDeRegistros];
     for (int i = 0; i < blocoAtual.numeroDeRegistros; i++) {
         arqBlocoAtual.read(reinterpret_cast<char*>(&vetorD[i]), sizeof(dado));
     }
 
     int posicao = buscaBinariaRecursiva(vetorD,0,blocoAtual.numeroDeRegistros,umdado.id);
     if(posicao == -1){
         cout << "O dado nao foi encontrado!" << endl;
     }
     else{
     cout << "\nID: " << vetorD[posicao].id << endl;
     cout << "Nome: " << vetorD[posicao].name << endl;
     cout << "Cargo: " << vetorD[posicao].job << endl;
     cout << "Base Pay: " << vetorD[posicao].basePay << endl;
     cout << "Overtime Pay: " << vetorD[posicao].overtimePay << endl;
     cout << "Other Pay: " << vetorD[posicao].otherPay << endl;
     cout << "Benefits: " << vetorD[posicao].benefits << endl;
     cout << "Total Pay: " << vetorD[posicao].totalPay << endl;
     cout << "Total Pay & Benefits: " << vetorD[posicao].totalPayBenefits << endl;
     cout << "Year: " << vetorD[posicao].year << endl;
     }
 }
 
 //Funçao para remover um dado(remove de forma logica)
 void removerDado(sequenceSet &sequenceSet,dado &umdado){
     dado primeiro;
     pedaco blocoAtual;
     bool achou = false;
     int cont = 0;
     int i = 1;
     while (cont < sequenceSet.numeroDePedacos && achou == false) {
         ifstream arq(to_string (i) + ".bin", ios::binary);
 
         arq.seekg(0);
         arq.read(reinterpret_cast<char*>(&blocoAtual.numeroDeRegistros), sizeof(blocoAtual.numeroDeRegistros));
         arq.read(reinterpret_cast<char*>(&blocoAtual.pedacoAnterior ), sizeof(blocoAtual.pedacoAnterior));
         arq.read(reinterpret_cast<char*>(&blocoAtual.proximoPedaco), sizeof(blocoAtual.proximoPedaco));
         arq.read(reinterpret_cast<char*>(&primeiro), sizeof(dado));
 
         if(umdado.id < primeiro.id) {
             achou = true;
         }
 
         arq.close();
         i = blocoAtual.proximoPedaco;
         cont++;
     }
     int pedacoEncontrado;
     if (achou == false) {
         pedacoEncontrado = sequenceSet.ultimoPedaco;
     } else {
         pedacoEncontrado =blocoAtual.pedacoAnterior;
     }
     fstream arqBlocoAtual(to_string(pedacoEncontrado) + ".bin", ios::binary | ios::in | ios::out);
     arqBlocoAtual.seekg(0);
     arqBlocoAtual.read(reinterpret_cast<char*>(&blocoAtual.numeroDeRegistros), sizeof(blocoAtual.numeroDeRegistros));
     arqBlocoAtual.read(reinterpret_cast<char*>(&blocoAtual.pedacoAnterior ), sizeof(blocoAtual.pedacoAnterior));
     arqBlocoAtual.read(reinterpret_cast<char*>(&blocoAtual.proximoPedaco), sizeof(blocoAtual.proximoPedaco));
 
     dado vetorD[blocoAtual.numeroDeRegistros];
     for (int i = 0; i < blocoAtual.numeroDeRegistros; i++) {
         arqBlocoAtual.read(reinterpret_cast<char*>(&vetorD[i]), sizeof(dado));
     }
 
     int posicao = buscaBinariaRecursiva(vetorD,0,blocoAtual.numeroDeRegistros,umdado.id);
     if (posicao == -1){
         cout << "Nao encontrado" << endl;
     } else {
         if(blocoAtual.numeroDeRegistros > 25){
             blocoAtual.numeroDeRegistros--;
             ofstream arqLimparAtual(to_string (pedacoEncontrado) + ".bin", ios::binary | ios::out |ios::trunc);
             arqLimparAtual.seekp(0);
             arqLimparAtual.write(reinterpret_cast<const char *>(&blocoAtual.numeroDeRegistros), sizeof(int));
             arqLimparAtual.write(reinterpret_cast<const char *>(&blocoAtual.pedacoAnterior), sizeof(int));
             arqLimparAtual.write(reinterpret_cast<const char *>(&blocoAtual.proximoPedaco), sizeof(int));
             for(int i= 0; i < posicao; i++){
                 arqLimparAtual.write(reinterpret_cast<const char *>(&vetorD[i]), sizeof(dado));
             }
             for(int i= posicao+1; i < blocoAtual.numeroDeRegistros + 1; i++){
                 arqLimparAtual.write(reinterpret_cast<const char *>(&vetorD[i]), sizeof(dado));
             }
             arqLimparAtual.close();
         } else {
             pedaco blocoAnt;
             fstream arqBlocoAnt(to_string (blocoAtual.pedacoAnterior) + ".bin", ios::binary | ios::in | ios::out);
             arqBlocoAnt.seekg(0);
             arqBlocoAnt.read(reinterpret_cast<char*>(&blocoAnt.numeroDeRegistros), sizeof(blocoAnt.numeroDeRegistros));
             arqBlocoAnt.read(reinterpret_cast<char*>(&blocoAnt.pedacoAnterior ), sizeof(blocoAnt.pedacoAnterior));
             arqBlocoAnt.read(reinterpret_cast<char*>(&blocoAnt.proximoPedaco), sizeof(blocoAnt.proximoPedaco));
 
             dado vetorPrincipal[blocoAnt.numeroDeRegistros];
             for(int i = 0; i < blocoAnt.numeroDeRegistros; i++){
                 arqBlocoAnt.read(reinterpret_cast<char*>(&vetorPrincipal[i]), sizeof(dado));
             }
 
             if(blocoAnt.numeroDeRegistros == 25){
                 blocoAnt.proximoPedaco = blocoAtual.proximoPedaco;
                 int tamanhoParte1 = blocoAnt.numeroDeRegistros;
                 blocoAnt.numeroDeRegistros = blocoAtual.numeroDeRegistros+blocoAnt.numeroDeRegistros -1;
 
                 //mudanças sequence set
                 sequenceSet.numeroDePedacos--;
                 if(achou == false){
                     sequenceSet.ultimoPedaco = blocoAtual.pedacoAnterior;
                 }
                 ofstream arqLimparAtual(to_string (blocoAtual.pedacoAnterior) + ".bin", ios::binary | ios::out |ios::trunc);
                 arqLimparAtual.seekp(0);
 
                 arqLimparAtual.write(reinterpret_cast<const char *>(&blocoAnt.numeroDeRegistros), sizeof(int));
                 arqLimparAtual.write(reinterpret_cast<const char *>(&blocoAnt.pedacoAnterior), sizeof(int));
                 arqLimparAtual.write(reinterpret_cast<const char *>(&blocoAnt.proximoPedaco), sizeof(int));
                 for(int i = 0; i < tamanhoParte1; i++){
                     arqLimparAtual.write(reinterpret_cast<char*>(&vetorPrincipal[i]), sizeof(dado));
                 }
                 for(int i=0; i<posicao; i++){
                     arqLimparAtual.write(reinterpret_cast<char*>(&vetorD[i]), sizeof(dado));
                 }
                 for(int i=posicao+1; i<blocoAtual.numeroDeRegistros; i++){
                     arqLimparAtual.write(reinterpret_cast<char*>(&vetorD[i]), sizeof(dado));
                 }
 
                 blocoAtual.numeroDeRegistros =0;
                 ofstream arqLimpo(to_string (pedacoEncontrado) + ".bin", ios::binary | ios::out |ios::trunc);
                 arqLimpo.close();
                 arqLimparAtual.close();
 
             } else {
                 vetorD[posicao] = vetorPrincipal[blocoAnt.numeroDeRegistros-1];
                 blocoAnt.numeroDeRegistros--;
 
                 for (int i = 1; i < blocoAtual.numeroDeRegistros; i++) {
                     int chave = vetorD[i].id;
                     int j = i - 1;
                     while (j >= 0 && vetorD[j].id > chave) {
                         vetorD[j + 1] = vetorD[j];
                         j = j - 1;
                     }
                     vetorD[j + 1].id = chave;
                 }
 
                 ofstream arqLimparAtual(to_string (pedacoEncontrado) + ".bin", ios::binary | ios::out |ios::trunc);
                 arqLimparAtual.seekp(0);
 
                 arqLimparAtual.write(reinterpret_cast<const char *>(&blocoAtual.numeroDeRegistros), sizeof(int));
                 arqLimparAtual.write(reinterpret_cast<const char *>(&blocoAtual.pedacoAnterior), sizeof(int));
                 arqLimparAtual.write(reinterpret_cast<const char *>(&blocoAtual.proximoPedaco), sizeof(int));
 
                 for(int i = 0; i < blocoAtual.numeroDeRegistros; i++) {
                     arqLimparAtual.write(reinterpret_cast<char*>(&vetorD[i]), sizeof(dado));
                 }
                 arqBlocoAnt.seekp(0);
                 arqBlocoAnt.write(reinterpret_cast<const char *>(&blocoAnt.numeroDeRegistros), sizeof(int));
                 arqBlocoAnt.write(reinterpret_cast<const char *>(&blocoAnt.pedacoAnterior), sizeof(int));
                 arqBlocoAnt.write(reinterpret_cast<const char *>(&blocoAnt.proximoPedaco), sizeof(int));
 
                 for(int i = 0; i < blocoAnt.numeroDeRegistros; i++) {
                     arqBlocoAnt.write(reinterpret_cast<char*>(&vetorPrincipal[i]), sizeof(dado));
                 }
 
                 arqBlocoAnt.close();
                 arqLimparAtual.close();
             }
         }
     }
     cout << "ID removido com sucesso!" << endl;
 }
 
 // Função para carregar dados de um arquivo CSV e salvar no arquivo binário
 void importarCSV() {
     ifstream entrada("dados.csv");
     if (!entrada.is_open()) {
         cout << "Erro ao abrir o arquivo CSV.\n";
         return;
     }
 
     string linha;
     dado novoRegistro;
     int contador =0;
 
     getline(entrada, linha); // Ignorar a primeira linha (cabeçalho)
 
     while (getline(entrada, linha)) {
         stringstream ss(linha);
         string campo;
 
         // Lendo o campo "Id"
         getline(ss, campo, ',');
         novoRegistro.id = stoi(campo);
 
         // Lendo o campo "Employee Name"
         getline(ss, campo, ',');
         strncpy(novoRegistro.name, campo.c_str(), sizeof(novoRegistro.name));
         novoRegistro.name[sizeof(novoRegistro.name) - 1] = '\0';
 
         // Lendo o campo "Job Title"
         getline(ss, campo, ',');
         if (campo.front() == '"' && campo.back() == '"') {
             campo = campo.substr(1, campo.size() - 2);
         }
         strncpy(novoRegistro.job, campo.c_str(), sizeof(novoRegistro.job));
         novoRegistro.job[sizeof(novoRegistro.job) - 1] = '\0';
 
         // Lendo o campo "Base Pay"
         getline(ss, campo, ',');
         try {
             novoRegistro.basePay = static_cast<float>(stod(campo));
         } catch (const std::invalid_argument&) {
             novoRegistro.basePay = 0;
         }
 
         // Lendo o campo "Overtime Pay"
         getline(ss, campo, ',');
         try {
             novoRegistro.overtimePay = static_cast<float>(stod(campo));
         } catch (const std::invalid_argument&) {
             novoRegistro.overtimePay = 0;
         }
 
         // Lendo o campo "Other Pay"
         getline(ss, campo, ',');
         try {
             novoRegistro.otherPay = static_cast<float>(stod(campo));
         } catch (const std::invalid_argument&) {
             novoRegistro.otherPay = 0;
         }
 
         // Lendo o campo "Benefits"
         getline(ss, campo, ',');
         try {
             novoRegistro.benefits = static_cast<float>(stod(campo));
         } catch (const std::invalid_argument&) {
             novoRegistro.benefits = 0;
         }
 
         // Lendo o campo "Total Pay"
         getline(ss, campo, ',');
         try {
             novoRegistro.totalPay = static_cast<float>(stod(campo));
         } catch (const std::invalid_argument&) {
             novoRegistro.totalPay = 0;
         }
 
         // Lendo o campo "Total Pay & Benefits"
         getline(ss, campo, ',');
         try {
             novoRegistro.totalPayBenefits = static_cast<float>(stod(campo));
         } catch (const std::invalid_argument&) {
             novoRegistro.totalPayBenefits = 0;
         }
 
         // Lendo o campo "Year"
         getline(ss, campo, ',');
         try {
             novoRegistro.year = static_cast<float>(stod(campo));
         } catch (const std::invalid_argument&) {
             novoRegistro.year = 0;
         }
 
        transformaBinario(novoRegistro,contador);
     }
 
     entrada.close();
     cout << "Dados carregados com sucesso do arquivo CSV.\n";
 }
 //Importaçao de um novo csv
 void importarCsvNovo(string nomeArquivo,sequenceSet &sequenceSet) {
     ifstream entrada(nomeArquivo);
     if (!entrada.is_open()) {
         cout << "Erro ao abrir o arquivo CSV.\n";
         return;
     }
 
     string linha;
     dado novoRegistro;
 
     getline(entrada, linha); // Ignorar a primeira linha (cabeçalho)
 
     while (getline(entrada, linha)) {
         stringstream ss(linha);
         string campo;
 
         // Lendo o campo "Id"
         getline(ss, campo, ',');
         novoRegistro.id = stoi(campo);
 
         // Lendo o campo "Employee Name"
         getline(ss, campo, ',');
         strncpy(novoRegistro.name, campo.c_str(), sizeof(novoRegistro.name));
         novoRegistro.name[sizeof(novoRegistro.name) - 1] = '\0';
 
         // Lendo o campo "Job Title"
         getline(ss, campo, ',');
         if (campo.front() == '"' && campo.back() == '"') {
             campo = campo.substr(1, campo.size() - 2);
         }
         strncpy(novoRegistro.job, campo.c_str(), sizeof(novoRegistro.job));
         novoRegistro.job[sizeof(novoRegistro.job) - 1] = '\0';
 
         // Lendo o campo "Base Pay"
         getline(ss, campo, ',');
         try {
             novoRegistro.basePay = static_cast<float>(stod(campo));
         } catch (const std::invalid_argument&) {
             novoRegistro.basePay = 0;
         }
 
         // Lendo o campo "Overtime Pay"
         getline(ss, campo, ',');
         try {
             novoRegistro.overtimePay = static_cast<float>(stod(campo));
         } catch (const std::invalid_argument&) {
             novoRegistro.overtimePay = 0;
         }
 
         // Lendo o campo "Other Pay"
         getline(ss, campo, ',');
         try {
             novoRegistro.otherPay = static_cast<float>(stod(campo));
         } catch (const std::invalid_argument&) {
             novoRegistro.otherPay = 0;
         }
 
         // Lendo o campo "Benefits"
         getline(ss, campo, ',');
         try {
             novoRegistro.benefits = static_cast<float>(stod(campo));
         } catch (const std::invalid_argument&) {
             novoRegistro.benefits = 0;
         }
 
         // Lendo o campo "Total Pay"
         getline(ss, campo, ',');
         try {
             novoRegistro.totalPay = static_cast<float>(stod(campo));
         } catch (const std::invalid_argument&) {
             novoRegistro.totalPay = 0;
         }
 
         // Lendo o campo "Total Pay & Benefits"
         getline(ss, campo, ',');
         try {
             novoRegistro.totalPayBenefits = static_cast<float>(stod(campo));
         } catch (const std::invalid_argument&) {
             novoRegistro.totalPayBenefits = 0;
         }
 
         // Lendo o campo "Year"
         getline(ss, campo, ',');
         try {
             novoRegistro.year = static_cast<float>(stod(campo));
         } catch (const std::invalid_argument&) {
             novoRegistro.year = 0;
         }
 
         insereNoPedaco(novoRegistro,sequenceSet);
     }
 
     entrada.close();
     cout << "Dados carregados com sucesso do arquivo CSV.\n";
 }
 
 //Funçao para visualizaçao de todos os blocos
 void visualizar_dados(sequenceSet &sequenceSet) {
 int cont = sequenceSet.primeiroPedaco;
     for(int i = 0; i < sequenceSet.numeroDePedacos; i++) {
         cout << "---------------BLOCO: " << cont << "---------------" << endl;
         ifstream arq(to_string (cont) + ".bin", ios::binary);
         pedaco blocoAtual;
 
         arq.seekg(0);
         arq.read(reinterpret_cast<char*>(&blocoAtual.numeroDeRegistros), sizeof(blocoAtual.numeroDeRegistros));
         arq.read(reinterpret_cast<char*>(&blocoAtual.pedacoAnterior ), sizeof(blocoAtual.pedacoAnterior));
         arq.read(reinterpret_cast<char*>(&blocoAtual.proximoPedaco), sizeof(blocoAtual.proximoPedaco));
 
         cout << "Numero de registros: " << blocoAtual.numeroDeRegistros << endl;
         cout << "Pedaco anterior: " << blocoAtual.pedacoAnterior << endl;
         cout << "Proximo pedaco: " << blocoAtual.proximoPedaco << endl;
 
         dado umdado;
 
 
         for(int i = 0; i < blocoAtual.numeroDeRegistros; i++) {
 
 
             arq.read(reinterpret_cast<char*>(&umdado), sizeof(dado));
             cout << "\nID: " << umdado.id << endl;
             cout << "Nome: " << umdado.name << endl;
             cout << "Cargo: " << umdado.job << endl;
             cout << "Base Pay: " << umdado.basePay << endl;
             cout << "Overtime Pay: " << umdado.overtimePay << endl;
             cout << "Other Pay: " << umdado.otherPay << endl;
             cout << "Benefits: " << umdado.benefits << endl;
             cout << "Total Pay: " << umdado.totalPay << endl;
             cout << "Total Pay & Benefits: " <<umdado.totalPayBenefits << endl;
             cout << "Year: " << umdado.year << endl;
         }
         arq.close();
         cont = blocoAtual.proximoPedaco;
     }
 }
 
 //Funçao menu
 void menu() {
     cout << "+----------------------------+" << endl;
     cout << "|        MENU PRINCIPAL      |" << endl;
     cout << "+----------------------------+" << endl;
     cout << "|  1. Inserir                |" << endl;
     cout << "|  2. Remover                |" << endl;
     cout << "|  3. Buscar                 |" << endl;
     cout << "|  4. Imprimir               |" << endl;
     cout << "|  5. Encerrar               |" << endl;
     cout << "+----------------------------+" << endl;
     cout << "\nEscolha uma opcao: ";
 }
 
 int main() {
     int count = 0;
     int opcao, opcao2;
     dado umdado;
     sequenceSet sequenceSet;
 
     for (int i = 0; i < 6; i++) { // como eu sei que importarei apenas 300 dados, deixarei meus arquivos.bin criados
         criarPedaco(count);
     }
 
     importarCSV();
     menu();
     cin >> opcao;
     while (opcao != 5) {
         switch (opcao) {
         case 1:
             cout << "+--------------------------------+" << endl;
             cout << "|             Inserir            |" << endl;
             cout << "+--------------------------------+" << endl;
             cout << "| 1. Inserir por entrada padrao  |" << endl;
             cout << "| 2. Inserir por arquivo(.csv)   |" << endl;
             cout << "| 3. Voltar ao menu              |" << endl;
             cout << "+--------------------------------+" << endl;
             cout << "\nEscolha uma opcao: ";
             cin >> opcao2;
             cin.ignore(); // Limpa o buffer
             if (opcao2 == 1) {
                 cout << "\nID: ";
                 cin >> umdado.id;
                 cin.ignore();
                 cout << "Nome: ";
                 cin.getline(umdado.name, sizeof(umdado.name));
                 cout << "Cargo: ";
                 cin.getline(umdado.job, sizeof(umdado.job));
                 cout << "Base Pay: ";
                 cin >> umdado.basePay;
                 cout << "Overtime Pay: ";
                 cin >> umdado.overtimePay;
                 cout << "Other Pay: ";
                 cin >> umdado.otherPay;
                 cout << "Benefits: ";
                 cin >> umdado.benefits;
                 cout << "Total Pay: ";
                 cin >> umdado.totalPay;
                 cout << "Total Pay & Benefits: ";
                 cin >> umdado.totalPayBenefits;
                 cout << "Year: ";
                 cin >> umdado.year;
                 insereNoPedaco(umdado, sequenceSet);
             } else if (opcao2 == 2){
                 string nomeArquivo;
                 cout << "Insira o nome do arquivo(.csv): ";
                 cin >> nomeArquivo;
                 importarCsvNovo(nomeArquivo,sequenceSet);
             } else {
                 break;
             }
             break;
         case 2:
             cout <<"\nInsira o ID que voce deseja remover: ";
             cin >> umdado.id;
             removerDado(sequenceSet,umdado);
             break;
         case 3:
             cout << "\nQual ID voce deseja buscar?" << endl;
             cout << "ID: ";
             cin >> umdado.id;
             buscarDado(sequenceSet,umdado);
             break;
         case 4:
             visualizar_dados(sequenceSet);
             cout << "\n\n\n";
             break;
         case 5: // finalizar
             return 0;
             break;
         default:
             cout << "Opcao invalida!" << endl;
         }
         menu();
         cin >> opcao;
     }
     return 0;
 }