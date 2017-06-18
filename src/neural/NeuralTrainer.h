#ifndef TP_FINAL_MYNEURALNETWORK_H
#define TP_FINAL_MYNEURALNETWORK_H

#include "../engine/RigidBody2D.h"
#include "NeuralNetwork.h"

extern NeuralNetwork theBrain;

void initializeTheBrain();
void endBrain();
void trainTheBrain();
void reTrainTheBrain(const RigidBody2D &unit, double d0, double d1, double d2);

#endif
