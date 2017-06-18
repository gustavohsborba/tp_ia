#include "game/Gui.h"
#include "game/magic.h"


/* 
 * TODO:
 *     [ ] Colocar um objetivo no jogo (ex matar todo mundo)
 *     [ ] Colocar os hitpoints do Unit[0] como entrada da rede neural
 *     [ ] Preparar o texto
 *     [ ] Preparar a apresentacao
 *     [ ] Re treinar a rede neural a cada período de tempo, e não só quando um cara morre.
 *
 * EXTRAS: 
 *     [ ] Colocar menu, menu de pausa e tela de 'voce ganhou'
 *     [ ] Criar uma versão sem jogador (com dois times)
 *     [ ] Colocar o poder de ataque do jogador como entrada da rede neural
 */


int main(int argc, char **argv){
    initialize();
    printf("initialized\n");
    initGui(argc, argv, drawCrafts, updateSimulation);
    glutMainLoop();
}