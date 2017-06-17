#ifndef TP_FINAL_MYNEURALNETWORK_H
#define TP_FINAL_MYNEURALNETWORK_H

#include "RigidBody2D.h"
#include "NeuralNetwork.h"

extern NeuralNetwork TheBrain;

void InitializeBrain();
void EndBrain();
void TrainTheBrain();
void ReTrainTheBrain(const RigidBody2D& unit, double d0, double d1, double d2);

#endif
