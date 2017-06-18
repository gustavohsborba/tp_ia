#include "game/Gui.h"
#include "game/magic.h"



int main(int argc, char **argv){
    initialize();
    printf("initialized\n");
    initGui(argc, argv, drawCrafts, updateSimulation);
    glutMainLoop();
}