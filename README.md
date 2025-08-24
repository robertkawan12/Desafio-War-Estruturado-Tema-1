## 🛠️ Funcionalidades

### Nível 1 ✅

-   Cadastro de territórios com:
    -   Nome
    -   Cor do exército
    -   Número de tropas
-   Exibição dos territórios cadastrados.

### Nível 2 ⚔️

-   **Alocação dinâmica** → o usuário define quantos territórios deseja cadastrar (usando `calloc`).
-   **Simulação de ataque**:
    -   O jogador escolhe um território atacante e um defensor.
    -   Cada lado rola um dado (valor entre 1 e 6).
    -   Se o atacante vencer:
        -   O defensor muda de dono (recebe a cor do atacante).
        -   O defensor recebe metade das tropas do atacante.
        -   O atacante perde metade das tropas.
    -   Se o defensor vencer:
        -   O atacante perde **1 tropa**.
-   **Validações**:
    -   Não é possível atacar territórios da mesma cor.
    -   Não é possível atacar com apenas 1 tropa.
-   **Liberação de memória** → uso de `free` ao final do programa.

### Nível 3 ✅

Criação de vetor de missões (mín. 5).

Sorteio de missão com atribuirMissao(char* destino, char* missoes[], int total).

Missão armazenada dinamicamente com malloc por jogador.

Verificação de missão com verificarMissao(char* missao, Territorio* mapa, int tamanho).

Ataque com rand() (1–6), atualizando cor e tropas conforme regras.

Ponteiros em todos os acessos (territórios, missões, jogadores).

Modularização: funções claras (atribuirMissao, verificarMissao, exibirMissao, atacar, exibirMapa, liberarMemoria, main).

Liberação de memória com free.
