#include <stdio.h>
#include <string.h>

// Definição da struct Territorio
// Essa estrutura agrupa informações de um território do jogo.
typedef struct
{
    char nome[30]; // Nome do território
    char cor[10];  // Cor do exército
    int tropas;    // Quantidade de tropas
} Territorio;

int main()
{
    // Vetor que armazena até 5 territórios
    Territorio territorios[5];

    // Cadastro dos territórios
    for (int i = 0; i < 5; i++)
    {
        printf("\n=== Cadastro do %dº território ===\n", i + 1);

        printf("Digite o nome do território: ");
        scanf(" %[^\n]", territorios[i].nome); // Lê string com espaços

        printf("Digite a cor do exército: ");
        scanf(" %[^\n]", territorios[i].cor);

        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
    }

    // Exibição dos dados
    printf("\n===== Territórios cadastrados =====\n");
    for (int i = 0; i < 5; i++)
    {
        printf("\nTerritório %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do exército: %s\n", territorios[i].cor);
        printf("Tropas: %d\n", territorios[i].tropas);
    }

    return 0;
}
