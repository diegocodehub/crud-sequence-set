/*

Projeto Prático de Estrutura de Dados

GRUPO:
Gustavo Martins de Oliveira (202411213)
Diego Alves de Oliveira (202410370)

*/
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <string>

using namespace std;

void menu() {
    cout << "*" <<"--------------------------------------"<< "*" << endl;
    cout << "|" << "                                     " << " |" << endl;
    cout << "|" << "                 " << "Menu" << "                " << " |" << endl;
    cout << "|" << "                                     " << " |" << endl;
    cout << "| 1. Adicionar dado" << "                   " << " |" << endl;
    cout << "| 2. Remover dado" << "                     " << " |" << endl;
    cout << "| 3. Buscar dado" << "                      " << " |" << endl;
    cout << "| 4. Mostrar dados" << "                    " << " |" << endl;
    cout << "| 5. Sair" << "                             " << " |" << endl;
    cout << "*" << "--------------------------------------"<< "*" << endl;
    cout << endl << "Escolha uma opcao: ";
}

struct Dado { // struct com os campos dos registros
    int id;
    int timeRef;
    char account[50];
    int code;
    char countryCode[50];
    char productType[50];
    double value;
    char status;
}; 

struct SequenceSet { // struct com as informações do SequenceSet
    int quantidadeDeBlocos = 10;
    int primeiroBloco = 1;
    int ultimoBloco = 10;
    int blocoProximo = -1;
};

struct Bloco { // struct com as informações do Bloco
    int quantidadeDeRegistros = 0;
    int blocoAnterior = -1;
    int blocoProximo = -1;
};

const int tamanho = 50; // constante com a capacidade fixa dos blocos.

void criarBloco(int &contador) {
    contador++;
    Bloco bloco;
    bloco.quantidadeDeRegistros = 0;
    bloco.blocoAnterior = contador - 1;
    bloco.blocoProximo = contador + 1;

    string numArquivo = to_string(contador);
    ofstream arquivo(numArquivo + ".bin", ios::binary); // cria um Bloco, sendo ele um arquivo binário

    if (arquivo.is_open()) {
        arquivo.write(reinterpret_cast<const char *>(&bloco.quantidadeDeRegistros), sizeof(int));
        arquivo.write(reinterpret_cast<const char *>(&bloco.blocoAnterior), sizeof(int));
        arquivo.write(reinterpret_cast<const char *>(&bloco.blocoProximo), sizeof(int));
    } 
    else {
        cout << "Erro no arquivo" << endl;
    }
    arquivo.close();
}

void adicionaNoBloco(Dado novoDado, SequenceSet &sequenceSet) {
    Dado primeiro;
    Bloco blocoAtual;
    bool encontrado = false;

    int i = 1;
    int cont = 0;
    while (cont < sequenceSet.quantidadeDeBlocos && encontrado == false) {
        ifstream arq(to_string(i) + ".bin", ios::binary);

        arq.seekg(0);   
        arq.read(reinterpret_cast<char*>(&blocoAtual.quantidadeDeRegistros), sizeof(blocoAtual.quantidadeDeRegistros));
        arq.read(reinterpret_cast<char*>(&blocoAtual.blocoAnterior), sizeof(blocoAtual.blocoAnterior));
        arq.read(reinterpret_cast<char*>(&blocoAtual.blocoProximo), sizeof(blocoAtual.blocoProximo));
        arq.read(reinterpret_cast<char*>(&primeiro), sizeof(Dado));

        if (novoDado.id < primeiro.id) { // comparação para saber em qual Bloco adicionar o novo registro de maneira ordenada
            encontrado = true; // comparação feita pegando o primeiro dado de cada Bloco e comparando o registro a ser inserido
        }

        arq.close();
        i = blocoAtual.blocoProximo;
        cont++;
    }

    int blocoEncontrado;
    if (encontrado == false) {
        blocoEncontrado = sequenceSet.ultimoBloco;
    }
    else {
        blocoEncontrado = blocoAtual.blocoAnterior;
    }

    fstream arquivoBloco(to_string(blocoEncontrado) + ".bin", ios::binary | ios::in | ios::out);

    arquivoBloco.read(reinterpret_cast<char*>(&blocoAtual.quantidadeDeRegistros), sizeof(blocoAtual.quantidadeDeRegistros));
    arquivoBloco.read(reinterpret_cast<char*>(&blocoAtual.blocoAnterior), sizeof(blocoAtual.blocoAnterior));
    arquivoBloco.read(reinterpret_cast<char*>(&blocoAtual.blocoProximo), sizeof(blocoAtual.blocoProximo));

    Dado vetorDados[blocoAtual.quantidadeDeRegistros + 2]; // cria um vetor para salvar os registros e adicionar o novo.
    for (int i = 0; i <= blocoAtual.quantidadeDeRegistros + 1; i++) {
        arquivoBloco.read(reinterpret_cast<char*>(&vetorDados[i]), sizeof(Dado));
    }

    ofstream blocoAtualizado(to_string(blocoEncontrado) + ".bin", ios::binary | ios::out | ios::trunc); // limpa o Bloco já que seus dados
                                                                                                           // estão salvos no vetor
    int j = blocoAtual.quantidadeDeRegistros-1;
    while (j >= 0 && vetorDados[j].id > novoDado.id) { // procurando a posição certa para inserir o novo registro ordenado
        vetorDados[j + 1] = vetorDados[j];
        j--;
    }
    vetorDados[j + 1] = novoDado; // Adiciona o novo registro no vetor

    if (blocoAtual.quantidadeDeRegistros == tamanho) { // se o Bloco em que o novo registro deve ser adicionado estiver cheio,
        int metade = (blocoAtual.quantidadeDeRegistros + 1) / 2; // outro bloco é criado e o Bloco é dividido. 

        Bloco novoBloco;

        novoBloco.quantidadeDeRegistros = metade;
        novoBloco.blocoAnterior = blocoEncontrado;
        novoBloco.blocoProximo = blocoAtual.blocoProximo;

        int diff = sequenceSet.ultimoBloco + 1;
        ofstream arquivoNovoBloco(to_string(diff) + ".bin", ios::binary | ios::out);

        if (arquivoNovoBloco.is_open()) {
            arquivoNovoBloco.seekp(0);
            arquivoNovoBloco.write(reinterpret_cast<const char *>(&novoBloco.quantidadeDeRegistros), sizeof(novoBloco.quantidadeDeRegistros));
            arquivoNovoBloco.write(reinterpret_cast<const char *>(&novoBloco.blocoAnterior), sizeof(novoBloco.blocoAnterior));
            arquivoNovoBloco.write(reinterpret_cast<const char *>(&novoBloco.blocoProximo), sizeof(novoBloco.blocoProximo));

            for (int i = metade + 1; i <= tamanho + 1; i++) {
                arquivoNovoBloco.write(reinterpret_cast<char*>(&vetorDados[i]), sizeof(Dado));
            }
        }

        blocoAtual.quantidadeDeRegistros = metade + 1;
        blocoAtual.blocoProximo = sequenceSet.ultimoBloco + 1;

        if (blocoAtualizado.is_open()) {
            blocoAtualizado.seekp(0);
            blocoAtualizado.write(reinterpret_cast<char*>(&blocoAtual.quantidadeDeRegistros), sizeof(blocoAtual.quantidadeDeRegistros));
            blocoAtualizado.write(reinterpret_cast<char*>(&blocoAtual.blocoAnterior), sizeof(blocoAtual.blocoAnterior));
            blocoAtualizado.write(reinterpret_cast<char*>(&blocoAtual.blocoProximo), sizeof(blocoAtual.blocoProximo));

            for (int i = 0; i <= metade; i++) {
                blocoAtualizado.write(reinterpret_cast<char*>(&vetorDados[i]), sizeof(Dado));
            }
        }

        Bloco atualProx;
        int atualProxIndice = sequenceSet.ultimoBloco + 1;
        fstream arquivoAtualProx(to_string(blocoAtual.blocoProximo) + ".bin", ios::binary | ios::in | ios::out);

        arquivoAtualProx.read(reinterpret_cast<char*>(&atualProx.quantidadeDeRegistros), sizeof(atualProx.quantidadeDeRegistros)); // olhar seekp
        arquivoAtualProx.read(reinterpret_cast<char*>(&atualProx.blocoAnterior), sizeof(atualProx.blocoAnterior));

        arquivoAtualProx.seekp(0);
        arquivoAtualProx.write(reinterpret_cast<char*>(&atualProx.quantidadeDeRegistros), sizeof(atualProx.quantidadeDeRegistros)); // olhar seekp
        arquivoAtualProx.write(reinterpret_cast<char*>(&atualProxIndice), sizeof(atualProxIndice));

        int blocos = sequenceSet.quantidadeDeBlocos;
        blocos++;
        sequenceSet.quantidadeDeBlocos = blocos;
        if (encontrado == false) {
            int ultimo = sequenceSet.ultimoBloco;
            ultimo++;
            sequenceSet.ultimoBloco = ultimo;
        }

    }
    else { // se o bloco tiver espaço, é apenas escrever os registros salvos no vetor, no Bloco novamente
        blocoAtual.quantidadeDeRegistros++;
        blocoAtual.blocoProximo = sequenceSet.ultimoBloco + 1;

        if (blocoAtualizado.is_open()) {
            blocoAtualizado.seekp(0);
            blocoAtualizado.write(reinterpret_cast<char*>(&blocoAtual.quantidadeDeRegistros), sizeof(int));
            blocoAtualizado.write(reinterpret_cast<char*>(&blocoAtual.blocoAnterior), sizeof(int));
            blocoAtualizado.write(reinterpret_cast<char*>(&blocoAtual.blocoProximo), sizeof(int));

            for (int i = 0; i <= blocoAtual.quantidadeDeRegistros; i++) {
                blocoAtualizado.write(reinterpret_cast<char*>(&vetorDados[i]), sizeof(Dado));
            }
        }
    }
}

void CriarBinario(Dado umDado, int &contador) {
    int arquivoIndice;
    if (contador == 0) {
        arquivoIndice = 1;
    }
    else {
        arquivoIndice = contador / tamanho + 1;
    }

    ofstream arquivo(to_string(arquivoIndice) + ".bin", ios::binary | ios::app);
    if (arquivo.is_open()) {
        arquivo.write(reinterpret_cast<const char *>(&umDado), sizeof(umDado));
        contador++;

        Bloco blocoAtual;
        ifstream arquivo(to_string(arquivoIndice) + ".bin", ios::binary);
        arquivo.read(reinterpret_cast<char*>(&blocoAtual.quantidadeDeRegistros), sizeof(blocoAtual.quantidadeDeRegistros));
        blocoAtual.quantidadeDeRegistros++;
        arquivo.close();

        ofstream arquivoAtualiza(to_string(arquivoIndice) + ".bin", ios::binary | ios::in | ios::out);
        arquivoAtualiza.seekp(0);
        arquivoAtualiza.write(reinterpret_cast<const char *>(&blocoAtual.quantidadeDeRegistros), sizeof(blocoAtual.quantidadeDeRegistros));
        arquivoAtualiza.close();
    }
    else {
        cout << "Erro no arquivo" << endl;
    }
}

int buscaBinaria(Dado vetorDados[], int inicio, int fim, int chave) {
    if (inicio > fim) {
        return -1;
    }
    int meio = inicio + (fim - inicio) / 2;

    if (vetorDados[meio].id == chave) {
        return meio;
    }

    if (vetorDados[meio].id < chave) {
        return buscaBinaria(vetorDados, meio + 1, fim, chave);
    }
    else {
        return buscaBinaria(vetorDados, inicio, meio - 1, chave);
    }
}

void excluirDado(SequenceSet &sequenceSet, Dado &umDado){
    Dado primeiro;
    Bloco blocoAtual;
    bool encontrado = false;
    int cont = 0;
    int i = 1;
    while (cont < sequenceSet.quantidadeDeBlocos && encontrado == false) {
        ifstream arquivo(to_string (i) + ".bin", ios::binary); // abre os Blocos para leitura para procurar o registro que deve ser excluído

        arquivo.seekg(0);
        arquivo.read(reinterpret_cast<char*>(&blocoAtual.quantidadeDeRegistros), sizeof(blocoAtual.quantidadeDeRegistros));
        arquivo.read(reinterpret_cast<char*>(&blocoAtual.blocoAnterior ), sizeof(blocoAtual.blocoAnterior));
        arquivo.read(reinterpret_cast<char*>(&blocoAtual.blocoProximo), sizeof(blocoAtual.blocoProximo));
        arquivo.read(reinterpret_cast<char*>(&primeiro), sizeof(Dado));

        if(umDado.id < primeiro.id) { // comparação para encontrar o registro que deve ser excluído
            encontrado = true;
        }

        arquivo.close();
        i = blocoAtual.blocoProximo;
        cont++;
    }
    int blocoEncontrado;
    if (encontrado == false) {
        blocoEncontrado = sequenceSet.ultimoBloco;
    } else {
        blocoEncontrado =blocoAtual.blocoAnterior;
    }
    fstream arquivoBloco(to_string(blocoEncontrado) + ".bin", ios::binary | ios::in | ios::out);
    arquivoBloco.seekg(0);
    arquivoBloco.read(reinterpret_cast<char*>(&blocoAtual.quantidadeDeRegistros), sizeof(blocoAtual.quantidadeDeRegistros));
    arquivoBloco.read(reinterpret_cast<char*>(&blocoAtual.blocoAnterior ), sizeof(blocoAtual.blocoAnterior));
    arquivoBloco.read(reinterpret_cast<char*>(&blocoAtual.blocoProximo), sizeof(blocoAtual.blocoProximo));
    
    Dado vetorRegistros[blocoAtual.quantidadeDeRegistros];
    for (int i = 0; i < blocoAtual.quantidadeDeRegistros; i++) {
        arquivoBloco.read(reinterpret_cast<char*>(&vetorRegistros[i]), sizeof(Dado));
    }
    
    int posicao = buscaBinaria(vetorRegistros,0,blocoAtual.quantidadeDeRegistros,umDado.id);
    if (posicao == -1){
        cout << "registro nao encontrado" << endl;
    } else {
        // registro encontrado, e agora será excluido
        if(blocoAtual.quantidadeDeRegistros > 25){
            blocoAtual.quantidadeDeRegistros--;
            ofstream blocoAtualizado(to_string (blocoEncontrado) + ".bin", ios::binary | ios::out |ios::trunc);
            blocoAtualizado.seekp(0);
            blocoAtualizado.write(reinterpret_cast<const char *>(&blocoAtual.quantidadeDeRegistros), sizeof(int));
            blocoAtualizado.write(reinterpret_cast<const char *>(&blocoAtual.blocoAnterior), sizeof(int));
            blocoAtualizado.write(reinterpret_cast<const char *>(&blocoAtual.blocoProximo), sizeof(int));
            for(int i= 0; i < posicao; i++){
                blocoAtualizado.write(reinterpret_cast<const char *>(&vetorRegistros[i]), sizeof(Dado));
            }
            for(int i= posicao+1; i< blocoAtual.quantidadeDeRegistros; i++){
                blocoAtualizado.write(reinterpret_cast<const char *>(&vetorRegistros[i]), sizeof(Dado));
            }
            blocoAtualizado.close();
            cout << "registro excluido com sucesso!" << endl; 
        }
        else { // caso o bloco tenha 25 ou menos elementos tem de se analisar o Bloco anterior para pegar emprestado um registro
            Bloco blocoAnt; 
            fstream arquivoBlocoAnt(to_string (blocoAtual.blocoAnterior) + ".bin", ios::binary | ios::in | ios::out);
            arquivoBlocoAnt.seekg(0);
            arquivoBlocoAnt.read(reinterpret_cast<char*>(&blocoAnt.quantidadeDeRegistros), sizeof(blocoAnt.quantidadeDeRegistros));
            arquivoBlocoAnt.read(reinterpret_cast<char*>(&blocoAnt.blocoAnterior ), sizeof(blocoAnt.blocoAnterior));
            arquivoBlocoAnt.read(reinterpret_cast<char*>(&blocoAnt.blocoProximo), sizeof(blocoAnt.blocoProximo));

            Dado vetorPrincipal[blocoAnt.quantidadeDeRegistros];
            for(int i = 0; i < blocoAnt.quantidadeDeRegistros; i++){
                arquivoBlocoAnt.read(reinterpret_cast<char*>(&vetorPrincipal[i]), sizeof(Dado));
            }

            if(blocoAnt.quantidadeDeRegistros == 25) { // caso o bloco anterior tenha exatamente 25 elementos
                blocoAnt.blocoProximo = blocoAtual.blocoProximo; // deve ser feito uma junção dos 2 Blocos
                int tamanhoParte1 = blocoAnt.quantidadeDeRegistros;
                blocoAnt.quantidadeDeRegistros = blocoAtual.quantidadeDeRegistros+blocoAnt.quantidadeDeRegistros -1; 
                
                sequenceSet.quantidadeDeBlocos--; // atualizando o sequenceSet
                if(encontrado == false){
                    sequenceSet.ultimoBloco = blocoAtual.blocoAnterior;
                }
                ofstream blocoAtualizado(to_string (blocoAtual.blocoAnterior) + ".bin", ios::binary | ios::out |ios::trunc);
                blocoAtualizado.seekp(0);

                blocoAtualizado.write(reinterpret_cast<const char *>(&blocoAnt.quantidadeDeRegistros), sizeof(int));
                blocoAtualizado.write(reinterpret_cast<const char *>(&blocoAnt.blocoAnterior), sizeof(int));
                blocoAtualizado.write(reinterpret_cast<const char *>(&blocoAnt.blocoProximo), sizeof(int));
                for(int i = 0; i < tamanhoParte1; i++){
                    blocoAtualizado.write(reinterpret_cast<char*>(&vetorPrincipal[i]), sizeof(Dado));
                }
                for(int i=0; i<posicao; i++){
                    blocoAtualizado.write(reinterpret_cast<char*>(&vetorRegistros[i]), sizeof(Dado));
                }
                for(int i=posicao+1; i<blocoAtual.quantidadeDeRegistros; i++){
                    blocoAtualizado.write(reinterpret_cast<char*>(&vetorRegistros[i]), sizeof(Dado));
                }
                
                blocoAtual.quantidadeDeRegistros =0;
                ofstream arquivoLimpo(to_string (blocoEncontrado) + ".bin", ios::binary | ios::out |ios::trunc);
                arquivoLimpo.close();
                blocoAtualizado.close();
               
            } else { // caso o Bloco anterior tenha mais que 25 registros, ai é só o Bloco que terá o elemento excluido pegar emprestado
                vetorRegistros[posicao] = vetorPrincipal[blocoAnt.quantidadeDeRegistros-1];
                blocoAnt.quantidadeDeRegistros--;                                       
                
                for (int i = 1; i < blocoAtual.quantidadeDeRegistros; i++) {
                    int chave = vetorRegistros[i].id;
                    int j = i - 1;
                    while (j >= 0 && vetorRegistros[j].id > chave) {
                        vetorRegistros[j + 1] = vetorRegistros[j];
                        j = j - 1;
                    }
                    vetorRegistros[j + 1].id = chave;
                }
            
                ofstream blocoAtualizado(to_string (blocoEncontrado) + ".bin", ios::binary | ios::out |ios::trunc);
                blocoAtualizado.seekp(0);
                
                blocoAtualizado.write(reinterpret_cast<const char *>(&blocoAtual.quantidadeDeRegistros), sizeof(int));
                blocoAtualizado.write(reinterpret_cast<const char *>(&blocoAtual.blocoAnterior), sizeof(int));
                blocoAtualizado.write(reinterpret_cast<const char *>(&blocoAtual.blocoProximo), sizeof(int));

                for(int i = 0; i < blocoAtual.quantidadeDeRegistros; i++) {
                    blocoAtualizado.write(reinterpret_cast<char*>(&vetorRegistros[i]), sizeof(Dado));
                }
                arquivoBlocoAnt.seekp(0);
                arquivoBlocoAnt.write(reinterpret_cast<const char *>(&blocoAnt.quantidadeDeRegistros), sizeof(int));
                arquivoBlocoAnt.write(reinterpret_cast<const char *>(&blocoAnt.blocoAnterior), sizeof(int));
                arquivoBlocoAnt.write(reinterpret_cast<const char *>(&blocoAnt.blocoProximo), sizeof(int));

                for(int i = 0; i < blocoAnt.quantidadeDeRegistros; i++) {
                    arquivoBlocoAnt.write(reinterpret_cast<char*>(&vetorPrincipal[i]), sizeof(Dado));
                }
                
                arquivoBlocoAnt.close();
                blocoAtualizado.close();
            }
        }
    }
}


void importarDeArquivoCSV() { // função que assim que o programa é iniciado importa para binário um arquivo já pronto .csv
    ifstream entrada("baseatualizada.csv");                // sendo esse arquivo a base de dados sorteada com tamanho 500
    if (!entrada.is_open()) {
        cout << "Erro ao abrir o arquivo CSV.\n";
        return;
    }

    string linha;
    Dado novoRegistro;
    int contador = 0;
    getline(entrada, linha);

    while (getline(entrada, linha)) { // lendo linha por linha e escrevendo no arquivo binário
        stringstream ss(linha);
        string campo;

        getline(ss, campo, ',');
        novoRegistro.id = stoi(campo);

        getline(ss, campo, ',');
        novoRegistro.timeRef = stoi(campo);

        getline(ss, campo, ',');
        strncpy(novoRegistro.account, campo.c_str(), sizeof(novoRegistro.account));
        novoRegistro.account[sizeof(novoRegistro.account) - 1] = '\0';

        getline(ss, campo, ',');
        novoRegistro.code = stoi(campo);

        getline(ss, campo, ',');
        strncpy(novoRegistro.countryCode, campo.c_str(), sizeof(novoRegistro.countryCode));
        novoRegistro.countryCode[sizeof(novoRegistro.countryCode) - 1] = '\0';

        getline(ss, campo, ',');
        strncpy(novoRegistro.productType, campo.c_str(), sizeof(novoRegistro.productType));
        novoRegistro.productType[sizeof(novoRegistro.productType) - 1] = '\0';

        getline(ss, campo, ',');
        try {
            novoRegistro.value = stod(campo);
        } catch (const std::invalid_argument&) {
            novoRegistro.value = 0.0;
        }

        getline(ss, campo, ',');
        novoRegistro.status = campo.empty() ? '\0' : campo[0];

        CriarBinario(novoRegistro, contador);
    }

    entrada.close();
    cout << "Dados importados do arquivo CSV." << endl;
}

void mostrarDados(SequenceSet &sequenceSet) {
    bool mostrouTudo = false;
    bool fim = false;
    int i = 0;

    while (i < sequenceSet.quantidadeDeBlocos and fim == false) {
        if (mostrouTudo == false) {
            cout << "================================================== BLOCO: " << i + 1;
            cout << " =================================================" << endl;
            ifstream arquivo(to_string(i + 1) + ".bin", ios::binary);

            Bloco blocoAtual;
            arquivo.seekg(0);
            arquivo.read(reinterpret_cast<char*>(&blocoAtual.quantidadeDeRegistros), sizeof(blocoAtual.quantidadeDeRegistros));
            arquivo.read(reinterpret_cast<char*>(&blocoAtual.blocoAnterior), sizeof(blocoAtual.blocoAnterior));
            arquivo.read(reinterpret_cast<char*>(&blocoAtual.blocoProximo), sizeof(blocoAtual.blocoProximo));

            cout << "quantidade de registros: " << blocoAtual.quantidadeDeRegistros << endl;
            cout << "Bloco anterior: " << blocoAtual.blocoAnterior << endl;
            cout << "Proximo bloco: " << blocoAtual.blocoProximo << endl;
            Dado umDado;

            for (int j = 0; j < blocoAtual.quantidadeDeRegistros; j++) {
                arquivo.read(reinterpret_cast<char*>(&umDado), sizeof(Dado));
                cout << "\nID: " << umDado.id << " ";
                cout << "Time Ref: " << umDado.timeRef << " ";
                cout << "Account: " << umDado.account << " ";
                cout << "Code: " << umDado.code << " ";
                cout << "Country Code: " << umDado.countryCode << " ";
                cout << "Product Type: " << umDado.productType << " ";
                cout << "Value: " << umDado.value << " ";
                cout << "Status: " << umDado.status << endl << " ";
            }
            arquivo.close();
            i++;
            if (i == (sequenceSet.quantidadeDeBlocos - 1)) {
                mostrouTudo = true;
            }
            if (mostrouTudo == true) {
                fim = true;
                cout << "================================================== BLOCO: " << i + 1;
                cout << " =================================================" << endl;
                ifstream arquivo(to_string(i + 1) + ".bin", ios::binary);

                Bloco blocoAtual;
                arquivo.seekg(0);
                arquivo.read(reinterpret_cast<char*>(&blocoAtual.quantidadeDeRegistros), sizeof(blocoAtual.quantidadeDeRegistros));
                arquivo.read(reinterpret_cast<char*>(&blocoAtual.blocoAnterior), sizeof(blocoAtual.blocoAnterior));
                arquivo.read(reinterpret_cast<char*>(&blocoAtual.blocoProximo), sizeof(blocoAtual.blocoProximo));

                cout << "quantidade de registros: " << blocoAtual.quantidadeDeRegistros << endl;
                cout << "Bloco anterior: " << blocoAtual.blocoAnterior << endl;
                cout << "Proximo bloco: " << blocoAtual.blocoProximo << endl;
                Dado umDado;

                for (int j = 0; j < blocoAtual.quantidadeDeRegistros; j++) {
                    arquivo.read(reinterpret_cast<char*>(&umDado), sizeof(Dado));
                    cout << "\nID: " << umDado.id << " ";
                    cout << "Time Ref: " << umDado.timeRef << " ";
                    cout << "Account: " << umDado.account << " ";
                    cout << "Code: " << umDado.code << " ";
                    cout << "Country Code: " << umDado.countryCode << " ";
                    cout << "Product Type: " << umDado.productType << " ";
                    cout << "Value: " << umDado.value << " ";
                    cout << "Status: " << umDado.status << endl << " ";
                }
                arquivo.close();
            }
        }
    }
}

void procurarDado(SequenceSet &sequenceSet, int id) {
    Dado umDado;
    Bloco blocoAtual;
    bool encontrado = false;

    for (int i = 0; i < sequenceSet.quantidadeDeBlocos; i++) { // loop para procurar em todos os Blocos pelo registro procurado
        ifstream arquivo(to_string(i + 1) + ".bin", ios::binary);
        
        if (!arquivo.is_open()) {
            cout << "nao foi encontrado o arquivo " << i + 1 << ".bin" << endl;
            continue;
        }

        arquivo.seekg(0);
        arquivo.read(reinterpret_cast<char*>(&blocoAtual.quantidadeDeRegistros), sizeof(blocoAtual.quantidadeDeRegistros));
        arquivo.read(reinterpret_cast<char*>(&blocoAtual.blocoAnterior), sizeof(blocoAtual.blocoAnterior));
        arquivo.read(reinterpret_cast<char*>(&blocoAtual.blocoProximo), sizeof(blocoAtual.blocoProximo));

        for (int j = 0; j < blocoAtual.quantidadeDeRegistros; j++) {
            arquivo.read(reinterpret_cast<char*>(&umDado), sizeof(Dado));
            if (umDado.id == id) { // comparação para achar em qual posição do Bloco o registro procurado está
                cout << "Registro encontrado:\n";
                cout << "ID: " << umDado.id << endl;
                cout << "Time Ref: " << umDado.timeRef << endl;
                cout << "Account: " << umDado.account << endl;
                cout << "Code: " << umDado.code << endl;
                cout << "Country Code: " << umDado.countryCode << endl;
                cout << "Product Type: " << umDado.productType << endl;
                cout << "Value: " << umDado.value << endl;
                cout << "Status: " << umDado.status << endl;
                encontrado = true;
                break;
            }
        }
        arquivo.close();
        if (encontrado) break;
    }

    if (!encontrado) {
        cout << "Registro com id " << id << " nao encontrado." << endl;
    }
}


void importarOutroArquivoCSV(string arquivoImportado, SequenceSet &sequenceSet) { // função para importar de um arquivo CSV diferente
    ifstream entrada(arquivoImportado);                                           // da baseatualizada.csv
    if (!entrada.is_open()) {
        cout << "Erro ao abrir o arquivo CSV.\n";
        return;
    }

    string linha;
    Dado novoRegistro;
    
    getline(entrada, linha);

    while (getline(entrada, linha)) { // lê 
        stringstream ss(linha);
        string campo;

        getline(ss, campo, ',');
        novoRegistro.id = stoi(campo);

        getline(ss, campo, ',');
        try {
            novoRegistro.timeRef = static_cast<float>(stod(campo));
        } catch (const std::invalid_argument&) {
            novoRegistro.timeRef = 0;
        }
        getline(ss, campo, ',');
        strncpy(novoRegistro.account, campo.c_str(), sizeof(novoRegistro.account));
        novoRegistro.account[sizeof(novoRegistro.account) - 1] = '\0';

        getline(ss, campo, ',');
        try {
            novoRegistro.code = static_cast<float>(stod(campo));
        } catch (const std::invalid_argument&) {
            novoRegistro.code = 0;
        }

        getline(ss, campo, ',');
        strncpy(novoRegistro.countryCode, campo.c_str(), sizeof(novoRegistro.countryCode));
        novoRegistro.countryCode[sizeof(novoRegistro.countryCode) - 1] = '\0';

        getline(ss, campo, ',');
        strncpy(novoRegistro.productType, campo.c_str(), sizeof(novoRegistro.productType));
        novoRegistro.productType[sizeof(novoRegistro.productType) - 1] = '\0';

        getline(ss, campo, ',');
        try {
            novoRegistro.value = static_cast<float>(stod(campo));
        } catch (const std::invalid_argument&) {
            novoRegistro.value = 0;
        }

        getline(ss, campo, ',');
        try {
            novoRegistro.status = static_cast<float>(stod(campo));
        } catch (const std::invalid_argument&) {
            novoRegistro.status = 0;
        }

        adicionaNoBloco(novoRegistro, sequenceSet);
    }

    entrada.close();
    cout << "Dados importados do arquivo CSV" << endl;
}

int main() {
    int cont = 0;
    int opcao, opcao2;
    Dado umDado;
    SequenceSet sequenceSet;

    for (int i = 0; i < 10; i++) { 
        criarBloco(cont);
    }

    importarDeArquivoCSV();
    menu();
    cin >> opcao;
    (void)system("cls||clear");
    while (opcao != 5) {
        switch (opcao) {
            case 1: // adicionar dado
                cout << "1. adicionar dados via arquivo" << endl
                <<"2. adicionar dados via entrada padrao" << endl;
                cin >> opcao2;
                cin.ignore();
                if (opcao2 == 1) { // adicionar dado importando de arquivo
                    string arquivoImportado;
                    cout << "Digite o nome do arquivo: ";
                    cin >> arquivoImportado;
                    importarOutroArquivoCSV(arquivoImportado, sequenceSet);
                }
                else if (opcao2 == 2) { // adicionar dado por entrada normal (cin)
                    cout << "Digite os dados do registro que deseja adicionar:" << endl;
                    cout << "ID: ";
                    cin >> umDado.id;
                    cout << "Time Reference: ";
                    cin >> umDado.timeRef;
                    cin.ignore();
                    cout << "Account: ";
                    cin.getline(umDado.account, sizeof(umDado.account));
                    cout << "Code: ";
                    cin >> umDado.code;
                    cin.ignore();
                    cout << "Country Code: ";
                    cin.getline(umDado.countryCode, sizeof(umDado.countryCode));
                    cout << "Product Type: ";
                    cin.getline(umDado.productType, sizeof(umDado.productType));
                    cout << "Value: ";
                    cin >> umDado.value;
                    cout << "Status: ";
                    cin >> umDado.status;
                    adicionaNoBloco(umDado, sequenceSet);
                }
                break;

            case 2: // excluir dado
                cout << "id do registro que sera deletado: ";
                cin >> umDado.id;

                excluirDado(sequenceSet,umDado);

                break;

            case 3: // buscar dado
                cout << "id do registro que sera procurado: ";
                int id;
                cin >> id;

                procurarDado(sequenceSet, id);
                break;

            case 4: // mostrar dados
                mostrarDados(sequenceSet);
                break;

            case 5: // sair
                break;

            default:
                cout << "Opcao invalida" << endl;
                break;
        }
        menu();
        cin >> opcao;
        (void)system("cls||clear");
    }
    return 0;
}