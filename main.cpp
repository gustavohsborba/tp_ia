#include "Gui.h"
#include "magic.h"


int main(int argc, char **argv){
    Initialize();
    printf("initialized\n");
    initGui(argc, argv, drawCrafts, UpdateSimulation);
    glutMainLoop();
}