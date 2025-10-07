#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Estruturas de Dados ---

// Estrutura para representar um Território no mapa
typedef struct {
    char nome[30]; // Nome do território
    char cor[10];  // Cor do jogador que o domina (ex: "Vermelho", "Azul")
    int tropas;    // Número de tropas no território
} Territorio;

// --- Protótipos das Funções ---

void inicializarJogo(Territorio** mapa, int* numTerritorios, char** missaoJogador);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
void exibirMissao(const char* missao);
int verificarMissao(const char* missao, Territorio* mapa, int tamanho, const char* corJogador);
void exibirMapa(Territorio* mapa, int tamanho);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa, char* missaoJogador);

// --- Implementação das Missões Pré-Definidas ---

#define MAX_MISSOES 5
char* MISSOES[] = {
    "Missao A: Conquistar 3 territorios seguidos.",       // Condicao: 3 territorios com a cor do jogador
    "Missao B: Eliminar todas as tropas da cor Azul.",    // Condicao: Nao ha mais territorios "Azul"
    "Missao C: Conquistar 5 territorios com pelo menos 4 tropas.", // Condicao: 5 territorios com corJogador e tropas >= 4
    "Missao D: Conquistar o territorio 'Canada'.",       // Condicao: Territorio "Canada" com a cor do jogador
    "Missao E: Conquistar 10 territorios no total."       // Condicao: 10 territorios com a cor do jogador
};

// A cor do jogador para quem a missao sera sorteada (simplificacao)
#define COR_JOGADOR "Verde"

// --- Função Principal (main) ---
int main() {
    // Inicializa a semente para a geracao de numeros aleatorios
    srand(time(NULL));

    // Ponteiros para alocacao dinamica
    Territorio* mapa = NULL;
    int numTerritorios = 0;
    char* missaoJogador = NULL; // Ponteiro para a string da missao

    // Inicializa o jogo: mapa e missao
    inicializarJogo(&mapa, &numTerritorios, &missaoJogador);

    printf("\n--- Bem-vindo ao War Estruturado! ---\n");
    printf("Voce jogara com a cor: %s\n", COR_JOGADOR);

    // Exibicao da missao (Passagem por valor, pois 'missaoJogador' e const)
    exibirMissao(missaoJogador);

    // Loop principal do jogo (Simulacao de turnos)
    for (int turno = 1; turno <= 3; turno++) {
        printf("\n\n=============== TURNO %d ==============\n", turno);
        exibirMapa(mapa, numTerritorios);

        // --- Simulacao de Acoes ---
        printf("\nSimulando ataque...\n");
        // O jogador "Verde" (i=0) ataca o territorio do jogador "Vermelho" (i=1)
        // Acesso usando ponteiros e aritmetica de ponteiros: *(mapa + indice)
        atacar((mapa + 0), (mapa + 1));
        
        // --- Verificacao da Missao (Passagem por referencia implicita de 'mapa') ---
        if (verificarMissao(missaoJogador, mapa, numTerritorios, COR_JOGADOR)) {
            printf("\n#################################################\n");
            printf("# PARABENS! O Jogador %s CUMPRIU SUA MISSAO! #\n", COR_JOGADOR);
            printf("# VENCEDOR: %s com a missao: %s #\n", COR_JOGADOR, missaoJogador);
            printf("#################################################\n");
            break; // Sai do loop principal
        } else {
            printf("\nMissao nao cumprida neste turno.\n");
        }
    }

    // Libera toda a memoria alocada dinamicamente
    liberarMemoria(mapa, missaoJogador);

    printf("\nJogo finalizado. Memoria liberada.\n");

    return 0;
}

// --- Funções Auxiliares ---

/**
 * @brief Inicializa o mapa com alguns territorios e sorteia a missao do jogador.
 * @param mapa Ponteiro para o ponteiro do vetor de Territorios (para alocar).
 * @param numTerritorios Ponteiro para o numero total de territorios (para atualizar).
 * @param missaoJogador Ponteiro para o ponteiro da string da missao (para alocar e preencher).
 */
void inicializarJogo(Territorio** mapa, int* numTerritorios, char** missaoJogador) {
    // 1. Alocacao dinamica dos territorios
    *numTerritorios = 4;
    // Usa calloc para garantir que a memoria seja zerada
    *mapa = (Territorio*)calloc(*numTerritorios, sizeof(Territorio));
    if (*mapa == NULL) {
        perror("Erro ao alocar o mapa");
        exit(EXIT_FAILURE);
    }

    // Inicializacao dos dados (acesso via aritmetica de ponteiros)
    // T1: Jogador "Verde"
    strcpy((*mapa + 0)->nome, "Brasil");
    strcpy((*mapa + 0)->cor, COR_JOGADOR);
    (*mapa + 0)->tropas = 5;

    // T2: Jogador "Vermelho" (inimigo)
    strcpy((*mapa + 1)->nome, "Argentina");
    strcpy((*mapa + 1)->cor, "Vermelho");
    (*mapa + 1)->tropas = 3;

    // T3: Jogador "Azul" (alvo de uma missao)
    strcpy((*mapa + 2)->nome, "Canada");
    strcpy((*mapa + 2)->cor, "Azul");
    (*mapa + 2)->tropas = 2;

    // T4: Jogador "Verde"
    strcpy((*mapa + 3)->nome, "Peru");
    strcpy((*mapa + 3)->cor, COR_JOGADOR);
    (*mapa + 3)->tropas = 4;

    // 2. Alocacao dinamica da missao do jogador (Tamanho da maior missao + 1 para '\0')
    int maxLen = 0;
    for (int i = 0; i < MAX_MISSOES; i++) {
        int len = strlen(MISSOES[i]);
        if (len > maxLen) maxLen = len;
    }

    // Aloca espaco para a missao
    *missaoJogador = (char*)malloc(maxLen + 1);
    if (*missaoJogador == NULL) {
        perror("Erro ao alocar a missao");
        free(*mapa); // Libera o mapa antes de sair
        exit(EXIT_FAILURE);
    }

    // 3. Atribuicao da Missao (Passagem da string 'missaoJogador' por referencia)
    atribuirMissao(*missaoJogador, MISSOES, MAX_MISSOES);
}

/**
 * @brief Sorteia uma missao e a copia para o destino.
 * @param destino Ponteiro para a string onde a missao sera copiada (passagem por referencia).
 * @param missoes Vetor de strings com as missoes pre-definidas.
 * @param totalMissoes Numero total de missoes.
 */
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    // Copia a string da missao para o espaco de memoria alocado dinamicamente
    strcpy(destino, missoes[indiceSorteado]);
}

/**
 * @brief Exibe a missao atual do jogador.
 * @param missao Ponteiro constante para a string da missao (passagem por valor conceitual).
 */
void exibirMissao(const char* missao) {
    printf("\n---------------- SUA MISSAO ----------------\n");
    printf("| %s\n", missao);
    printf("--------------------------------------------\n");
}

/**
 * @brief Verifica se a condicao de vitoria da missao foi atingida.
 * @param missao Ponteiro constante para a string da missao (passagem por valor conceitual).
 * @param mapa Ponteiro para o vetor de territorios (para inspecionar o estado do jogo).
 * @param tamanho Tamanho do mapa.
 * @param corJogador Cor do jogador cuja missao esta sendo verificada.
 * @return 1 se a missao foi cumprida, 0 caso contrario.
 */
int verificarMissao(const char* missao, Territorio* mapa, int tamanho, const char* corJogador) {
    int territoriosDojogador = 0;
    int territoriosCom4Tropas = 0;
    int corAzulEliminada = 1; // Assume que sim, muda para 0 se encontrar
    int canadaConquistado = 0;

    // Laco para percorrer o mapa e verificar condicoes de vitoria
    for (int i = 0; i < tamanho; i++) {
        // Acesso a cada territorio usando aritmetica de ponteiros: *(mapa + i)
        Territorio* t = (mapa + i);

        if (strcmp(t->cor, corJogador) == 0) {
            territoriosDojogador++;
            if (t->tropas >= 4) {
                territoriosCom4Tropas++;
            }
            if (strcmp(t->nome, "Canada") == 0) {
                canadaConquistado = 1;
            }
        }

        if (strcmp(t->cor, "Azul") == 0) {
            corAzulEliminada = 0;
        }
    }

    // Verifica as condicoes de missao com base no prefixo (simplificacao da logica)
    if (strstr(missao, "Missao A:") != NULL) { // Conquistar 3 territorios seguidos (logica simplificada para 3 territorios)
        return (territoriosDojogador >= 3);
    }
    if (strstr(missao, "Missao B:") != NULL) { // Eliminar todas as tropas da cor Azul
        return corAzulEliminada;
    }
    if (strstr(missao, "Missao C:") != NULL) { // Conquistar 5 territorios com pelo menos 4 tropas
        return (territoriosCom4Tropas >= 5);
    }
    if (strstr(missao, "Missao D:") != NULL) { // Conquistar o territorio 'Canada'
        return canadaConquistado;
    }
    if (strstr(missao, "Missao E:") != NULL) { // Conquistar 10 territorios no total
        return (territoriosDojogador >= 10);
    }

    return 0; // Missao desconhecida ou nao cumprida
}

/**
 * @brief Exibe o estado atual de cada territorio no mapa.
 * @param mapa Ponteiro para o vetor de territorios.
 * @param tamanho Tamanho do mapa.
 */
void exibirMapa(Territorio* mapa, int tamanho) {
    printf("\n--- Estado Atual do Mapa ---\n");
    for (int i = 0; i < tamanho; i++) {
        Territorio* t = (mapa + i); // Acesso via aritmetica de ponteiros
        printf("[%d] %-10s | Dono: %-8s | Tropas: %d\n",
               i, t->nome, t->cor, t->tropas);
    }
    printf("----------------------------\n");
}

/**
 * @brief Simula um ataque entre dois territorios.
 * @param atacante Ponteiro para o territorio atacante (referencia).
 * @param defensor Ponteiro para o territorio defensor (referencia).
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    // Validacao do requisito: so pode atacar territorios inimigos
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("!! Falha no ataque: %s nao pode atacar a si mesmo/territorio aliado.\n", atacante->nome);
        return;
    }

    // Rolagem de dados (1 a 6)
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("%s (%s, %d tropas) ataca %s (%s, %d tropas).\n",
           atacante->nome, atacante->cor, atacante->tropas,
           defensor->nome, defensor->cor, defensor->tropas);
    printf("Dados: Atacante (%d) vs Defensor (%d)\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        // Atacante vence
        printf("Atacante VENCE! %s conquista %s.\n", atacante->nome, defensor->nome);

        // Atualizacao de campos
        strcpy(defensor->cor, atacante->cor);
        int tropasTransferidas = atacante->tropas / 2;
        defensor->tropas = tropasTransferidas;
        atacante->tropas -= tropasTransferidas;
    } else {
        // Defensor vence ou empate
        printf("Defensor RESISTE! %s perde 1 tropa.\n", atacante->nome);
        atacante->tropas -= 1;
        // Garante que o numero de tropas nao fique negativo
        if (atacante->tropas < 1) {
            atacante->tropas = 1; // Um territorio precisa de pelo menos 1 tropa
        }
    }
}

/**
 * @brief Libera toda a memoria alocada dinamicamente para evitar vazamentos.
 * @param mapa Ponteiro para o vetor de territorios.
 * @param missaoJogador Ponteiro para a string da missao.
 */
void liberarMemoria(Territorio* mapa, char* missaoJogador) {
    if (mapa != NULL) {
        free(mapa);
        printf("\nMemoria do mapa liberada.\n");
    }
    if (missaoJogador != NULL) {
        free(missaoJogador);
        printf("Memoria da missao liberada.\n");
    }
}