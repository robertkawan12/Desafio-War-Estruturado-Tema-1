#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* =========================================
   Estruturas e assinaturas
   ========================================= */

typedef struct
{
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

typedef struct
{
    char nome[30];
    char cor[10];
    char *missao;   // armazenada dinamicamente (malloc)
    int conquistas; // conta quantas vezes este jogador conquistou território
} Jogador;

/* --- Assinaturas de funções --- */
void cadastrarTerritorios(Territorio *mapa, int qtd);
void exibirMapa(Territorio *mapa, int qtd);

void atribuirMissao(char *destino, char *missoes[], int totalMissoes);
void exibirMissao(const char *missao, const char *nomeJogador);

void atacar(Territorio *atacante, Territorio *defensor, Jogador *jogAtacante);

int escolherIndiceTerritorio(Territorio *mapa, int qtd, const char *rotulo);
int pertenceAoJogador(const Territorio *t, const char *corJogador);

int verificarMissao(char *missao, Territorio *mapa, int tamanho); /* assinatura pedida */
void liberarMemoria(Territorio *mapa, Jogador *jogs, int qtdJogadores);

/*
   Para cumprir a assinatura pedida de verificarMissao(missao, mapa, tamanho),
   usamos uma variável global para indicar a "cor em verificação" no momento
   (o jogador cujo objetivo está sendo checado).
   Assim mantemos a função com a assinatura exigida, mas ainda checamos por jogador.
*/
static const char *g_corEmVerificacao = NULL;
static int g_conquistasDoJogador = 0;

/* =========================================
   Implementação
   ========================================= */

void cadastrarTerritorios(Territorio *mapa, int qtd)
{
    for (int i = 0; i < qtd; i++)
    {
        printf("\n=== Cadastro do %dº território ===\n", i + 1);
        printf("Nome: ");
        scanf(" %29[^\n]", (mapa + i)->nome);
        printf("Cor do exército: ");
        scanf(" %9[^\n]", (mapa + i)->cor);
        printf("Tropas: ");
        scanf("%d", &(mapa + i)->tropas);
    }
}

void exibirMapa(Territorio *mapa, int qtd)
{
    printf("\n===== MAPA ATUAL =====\n");
    for (int i = 0; i < qtd; i++)
    {
        printf("%2d) %-20s | Cor: %-10s | Tropas: %d\n",
               i + 1, (mapa + i)->nome, (mapa + i)->cor, (mapa + i)->tropas);
    }
}

/* Sorteia uma missão do vetor e copia para destino (pré-alocado com malloc) */
void atribuirMissao(char *destino, char *missoes[], int totalMissoes)
{
    int idx = rand() % totalMissoes;
    strcpy(destino, missoes[idx]);
}

/* Exibe a missão (passagem por valor para leitura) */
void exibirMissao(const char *missao, const char *nomeJogador)
{
    printf("\n🎯 Missão de %s: %s\n", nomeJogador, missao);
}

/* Retorna 1 se o território pertence (mesma cor), 0 caso contrário */
int pertenceAoJogador(const Territorio *t, const char *corJogador)
{
    return (strcmp(t->cor, corJogador) == 0);
}

/* Função de ataque com dados (1 a 6) e regras do enunciado */
void atacar(Territorio *atacante, Territorio *defensor, Jogador *jogAtacante)
{
    if (strcmp(atacante->cor, defensor->cor) == 0)
    {
        printf("\n⚠️ Você não pode atacar um território da mesma cor!\n");
        return;
    }
    if (atacante->tropas <= 1)
    {
        printf("\n⚠️ Tropas insuficientes para atacar (precisa de > 1)!\n");
        return;
    }

    int dadoA = (rand() % 6) + 1;
    int dadoD = (rand() % 6) + 1;

    printf("\n🎲 Rolagem de dados:\n");
    printf("Atacante (%s - %s) tirou: %d\n", atacante->nome, atacante->cor, dadoA);
    printf("Defensor  (%s - %s) tirou: %d\n", defensor->nome, defensor->cor, dadoD);

    if (dadoA > dadoD)
    {
        printf("✅ Ataque bem-sucedido! %s conquistou %s.\n", jogAtacante->nome, defensor->nome);
        /* Regras: defensor muda de cor, recebe metade das tropas do atacante, atacante perde metade */
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas /= 2;

        /* Marca conquista para missões dependentes */
        jogAtacante->conquistas += 1;
    }
    else
    {
        printf("🛡️ Defesa bem-sucedida! O atacante perdeu 1 tropa.\n");
        atacante->tropas -= 1;
        if (atacante->tropas < 0)
            atacante->tropas = 0;
    }
}

/* Utilitário para o jogador escolher um território pelo índice (1..qtd) */
int escolherIndiceTerritorio(Territorio *mapa, int qtd, const char *rotulo)
{
    int idx;
    printf("\nEscolha o território %s (1 a %d): ", rotulo, qtd);
    scanf("%d", &idx);
    if (idx < 1 || idx > qtd)
    {
        printf("⚠️ Índice inválido.\n");
        return -1;
    }
    return idx - 1;
}

/*
   Verifica a missão do jogador atual (cor em g_corEmVerificacao):
   Missões implementadas (todas independentes do texto exato, baseadas em prefixos):
   1) "Controlar pelo menos 3 territórios"
   2) "Acumular 20 tropas no total"
   3) "Conquistar 1 território neste jogo"
   4) "Ser a única cor no mapa"
   5) "Eliminar todas as tropas da cor vermelha"
*/
int verificarMissao(char *missao, Territorio *mapa, int tamanho)
{
    if (g_corEmVerificacao == NULL)
        return 0;

    /* 1) Controlar pelo menos 3 territórios */
    if (strncmp(missao, "Controlar pelo menos 3 territorios", 34) == 0 ||
        strncmp(missao, "Controlar pelo menos 3 territórios", 34) == 0)
    {
        int count = 0;
        for (int i = 0; i < tamanho; i++)
        {
            if (pertenceAoJogador(&mapa[i], g_corEmVerificacao))
                count++;
        }
        return count >= 3;
    }

    /* 2) Acumular 20 tropas no total */
    if (strncmp(missao, "Acumular 20 tropas no total", 27) == 0)
    {
        int soma = 0;
        for (int i = 0; i < tamanho; i++)
        {
            if (pertenceAoJogador(&mapa[i], g_corEmVerificacao))
            {
                soma += mapa[i].tropas;
            }
        }
        return soma >= 20;
    }

    /* 3) Conquistar 1 território neste jogo */
    if (strncmp(missao, "Conquistar 1 territorio neste jogo", 34) == 0 ||
        strncmp(missao, "Conquistar 1 território neste jogo", 34) == 0)
    {
        return g_conquistasDoJogador >= 1;
    }

    /* 4) Ser a única cor no mapa */
    if (strncmp(missao, "Ser a unica cor no mapa", 23) == 0 ||
        strncmp(missao, "Ser a única cor no mapa", 23) == 0)
    {
        char corViva[10] = {0};
        for (int i = 0; i < tamanho; i++)
        {
            if (mapa[i].tropas > 0)
            {
                if (corViva[0] == '\0')
                {
                    strcpy(corViva, mapa[i].cor);
                }
                else if (strcmp(corViva, mapa[i].cor) != 0)
                {
                    return 0; // há ao menos duas cores
                }
            }
        }
        // Se só há 1 cor viva, checar se é a do jogador
        return (corViva[0] != '\0' && strcmp(corViva, g_corEmVerificacao) == 0);
    }

    /* 5) Eliminar todas as tropas da cor vermelha */
    if (strncmp(missao, "Eliminar todas as tropas da cor vermelha", 40) == 0)
    {
        for (int i = 0; i < tamanho; i++)
        {
            if (strcmp(mapa[i].cor, "Vermelho") == 0 && mapa[i].tropas > 0)
            {
                return 0;
            }
        }
        return 1;
    }

    /* Caso a missão não seja reconhecida, considerar não cumprida */
    return 0;
}

void liberarMemoria(Territorio *mapa, Jogador *jogs, int qtdJogadores)
{
    free(mapa);
    for (int i = 0; i < qtdJogadores; i++)
    {
        free(jogs[i].missao);
    }
}

/* =========================================
   Função principal
   ========================================= */
int main(void)
{
    srand((unsigned)time(NULL));

    /* --- Vetor de missões (mínimo 5) --- */
    char *missoes[] = {
        "Controlar pelo menos 3 territorios",
        "Acumular 20 tropas no total",
        "Conquistar 1 territorio neste jogo",
        "Ser a unica cor no mapa",
        "Eliminar todas as tropas da cor vermelha"};
    int totalMissoes = (int)(sizeof(missoes) / sizeof(missoes[0]));

    /* --- Configuração do mapa --- */
    int qtdTerritorios;
    printf("Quantidade de territorios no mapa: ");
    scanf("%d", &qtdTerritorios);

    Territorio *mapa = (Territorio *)calloc(qtdTerritorios, sizeof(Territorio));
    if (!mapa)
    {
        printf("Erro de alocacao de memoria para o mapa.\n");
        return 1;
    }
    cadastrarTerritorios(mapa, qtdTerritorios);
    exibirMapa(mapa, qtdTerritorios);

    /* --- Jogadores (2 jogadores) --- */
    const int qtdJogadores = 2;
    Jogador jogadores[qtdJogadores];

    for (int j = 0; j < qtdJogadores; j++)
    {
        printf("\n=== Cadastro do Jogador %d ===\n", j + 1);
        printf("Nome do jogador: ");
        scanf(" %29[^\n]", jogadores[j].nome);
        printf("Cor do jogador (deve existir no mapa para ter chance real): ");
        scanf(" %9[^\n]", jogadores[j].cor);

        jogadores[j].missao = (char *)malloc(128); // missão armazenada dinamicamente
        if (!jogadores[j].missao)
        {
            printf("Erro de alocacao de memoria para a missao do jogador.\n");
            liberarMemoria(mapa, jogadores, j); // libera os já alocados
            return 1;
        }
        atribuirMissao(jogadores[j].missao, missoes, totalMissoes);
        jogadores[j].conquistas = 0;
    }

    /* Exibe a missão de cada jogador apenas uma vez (no início) */
    for (int j = 0; j < qtdJogadores; j++)
    {
        exibirMissao(jogadores[j].missao, jogadores[j].nome);
    }

    /* --- Loop de turnos simples --- */
    int vencedor = -1;
    int turno = 0;

    while (vencedor == -1)
    {
        int j = turno % qtdJogadores;
        Jogador *J = &jogadores[j];

        printf("\n==============================\n");
        printf("Turno de %s (Cor: %s)\n", J->nome, J->cor);
        exibirMapa(mapa, qtdTerritorios);

        /* Escolha do ataque */
        int idxAtac = escolherIndiceTerritorio(mapa, qtdTerritorios, "ATACANTE");
        if (idxAtac < 0)
        {
            turno++;
            continue;
        }
        int idxDef = escolherIndiceTerritorio(mapa, qtdTerritorios, "DEFENSOR");
        if (idxDef < 0)
        {
            turno++;
            continue;
        }

        /* Validação: atacante precisa pertencer ao jogador atual */
        if (!pertenceAoJogador(&mapa[idxAtac], J->cor))
        {
            printf("⚠️ O território atacante selecionado não pertence à sua cor (%s).\n", J->cor);
            turno++;
            continue;
        }
        /* Validação: o defensor deve ser inimigo */
        if (strcmp(mapa[idxDef].cor, J->cor) == 0)
        {
            printf("⚠️ Você não pode atacar um território da sua própria cor.\n");
            turno++;
            continue;
        }

        atacar(&mapa[idxAtac], &mapa[idxDef], J);

        /* Após o ataque, checar vitória de todos os jogadores (silenciosamente).
           Para cumprir a assinatura exigida de verificarMissao, setamos o contexto global
           (cor e conquistas do jogador checado) imediatamente antes da chamada. */
        for (int k = 0; k < qtdJogadores; k++)
        {
            g_corEmVerificacao = jogadores[k].cor;
            g_conquistasDoJogador = jogadores[k].conquistas;

            if (verificarMissao(jogadores[k].missao, mapa, qtdTerritorios))
            {
                vencedor = k;
                break;
            }
        }

        if (vencedor != -1)
            break;

        turno++;
    }

    if (vencedor != -1)
    {
        printf("\n🏆 VITÓRIA! %s cumpriu sua missão: %s\n",
               jogadores[vencedor].nome, jogadores[vencedor].missao);
    }

    liberarMemoria(mapa, jogadores, qtdJogadores);
    return 0;
}
