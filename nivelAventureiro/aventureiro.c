#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
#define TAM_NOME 30
#define TAM_COR 10
#define MIN_TROPAS 2 // Mínimo de tropas para atacar (deve sobrar 1 no atacante)

// ---------------------- DEFINIÇÃO DA ESTRUTURA DE DADOS ---------------------

/**
 * @brief Estrutura que representa um Território no jogo WAR.
 */
typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

// ---------------------- PROTÓTIPOS DAS FUNÇÕES -----------------------------

// Funções de Setup e Gerenciamento de Memória
int lerNumTerritorios();
Territorio* alocarMapa(int numTerritorios);
void cadastrarTerritorios(Territorio* mapa, int numTerritorios);
void liberarMemoria(Territorio* mapa);

// Funções de Lógica Principal do Jogo
void exibirMapa(const Territorio* mapa, int numTerritorios);
void faseDeAtaque(Territorio* mapa, int numTerritorios);
void atacar(Territorio* atacante, Territorio* defensor);
int rolarDado();

// ---------------------- FUNÇÃO PRINCIPAL (MAIN) -----------------------------

int main() {
    // 1. Configuração Inicial (Setup)
    
    // Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // Essencial para o rand() simular dados de forma aleatória.
    srand(time(NULL)); 
    
    Territorio* mapa = NULL; // Ponteiro para o início do vetor de territórios.
    int numTerritorios = 0;
    int escolha;

    // Alocação e Cadastro
    numTerritorios = lerNumTerritorios();
    mapa = alocarMapa(numTerritorios);
    
    // Verifica se a alocação foi bem-sucedida.
    if (mapa == NULL) {
        printf("\nERRO: Falha ao alocar memória. Encerrando o programa.\n");
        return 1;
    }
    
    cadastrarTerritorios(mapa, numTerritorios);

    // 2. Laço Principal do Jogo (Game Loop)
    do {
        printf("\n====================================================\n");
        printf("                MENU DE AÇÕES - WAR\n");
        printf("====================================================\n");
        
        // Exibe o estado atual do mapa antes do menu.
        exibirMapa(mapa, numTerritorios);

        printf("\n[1] Iniciar Fase de Ataque\n");
        printf("[0] Sair do Jogo e Liberar Memória\n");
        printf("Escolha sua ação: ");
        
        if (scanf("%d", &escolha) != 1) {
            printf("\nOpção inválida. Por favor, insira um número.\n");
            // Limpa o buffer em caso de entrada não numérica
            while (getchar() != '\n'); 
            escolha = -1; // Garante que o loop continue
            continue;
        }

        switch (escolha) {
            case 1:
                faseDeAtaque(mapa, numTerritorios);
                break;
            case 0:
                printf("\nEncerrando o jogo...\n");
                break;
            default:
                printf("\nOpção não reconhecida. Tente novamente.\n");
                break;
        }
        
    } while (escolha != 0);

    // 3. Limpeza
    liberarMemoria(mapa);
    
    return 0;
}

// ---------------------- IMPLEMENTAÇÃO DAS FUNÇÕES -----------------------------

/**
 * @brief Lê do usuário o número de territórios que serão criados.
 * @return O número de territórios desejado.
 */
int lerNumTerritorios() {
    int num;
    do {
        printf("Quantos territórios você deseja cadastrar (mínimo 2)? ");
        if (scanf("%d", &num) != 1 || num < 2) {
            printf("Entrada inválida. Por favor, digite um número inteiro maior ou igual a 2.\n");
            // Limpa o buffer de entrada
            while (getchar() != '\n');
        } else {
            return num;
        }
    } while(1);
}


/**
 * @brief Aloca dinamicamente a memória para o vetor de territórios.
 * @param numTerritorios O tamanho do vetor a ser alocado.
 * @return Um ponteiro para o vetor de Territorio ou NULL em caso de falha.
 */
Territorio* alocarMapa(int numTerritorios) {
    // calloc(n, size) aloca memória para n elementos de tamanho 'size' e a inicializa com zero.
    Territorio* mapa = (Territorio*)calloc(numTerritorios, sizeof(Territorio));
    return mapa;
}

/**
 * @brief Preenche os dados iniciais de cada território.
 * @param mapa Ponteiro para o primeiro elemento do vetor de Territorio (referência).
 * @param numTerritorios O tamanho do vetor.
 */
void cadastrarTerritorios(Territorio* mapa, int numTerritorios) {
    printf("\n--- CADASTRO INICIAL DE TERRITÓRIOS (%d no total) ---\n", numTerritorios);
    for (int i = 0; i < numTerritorios; i++) {
        // Acesso aos campos do struct através de ponteiro: (mapa + i) é o endereço do i-ésimo elemento.
        // O operador '->' (ou (*(mapa + i)).campo) é usado para acessar um membro de um struct através de um ponteiro.
        printf("\nTerritório %d:\n", i);
        
        printf("  Nome (máx. %d): ", TAM_NOME - 1);
        scanf("%29s", (mapa + i)->nome); 

        printf("  Cor do Exército (máx. %d): ", TAM_COR - 1);
        scanf("%9s", (mapa + i)->cor);
        
        // Garante que o território comece com tropas suficientes para jogar.
        do {
            printf("  Tropas (mínimo %d): ", MIN_TROPAS);
            scanf("%d", &(mapa + i)->tropas);
            if ((mapa + i)->tropas < MIN_TROPAS) {
                printf("O território deve ter pelo menos %d tropas para ser jogável. Tente novamente.\n", MIN_TROPAS);
            }
        } while ((mapa + i)->tropas < MIN_TROPAS);
    }
}

/**
 * @brief Libera a memória previamente alocada dinamicamente.
 * @param mapa Ponteiro para o início da memória a ser liberada.
 */
void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
        printf("\nMemória do mapa liberada com sucesso.\n");
    }
}

/**
 * @brief Exibe o estado atual de todos os territórios no mapa.
 * @param mapa Ponteiro constante para o primeiro elemento (apenas leitura).
 * @param numTerritorios O tamanho do vetor.
 */
void exibirMapa(const Territorio* mapa, int numTerritorios) {
    printf("\n----------------------- MAPA ATUAL ------------------------\n");
    printf("| %-4s | %-30s | %-10s | %-6s |\n", "ID", "NOME", "DONO", "TROPAS");
    printf("|------|--------------------------------|------------|--------|\n");
    
    for (int i = 0; i < numTerritorios; i++) {
        // Acesso aos campos do struct através de ponteiro constante.
        printf("| %-4d | %-30s | %-10s | %-6d |\n", 
               i, 
               (mapa + i)->nome, 
               (mapa + i)->cor, 
               (mapa + i)->tropas);
    }
    printf("-----------------------------------------------------------\n");
}

/**
 * @brief Simula a rolagem de um dado (número aleatório entre 1 e 6).
 * @return O valor do dado rolado.
 */
int rolarDado() {
    // rand() % 6 gera um número entre 0 e 5. Adicionando 1, temos um número entre 1 e 6.
    return (rand() % 6) + 1;
}

/**
 * @brief Gerencia a interface e a lógica de seleção de territórios para o ataque.
 * @param mapa Ponteiro para o vetor de territórios.
 * @param numTerritorios O tamanho do vetor.
 */
void faseDeAtaque(Territorio* mapa, int numTerritorios) {
    int idAtacante, idDefensor;
    
    printf("\n--- INÍCIO DA FASE DE ATAQUE ---\n");
    
    // 1. Escolha do atacante e validação
    do {
        printf("ID do Território ATACANTE: ");
        scanf("%d", &idAtacante);
        
        // Verifica se o ID é válido
        if (idAtacante < 0 || idAtacante >= numTerritorios) {
            printf("ID inválido. Escolha um ID entre 0 e %d.\n", numTerritorios - 1);
            continue;
        }
        
        // Valida se o atacante tem tropas suficientes
        if ((mapa + idAtacante)->tropas < MIN_TROPAS) {
            printf("Território %s não tem tropas suficientes (mínimo %d). Escolha outro.\n", 
                   (mapa + idAtacante)->nome, MIN_TROPAS);
        }
    } while (idAtacante < 0 || idAtacante >= numTerritorios || (mapa + idAtacante)->tropas < MIN_TROPAS);

    // 2. Escolha do defensor e validação
    do {
        printf("ID do Território DEFENSOR: ");
        scanf("%d", &idDefensor);
        
        // Verifica se o ID é válido
        if (idDefensor < 0 || idDefensor >= numTerritorios || idDefensor == idAtacante) {
            printf("ID inválido ou igual ao atacante. Escolha um ID diferente entre 0 e %d.\n", numTerritorios - 1);
            continue;
        }
        
        // Validação: não pode atacar território da mesma cor.
        if (strcmp((mapa + idAtacante)->cor, (mapa + idDefensor)->cor) == 0) {
            printf("Não é possível atacar um território da mesma cor (%s). Escolha outro defensor.\n", 
                   (mapa + idDefensor)->cor);
        }
    } while (idDefensor < 0 || idDefensor >= numTerritorios || idDefensor == idAtacante || 
             strcmp((mapa + idAtacante)->cor, (mapa + idDefensor)->cor) == 0);
             
    // 3. Execução do ataque, passando os ponteiros para as structs.
    // &mapa[id] ou (mapa + id) obtêm o endereço do struct no vetor.
    atacar((mapa + idAtacante), (mapa + idDefensor));

    printf("\n--- RESULTADO DA BATALHA ---\n");
    exibirMapa(mapa, numTerritorios); // Exibe o mapa atualizado
}


/**
 * @brief Simula a lógica de uma batalha e atualiza as tropas/donos.
 * @param atacante Ponteiro para o struct do território atacante (modifica).
 * @param defensor Ponteiro para o struct do território defensor (modifica).
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    // Ponteiros são usados para ler e MODIFICAR diretamente os dados na memória alocada.
    
    printf("\nBatalha: %s (%s) ataca %s (%s)!\n", 
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    int dadoAtacante = rolarDado();
    int dadoDefensor = rolarDado();

    printf("Dados Rolados: Atacante (%d) vs. Defensor (%d)\n", dadoAtacante, dadoDefensor);

    // Lógica simples de combate: quem tirar o maior número vence.
    if (dadoAtacante > dadoDefensor) {
        printf("O ATAQUE VENCEU! %s perde 1 tropa.\n", defensor->nome);
        defensor->tropas--;
        
        // Verifica se o defensor perdeu todas as tropas.
        if (defensor->tropas <= 0) {
            printf("Território CONQUISTADO! %s agora pertence a %s.\n", defensor->nome, atacante->cor);
            
            // Transferência de controle (cor)
            strcpy(defensor->cor, atacante->cor);
            
            // Movimentação de tropas (metade das tropas do atacante são movidas)
            int tropasMovidas = atacante->tropas / 2;
            
            // O atacante DEVE manter pelo menos 1 tropa em seu território original.
            if (tropasMovidas >= atacante->tropas) {
                tropasMovidas = atacante->tropas - 1; 
            }
            
            atacante->tropas -= tropasMovidas;
            defensor->tropas = tropasMovidas; // O defensor agora tem as tropas transferidas
            
            printf("Tropas Movidas: %d tropas transferidas de %s para %s.\n", 
                   tropasMovidas, atacante->nome, defensor->nome);
        }
    } else { // Inclui empate (em War, o defensor geralmente vence empates)
        printf("O DEFENSOR VENCEU! %s perde 1 tropa.\n", atacante->nome);
        // O atacante perde 1 tropa. Deve sobrar pelo menos 1 tropa.
        if (atacante->tropas > 1) {
            atacante->tropas--;
        } else {
            printf("O atacante %s não pode perder mais tropas (mínimo 1 mantido).\n", atacante->nome);
        }
    }
}