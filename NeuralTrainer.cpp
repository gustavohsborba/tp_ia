#include "NeuralTrainer.h"

NeuralNetwork TheBrain;

double TrainingSet[14][7] = {
//#Friends, Hit points, Enemy Engaged, Range, Chase, Flock, Evade
        0,		1,		    0,		0.2,	0.9,	0.1,	0.1,
        0,		1,		    1,		0.2,	0.9,	0.1,	0.1,
        0,		1,		    0,		0.8,	0.1,	0.1,	0.1,
        0.1,	0.5,	    0,		0.2,	0.9,	0.1,	0.1,
        0,		0.25,	    1,		0.5,	0.1,	0.9,	0.1,
        0,		0.2,	    1,		0.2,	0.1,	0.1,	0.9,
        0.3,	0.2,	    0,		0.2,	0.9,	0.1,	0.1,
        0,		0.2,	    0,		0.3,	0.1,	0.9,	0.1,
        0,		1,		    0,		0.2,	0.1,	0.9,	0.1,
        0,		1,		    1,		0.6,	0.1,	0.1,	0.1,
        0,		1,		    0,		0.8,	0.1,	0.9,	0.1,
        0.1,	0.2,	    0,		0.2,	0.1,	0.1,	0.9,
        0,		0.25,	    1,		0.5,	0.1,	0.1,	0.9,
        0,		0.6,	    0,		0.2,	0.1,	0.1,	0.9
};

void TrainTheBrain() {
    int		i;
    double	error = 1;
    int		c = 0;

    TheBrain.DumpData("PreTraining.txt");

    while((error > 0.05) && (c<50000)) {
        error = 0;
        c++;
        for(i=0; i<14; i++) {
            TheBrain.SetInput(0, TrainingSet[i][0]);
            TheBrain.SetInput(1, TrainingSet[i][1]);
            TheBrain.SetInput(2, TrainingSet[i][2]);
            TheBrain.SetInput(3, TrainingSet[i][3]);


            TheBrain.SetDesiredOutput(0, TrainingSet[i][4]);
            TheBrain.SetDesiredOutput(1, TrainingSet[i][5]);
            TheBrain.SetDesiredOutput(2, TrainingSet[i][6]);
            TheBrain.SetDesiredOutput(3, TrainingSet[i][7]);

            TheBrain.FeedForward();
            error += TheBrain.CalculateError();
            TheBrain.BackPropagate();

        }
        error = error / 14.0f;
    }

    TheBrain.DumpData("PostTraining.txt");
}

void ReTrainTheBrain(const RigidBody2D& unit, double d0, double d1, double d2) {
    double	error = 1;
    int		c = 0;

    while((error > 0.1) && (c<5000)) {

        c++;
        TheBrain.SetInput(0, unit.Inputs[0]);
        TheBrain.SetInput(1, unit.Inputs[1]);
        TheBrain.SetInput(2, unit.Inputs[2]);
        TheBrain.SetInput(3, unit.Inputs[3]);
        TheBrain.SetDesiredOutput(0, d0);
        TheBrain.SetDesiredOutput(1, d1);
        TheBrain.SetDesiredOutput(2, d2);

        TheBrain.FeedForward();
        error = TheBrain.CalculateError();
        TheBrain.BackPropagate();
    }
}

void InitializeBrain() {
    TheBrain.Initialize(4, 3, 3);
    TheBrain.SetLearningRate(0.2);
    TheBrain.SetMomentum(true, 0.9);
}

void EndBrain(){
    TheBrain.DumpData("Final.txt");
    TheBrain.CleanUp();
}