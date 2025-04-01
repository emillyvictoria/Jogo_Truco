#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int numero;
    char naipe[10];
    int forca;
    int usada;
} Carta;

typedef struct {
    Carta mao[3];
    int pontos;
    int vitoriasNaRodada;
    Carta ultimaCarta;
} Jogador;


int calcularForca(Carta c) {
    if (c.numero == 4 && strcmp(c.naipe, "Paus") == 0) return 14;
    if (c.numero == 7 && strcmp(c.naipe, "Copas") == 0) return 13;
    if (c.numero == 1 && strcmp(c.naipe, "Espadas") == 0) return 12;
    if (c.numero == 1 && strcmp(c.naipe, "Paus") == 0) return 11;
    if (c.numero == 7) return 10;
    if (c.numero == 6) return 9;
    if (c.numero == 5) return 8;
    if (c.numero == 4) return 7;
    if (c.numero == 3) return 6;
    if (c.numero == 2) return 5;
    if (c.numero == 12) return 4;
    if (c.numero == 11) return 3;
    if (c.numero == 10) return 2;
    return 1;
}

void criarBaralho(Carta baralho[]) {
    char *naipes[] = {"Espadas", "Copas", "Ouros", "Paus"};
    int numeros[] = {1,2,3,4,5,6,7,10,11,12};
    int k = 0;

    for (int n = 0; n < 4; n++) {
        for (int i = 0; i < 10; i++) {
            baralho[k].numero = numeros[i];
            strcpy(baralho[k].naipe, naipes[n]);
            baralho[k].forca = calcularForca(baralho[k]);
            baralho[k].usada = 0;
            k++;
        }
    }
}

void embaralhar(Carta baralho[]) {
    for (int i = 0; i < 40; i++) {
        int r = rand() % 40;
        Carta temp = baralho[i];
        baralho[i] = baralho[r];
        baralho[r] = temp;
    }
}

void mostrarCarta(Carta c) {
    printf("%d de %s", c.numero, c.naipe);
}

void mostrarMao(Jogador j) {
    for (int i = 0; i < 3; i++) {
        if (!j.mao[i].usada) {
            printf("[%d] ", i + 1);
            mostrarCarta(j.mao[i]);
            printf("\n");
        }
    }
}


int comparar(Carta c1, Carta c2) {
    if (c1.forca > c2.forca) return 1;
    if (c2.forca > c1.forca) return 2;
    return 0;
}

int todasCartasUsadas(Jogador j) {
    return j.mao[0].usada && j.mao[1].usada && j.mao[2].usada;
}

Carta escolherCarta(Jogador *j, int jogadorNum) {
    int escolha;
    do {
        printf("Jogador %d, escolha a carta (1 a 3): ", jogadorNum);
        scanf("%d", &escolha);
        escolha--;
    } while (escolha < 0 || escolha >= 3 || j->mao[escolha].usada);

    j->mao[escolha].usada = 1;
    j->ultimaCarta = j->mao[escolha];
    return j->mao[escolha];
}

// Rodada

void jogarRodada(Jogador *j1, Jogador *j2, Carta baralho[], int *indice, int numRodada) {
    int valorRodada = 1;
    int aumentoPendente = 0;
    int proximoValor[] = {3, 6, 9};
    int aumentoIndex = 0;
    int quemPropôs = -1;

    printf("\n== Rodada %d ==\n", numRodada);
    for (int i = 0; i < 3; i++) {
        j1->mao[i] = baralho[(*indice)++];
        j2->mao[i] = baralho[(*indice)++];
        j1->mao[i].usada = 0;
        j2->mao[i].usada = 0;
    }

    j1->vitoriasNaRodada = 0;
    j2->vitoriasNaRodada = 0;

    int mao = 1;
    while (1) {
        printf("\n--- Mão %d | Rodada vale %d ponto(s) ---\n", mao, valorRodada);
        if (j1->ultimaCarta.forca > 0) {
            printf("Última carta do jogador 1: ");
            mostrarCarta(j1->ultimaCarta);
            printf("\n");
        }
        if (j2->ultimaCarta.forca > 0) {
            printf("Última carta do jogador 2: ");
            mostrarCarta(j2->ultimaCarta);
            printf("\n");
        }

        // jogador 1
        int turnoFeito = 0;
        while (!turnoFeito) {
            printf("\nJogador 1, sua mão:\n");
            mostrarMao(*j1);
            printf("Ações: [1] Jogar Carta  [2] Aumentar  [3] Desistir\n");
            int acao;
            scanf("%d", &acao);
            if (acao == 1) {
                Carta carta1 = escolherCarta(j1, 1);
                // jogador 2
                int turnoFeito2 = 0;
                while (!turnoFeito2) {
                    printf("\nJogador 2, sua mão:\n");
                    mostrarMao(*j2);
                    printf("Ações: [1] Jogar Carta  [2] Aumentar  [3] Desistir\n");
                    int acao2;
                    scanf("%d", &acao2);
                    if (acao2 == 1) {
                        Carta carta2 = escolherCarta(j2, 2);
                        printf("\nJogador 1 jogou: ");
                        mostrarCarta(carta1);
                        printf("\nJogador 2 jogou: ");
                        mostrarCarta(carta2);
                        printf("\n");

                        int resultado = comparar(carta1, carta2);
                        if (resultado == 1) {
                            printf("Jogador 1 venceu a mão!\n");
                            j1->vitoriasNaRodada++;
                        } else if (resultado == 2) {
                            printf("Jogador 2 venceu a mão!\n");
                            j2->vitoriasNaRodada++;
                        } else {
                            printf("Empate na mão!\n");
                        }
                        turnoFeito = 1;
                        turnoFeito2 = 1;
                        mao++;
                    } else if (acao2 == 2 && quemPropôs != 2 && aumentoIndex < 3) {
                        printf("Jogador 2 pediu truco! (%d pontos)\n", proximoValor[aumentoIndex]);
                        quemPropôs = 2;
                        printf("Jogador 1 aceita? [1] Sim  [2] Não: ");
                        int resp;
                        scanf("%d", &resp);
                        if (resp == 1) {
                            valorRodada = proximoValor[aumentoIndex++];
                            printf("Rodada agora vale %d pontos!\n", valorRodada);
                        } else {
                            printf("Jogador 1 desistiu! Jogador 2 vence a rodada.\n");
                            j2->pontos += valorRodada;
                            return;
                        }
                    } else if (acao2 == 3) {
                        printf("Jogador 2 desistiu! Jogador 1 vence a rodada.\n");
                        j1->pontos += valorRodada;
                        return;
                    } else {
                        printf("Ação inválida.\n");
                    }
                }
            } else if (acao == 2 && quemPropôs != 1 && aumentoIndex < 3) {
                printf("Jogador 1 pediu truco! (%d pontos)\n", proximoValor[aumentoIndex]);
                quemPropôs = 1;
                printf("Jogador 2 aceita? [1] Sim  [2] Não: ");
                int resp;
                scanf("%d", &resp);
                if (resp == 1) {
                    valorRodada = proximoValor[aumentoIndex++];
                    printf("Rodada agora vale %d pontos!\n", valorRodada);
                } else {
                    printf("Jogador 2 desistiu! Jogador 1 vence a rodada.\n");
                    j1->pontos += valorRodada;
                    return;
                }
            } else if (acao == 3) {
                printf("Jogador 1 desistiu! Jogador 2 vence a rodada.\n");
                j2->pontos += valorRodada;
                return;
            } else {
                printf("Ação inválida.\n");
            }
        }

        // Verificar fim da rodada
        if (j1->vitoriasNaRodada == 2) {
            printf("\nJogador 1 venceu a rodada!\n");
            j1->pontos += valorRodada;
            break;
        } else if (j2->vitoriasNaRodada == 2) {
            printf("\nJogador 2 venceu a rodada!\n");
            j2->pontos += valorRodada;
            break;
        } else if (mao > 3) {
            printf("\nEmpate! Rodada sem vencedor.\n");
            break;
        }
    }

    printf("Placar: Jogador 1 = %d | Jogador 2 = %d\n", j1->pontos, j2->pontos);
}

int main() {
    srand(time(NULL));
    Jogador j1 = {.pontos = 0};
    Jogador j2 = {.pontos = 0};
    Carta baralho[40];
    criarBaralho(baralho);
    embaralhar(baralho);

    int rodada = 1;
    int indice = 0;

    while (j1.pontos < 12 && j2.pontos < 12) {
        if (indice > 28) {
            criarBaralho(baralho);
            embaralhar(baralho);
            indice = 0;
        }
        jogarRodada(&j1, &j2, baralho, &indice, rodada++);
    }

    printf("\n===== FIM DO JOGO =====\n");
    if (j1.pontos >= 12)
        printf(" Jogador 1 venceu!\n");
    else
        printf(" Jogador 2 venceu!\n");

    return 0;
}
