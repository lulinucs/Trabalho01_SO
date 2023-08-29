# Space Invaders - Implementação Simplificada

## Descrição do Problema

O objetivo deste trabalho é aplicar os conceitos de threads, exclusão mútua e coordenação de processos por meio do projeto e implementação de uma versão simplificada do jogo clássico Space Invaders.

Nesta versão, 20 naves alienígenas estão se preparando para atacar a Terra, e a única defesa disponível é uma bateria antiaérea que dispara foguetes para abater as naves. Ao contrário do jogo original, não há alienígenas saltando das naves.

As naves se movem da direita para a esquerda da tela, em alturas diferentes acima da metade da tela. O jogador controla a bateria antiaérea, disparando contra as naves. Os disparos são sempre verticais, mas o jogo pode ser implementado com variações no ângulo de tiro. O jogador pode disparar usando o teclado ou o mouse. O jogo termina quando o tempo t se esgota antes que todas as 20 naves sejam abatidas (nesse caso, o jogador e a Terra perdem) ou quando todas as 20 naves forem derrubadas.

## Interface

O código fornecido no anexo inclui uma versão simples da interface usando caracteres ASCII. Essa versão é compilável em ambiente Windows. Caso queira alterar para Linux, a função `gotoxy(int x, int y)` deve ser redefinida, assim como o comando `system("cls")`. Você pode optar por usar essa interface ou aprimorá-la, se preferir.

## Instruções de Uso

1. Clone ou faça o download deste repositório para o seu sistema.
2. Compile o código em um ambiente Linux.
3. Execute o jogo e divirta-se!

## Contribuições

Este projeto foi desenvolvido como parte do seu trabalho acadêmico para aplicar conceitos de threads, exclusão mútua e coordenação de processos. Se você encontrar bugs ou quiser contribuir com melhorias, fique à vontade para abrir uma issue ou um pull request.
