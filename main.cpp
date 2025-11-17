#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>

using namespace std;

// ==========================================================
// 1. ESTRUTURA DE DADOS (STRUCT)
// Representa o registro principal (Mídia: Filme/Série).
// Três campos de tipos diferentes: char[], char[], int.
// ==========================================================
struct Midia {
    int id;                // Identificador único (posição do registro)
    char titulo[100];      // Título da Mídia (char[])
    char genero[50];       // Gênero da Mídia (char[])
    int anoLancamento;     // Ano de Lançamento (int)
};

// ==========================================================
// PROTÓTIPOS DE FUNÇÕES (Modularização)
// ==========================================================
void limpaBuffer();
void ler_string_segura(char* destino, int tamanho);
long tamanho(FILE* arq);
void cadastrar(FILE* arq);
void consultar(FILE* arq);
void menu();

// ==========================================================
// FUNÇÃO PRINCIPAL
// ==========================================================
int main() {
    // 2. MÓDULO DE ARQUIVOS
    // Abrir/Criar um arquivo binário para leitura e escrita.
    // "r+b": Abre para leitura/escrita. O arquivo deve existir.
    FILE* arquivo = fopen("midias.bin", "r+b");

    if (arquivo == NULL) {
        // Se o arquivo não existe, cria um novo ("w+b")
        arquivo = fopen("midias.bin", "w+b");
        if (arquivo == NULL) {
            cerr << "Erro fatal: Não foi possível abrir ou criar o arquivo midias.bin." << endl;
            return 1; // Sair com código de erro
        }
        cout << "Arquivo 'midias.bin' criado com sucesso!" << endl;
    } else {
        cout << "Arquivo 'midias.bin' aberto com sucesso!" << endl;
    }

    int opcao;

    do {
        menu();
        cout << "Escolha uma opção: ";
        if (!(cin >> opcao)) {
            // Lida com entrada não numérica
            cin.clear();
            limpaBuffer();
            opcao = -1; // Opção inválida
        } else {
            limpaBuffer();
        }

        switch (opcao) {
            case 1:
                cadastrar(arquivo);
                break;
            case 2:
                consultar(arquivo);
                break;
            case 3:
                cout << "\nTotal de registros no arquivo: " << tamanho(arquivo) << endl;
                break;
            case 0:
                cout << "\nEncerrando o sistema. Até mais!" << endl;
                break;
            default:
                cout << "\nOpção inválida. Tente novamente." << endl;
                break;
        }
        cout << "------------------------------------------" << endl;
    } while (opcao != 0);

    // Fechar o arquivo ao sair
    fclose(arquivo);

    return 0;
}

// ==========================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES AUXILIARES
// ==========================================================

// Padronização de Nomenclatura: limpaBuffer()
void limpaBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Requisito Opcional: Leitura de String Segura com fgets() e strcspn()
void ler_string_segura(char* destino, int tamanho) {
    if (fgets(destino, tamanho, stdin) != NULL) {
        // Remover o '\n' deixado pelo fgets
        destino[strcspn(destino, "\n")] = 0;
    }
}

// ==========================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES DE NEGÓCIO
// ==========================================================

// tamanho(): Calcula e retorna o número total de registros
long tamanho(FILE* arq) {
    if (arq == NULL) {
        return 0;
    }

    // Posiciona o ponteiro no final do arquivo (SEEK_END)
    fseek(arq, 0, SEEK_END);

    // Obtém a posição atual (que é o tamanho total em bytes)
    long tamanho_bytes = ftell(arq);

    // Calcula o número de registros dividindo pelo tamanho da struct
    long num_registros = tamanho_bytes / sizeof(Midia);

    // Volta o ponteiro para o início para futuras operações
    fseek(arq, 0, SEEK_SET);

    return num_registros;
}

// cadastrar(): Lê dados do usuário e salva no final do arquivo
void cadastrar(FILE* arq) {
    Midia novaMidia;

    // Obtém o próximo ID, que é o número de registros atual + 1
    long total = tamanho(arq);
    novaMidia.id = (int)total + 1;

    cout << "\n--- CADASTRO DE NOVA MÍDIA ---" << endl;
    cout << "ID do Registro: " << novaMidia.id << endl;

    cout << "Título: ";
    ler_string_segura(novaMidia.titulo, sizeof(novaMidia.titulo));

    cout << "Gênero: ";
    ler_string_segura(novaMidia.genero, sizeof(novaMidia.genero));

    cout << "Ano de Lançamento (Ex: 2023): ";
    // Leitura simples de int, assumindo que limpaBuffer já foi chamada
    while (!(cin >> novaMidia.anoLancamento)) {
        cout << "Entrada inválida. Digite um número para o ano: ";
        cin.clear();
        limpaBuffer();
    }
    limpaBuffer(); // Limpar buffer após a leitura do inteiro

    // Posiciona o ponteiro no final do arquivo (SEEK_END)
    fseek(arq, 0, SEEK_END);

    // Salva a struct no final do arquivo (fwrite)
    if (fwrite(&novaMidia, sizeof(Midia), 1, arq) == 1) {
        cout << "\n Mídia '" << novaMidia.titulo << "' cadastrada com sucesso! (ID: " << novaMidia.id << ")" << endl;
    } else {
        cerr << "\n Erro ao salvar a mídia no arquivo." << endl;
    }

    // Garante que os dados sejam escritos no disco
    fflush(arq);
}

// consultar(): Solicita código (posição) e exibe o registro
void consultar(FILE* arq) {
    long total = tamanho(arq);
    if (total == 0) {
        cout << "\nNão há registros no arquivo para consultar." << endl;
        return;
    }

    int idConsulta;
    cout << "\n--- CONSULTA DE MÍDIA ---" << endl;
    cout << "Digite o ID (posição de 1 a " << total << ") do registro: ";

    if (!(cin >> idConsulta)) {
        cout << "Entrada inválida." << endl;
        cin.clear();
        limpaBuffer();
        return;
    }
    limpaBuffer(); // Limpar buffer

    if (idConsulta < 1 || idConsulta > total) {
        cout << "\nID inválido. Deve estar entre 1 e " << total << "." << endl;
        return;
    }

    Midia midiaConsultada;

    // Calcula o deslocamento (offset) em bytes
    long offset = (long)(idConsulta - 1) * sizeof(Midia);

    // Posiciona o ponteiro no registro desejado (SEEK_SET)
    fseek(arq, offset, SEEK_SET);

    // Lê o registro (fread)
    if (fread(&midiaConsultada, sizeof(Midia), 1, arq) == 1) {
        cout << "\n--- DADOS DA MÍDIA (ID: " << midiaConsultada.id << ") ---" << endl;
        cout << "  Título: " << midiaConsultada.titulo << endl;
        cout << "  Gênero: " << midiaConsultada.genero << endl;
        cout << "  Ano:    " << midiaConsultada.anoLancamento << endl;
    } else {
        cerr << "\n Erro ao ler o registro no ID " << idConsulta << "." << endl;
    }
}

// Função de menu
void menu() {
    cout << "\n=== SISTEMA DE COLEÇÃO DE MÍDIAS ===" << endl;
    cout << "1. Cadastrar nova Mídia" << endl;
    cout << "2. Consultar Mídia por ID" << endl;
    cout << "3. Exibir Total de Registros" << endl;
    cout << "0. Sair" << endl;
    cout << "====================================" << endl;
}