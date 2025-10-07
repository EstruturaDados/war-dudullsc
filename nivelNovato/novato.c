#include <stdio.h>
#include <string.h>

// --- Constantes Globais para Manutenibilidade ---
// Define o número fixo de territórios a serem cadastrados.
#define NUM_TERRITORIOS 5
// Define o tamanho máximo para o nome de um território.
#define TAM_NOME 30
// Define o tamanho máximo para a cor do exército.
#define TAM_COR 10

// ----------------------------------------------------------------------------
// ---------------------- DEFINIÇÃO DA ESTRUTURA DE DADOS ---------------------
// ----------------------------------------------------------------------------

/**
 * @brief Estrutura que representa um Território no jogo WAR.
 * * Agrupa dados relacionados a um território, como seu nome, a cor do exército
 * que o domina e a quantidade de tropas presentes.
 */
typedef struct {
    char nome[TAM_NOME];   // Nome do território (ex: Brasil, Argentina)
    char cor[TAM_COR];     // Cor do exército que o domina (ex: Verde, Vermelho)
    int tropas;            // Número de tropas presentes no território
} Territorio;


// ----------------------------------------------------------------------------
// ------------------------------- FUNÇÃO PRINCIPAL -----------------------------
// ----------------------------------------------------------------------------

int main() {
    // Declaração do vetor de structs para armazenar os 5 territórios.
    // Isso cria um array estático na memória com 5 posições do tipo Territorio.
    Territorio mapa[NUM_TERRITORIOS];
    int i; // Variável de controle para os laços 'for'.

    printf("====================================================\n");
    printf("        CADASTRO INICIAL DE TERRITÓRIOS\n");
    printf("====================================================\n");

    // ------------------------------------------------------------------------
    // ENTRADA DOS DADOS: Laço para preencher os dados dos 5 territórios.
    // ------------------------------------------------------------------------
    for (i = 0; i < NUM_TERRITORIOS; i++) {
        printf("\n--- Cadastrando Território %d de %d ---\n", i + 1, NUM_TERRITORIOS);
        
        // 1. Entrada do NOME do território
        printf("Digite o NOME do território (máx. %d caracteres): ", TAM_NOME - 1);
        // %29s garante que a string lida não ultrapasse o tamanho de 'nome[30]', 
        // evitando estouro de buffer, e 'scanf' ignora espaços em branco antes da leitura.
        if (scanf("%29s", mapa[i].nome) != 1) {
             // Tratamento de erro básico
             printf("Erro ao ler o nome. Abortando.\n");
             return 1;
        }

        // 2. Entrada da COR do exército
        printf("Digite a COR do exército dominador (máx. %d caracteres): ", TAM_COR - 1);
        // Semelhante ao nome, limita a leitura.
        if (scanf("%9s", mapa[i].cor) != 1) {
             printf("Erro ao ler a cor. Abortando.\n");
             return 1;
        }

        // 3. Entrada da QUANTIDADE de tropas
        printf("Digite o número de TROPAS (valor inteiro): ");
        if (scanf("%d", &mapa[i].tropas) != 1) {
             // Se o usuário digitar algo que não é um número inteiro, 'scanf' falhará.
             printf("Entrada inválida para o número de tropas. Abortando.\n");
             return 1;
        }
        
        // Em um projeto maior, seria necessário limpar o buffer após o scanf
        // para evitar problemas, mas para este cadastro simples não é estritamente
        // necessário, pois todos os campos são lidos com scanf, um após o outro.
    }
    
    printf("\n====================================================\n");
    printf("           DADOS DOS TERRITÓRIOS CADASTRADOS\n");
    printf("====================================================\n");
    
    // Tabela de cabeçalho para melhor formatação
    printf("| %-30s | %-10s | %-6s |\n", "NOME", "DONO (COR)", "TROPAS");
    printf("|------------------------------|------------|--------|\n");

    // ------------------------------------------------------------------------
    // EXIBIÇÃO DOS DADOS: Laço para imprimir o conteúdo do vetor de structs.
    // ------------------------------------------------------------------------
    for (i = 0; i < NUM_TERRITORIOS; i++) {
        // Utilização de especificadores de formato de largura fixa (%-30s, %-10s, %-6d)
        // para garantir que os dados sejam exibidos em colunas alinhadas,
        // melhorando a usabilidade e clareza.
        printf("| %-30s | %-10s | %-6d |\n", 
               mapa[i].nome, 
               mapa[i].cor, 
               mapa[i].tropas);
    }
    printf("----------------------------------------------------\n");
    printf("Cadastro concluído com sucesso. %d territórios registrados.\n", NUM_TERRITORIOS);

    return 0;
}