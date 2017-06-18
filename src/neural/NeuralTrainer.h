#ifndef TP_FINAL_MYNEURALNETWORK_H
#define TP_FINAL_MYNEURALNETWORK_H

#include "NeuralNetwork.h"

extern NeuralNetwork theBrain;

void initializeTheBrain();
void endBrain();
void trainTheBrain();
void reTrainTheBrain(const double inputs[], double d0, double d1, double d2);

#endif
