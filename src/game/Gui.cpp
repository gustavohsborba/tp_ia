#include "Gui.h"

char keys[1024];
const int DOWN_NOW = 2;
const int DOWN = 1;

void keypress_callback(unsigned char key, int x, int y){
    if (key >= 0 && key < 1024){
        if(keys[key] == 0){
            keys[key] |= DOWN_NOW;
        }
        keys[key] |= DOWN;
    } else {
        printf("keycode inválido: %d\n", key);
        exit(1);
    }
}

void keyup_callback(unsigned char key, int x, int y){
    if (key >= 0 && key < 1024){
        keys[key] = 0;
    }
}

bool isKeyDown(int key) {
    return keys[key];
}

bool isKeyDownNow(int key){
    if(keys[key] & DOWN_NOW){
        keys[key] &= ~DOWN_NOW;
        return true;
    } else {
        return false;
    }
}

void initGui(int argc, char **argv, void (*drawFunc)(), void (*updateFunc)(int)) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(_WINWIDTH, _WINHEIGHT);
    glutCreateWindow("demonio 14");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0,0, _WINWIDTH, _WINHEIGHT);
    glOrtho(0, _WINWIDTH, _WINHEIGHT, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutDisplayFunc(drawFunc);
    glutIgnoreKeyRepeat(GLUT_KEY_REPEAT_ON);
    glutKeyboardFunc(keypress_callback);
    glutKeyboardUpFunc(keyup_callback);
    glutTimerFunc(30, updateFunc, 0);
}