#include "NeuralTrainer.h"
#include "../engine/RigidBody2D.h"

NeuralNetwork theBrain;

double TrainingSet[14][7] = {
//#Friends, Hit points, Enemy Engaged, Range,  Chase,  Flock,  Evade
        0,        1,          0,        0.2,    0.9,    0.1,    0.1,
        0,        1,          1,        0.2,    0.9,    0.1,    0.1,
        0,        1,          0,        0.8,    0.1,    0.1,    0.1,
        0.1,      0.5,        0,        0.2,    0.9,    0.1,    0.1,
        0,        0.25,       1,        0.5,    0.1,    0.9,    0.1,
        0,        0.2,        1,        0.2,    0.1,    0.1,    0.9,
        0.3,      0.2,        0,        0.2,    0.9,    0.1,    0.1,
        0,        0.2,        0,        0.3,    0.1,    0.9,    0.1,
        0,        1,          0,        0.2,    0.1,    0.9,    0.1,
        0,        1,          1,        0.6,    0.1,    0.1,    0.1,
        0,        1,          0,        0.8,    0.1,    0.9,    0.1,
        0.1,      0.2,        0,        0.2,    0.1,    0.1,    0.9,
        0,        0.25,       1,        0.5,    0.1,    0.1,    0.9,
        0,        0.6,        0,        0.2,    0.1,    0.1,    0.9
};

void trainTheBrain() {
    int		i;
    double	error = 1;
    int		c = 0;

    theBrain.dumpData("PreTraining.txt");

    while((error > 0.05) && (c<50000)) {
        error = 0;
        c++;
        for(i=0; i<14; i++) {
            theBrain.setInput(0, TrainingSet[i][0]);
            theBrain.setInput(1, TrainingSet[i][1]);
            theBrain.setInput(2, TrainingSet[i][2]);
            theBrain.setInput(3, TrainingSet[i][3]);


            theBrain.setDesiredOutput(0, TrainingSet[i][4]);
            theBrain.setDesiredOutput(1, TrainingSet[i][5]);
            theBrain.setDesiredOutput(2, TrainingSet[i][6]);
            theBrain.setDesiredOutput(3, TrainingSet[i][7]);

            theBrain.feedForward();
            error += theBrain.calculateError();
            theBrain.backPropagate();

        }
        error = error / 14.0f;
    }

    theBrain.dumpData("PostTraining.txt");
}

void reTrainTheBrain(const double inputs[], double d0, double d1, double d2) {
    double	error = 1;
    int		c = 0;

    while((error > 0.1) && (c<5000)) {

        c++;
        theBrain.setInput(0, inputs[0]);
        theBrain.setInput(1, inputs[1]);
        theBrain.setInput(2, inputs[2]);
        theBrain.setInput(3, inputs[3]);
        theBrain.setDesiredOutput(0, d0);
        theBrain.setDesiredOutput(1, d1);
        theBrain.setDesiredOutput(2, d2);

        theBrain.feedForward();
        error = theBrain.calculateError();
        theBrain.backPropagate();
    }
}

void initializeTheBrain() {
    theBrain.initialize(NEURAL_INPUTS, 3, 3);
    theBrain.setLearningRate(0.2);
    theBrain.setMomentum(true, 0.9);
}

void endBrain(){
    theBrain.dumpData("Final.txt");
    theBrain.cleanUp();
}