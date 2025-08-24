#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura do território
typedef struct
{
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Função para cadastrar os territórios
void cadastrarTerritorios(Territorio *mapa, int qtd)
{
    for (int i = 0; i < qtd; i++)
    {
        printf("\n=== Cadastro do %dº território ===\n", i + 1);

        printf("Digite o nome do território: ");
        scanf(" %[^\n]", (mapa + i)->nome);

        printf("Digite a cor do exército: ");
        scanf(" %[^\n]", (mapa + i)->cor);

        printf("Digite a quantidade de tropas: ");
        scanf("%d", &(mapa + i)->tropas);
    }
}

// Função para exibir os territórios
void exibirTerritorios(Territorio *mapa, int qtd)
{
    printf("\n===== Territórios cadastrados =====\n");
    for (int i = 0; i < qtd; i++)
    {
        printf("\nTerritório %d:\n", i + 1);
        printf("Nome: %s\n", (mapa + i)->nome);
        printf("Cor: %s\n", (mapa + i)->cor);
        printf("Tropas: %d\n", (mapa + i)->tropas);
    }
}

// Função de ataque entre dois territórios
void atacar(Territorio *atacante, Territorio *defensor)
{
    if (strcmp(atacante->cor, defensor->cor) == 0)
    {
        printf("\n⚠️ Não é possível atacar um território da mesma cor!\n");
        return;
    }

    if (atacante->tropas <= 1)
    {
        printf("\n⚠️ O território atacante não possui tropas suficientes para atacar!\n");
        return;
    }

    int dadoAtacante = (rand() % 6) + 1; // valor de 1 a 6
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n🎲 Rolagem de dados:\n");
    printf("%s (Atacante) tirou: %d\n", atacante->nome, dadoAtacante);
    printf("%s (Defensor) tirou: %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor)
    {
        printf("\n✅ O atacante venceu a batalha!\n");
        strcpy(defensor->cor, atacante->cor);    // defensor muda de cor
        defensor->tropas = atacante->tropas / 2; // metade das tropas migram
        atacante->tropas /= 2;                   // atacante perde metade das tropas
    }
    else
    {
        printf("\n❌ O defensor resistiu ao ataque!\n");
        atacante->tropas--; // atacante perde uma tropa
    }
}

// Função para liberar memória
void liberarMemoria(Territorio *mapa)
{
    free(mapa);
}

int main()
{
    srand(time(NULL)); // garante aleatoriedade

    int qtdTerritorios;
    printf("Digite a quantidade de territórios: ");
    scanf("%d", &qtdTerritorios);

    // Alocação dinâmica
    Territorio *mapa = (Territorio *)calloc(qtdTerritorios, sizeof(Territorio));
    if (mapa == NULL)
    {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    // Cadastro e exibição inicial
    cadastrarTerritorios(mapa, qtdTerritorios);
    exibirTerritorios(mapa, qtdTerritorios);

    // Simulação de ataque
    int atacante, defensor;
    printf("\nEscolha o território atacante (1 a %d): ", qtdTerritorios);
    scanf("%d", &atacante);
    printf("Escolha o território defensor (1 a %d): ", qtdTerritorios);
    scanf("%d", &defensor);

    if (atacante < 1 || atacante > qtdTerritorios || defensor < 1 || defensor > qtdTerritorios)
    {
        printf("\n⚠️ Escolha inválida de territórios!\n");
    }
    else
    {
        atacar(&mapa[atacante - 1], &mapa[defensor - 1]);
    }

    // Exibe territórios após o ataque
    exibirTerritorios(mapa, qtdTerritorios);

    // Liberação de memória
    liberarMemoria(mapa);

    return 0;
}
