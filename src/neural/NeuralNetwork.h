#ifndef _MYNEURALNETWORKHEADER
#define _MYNEURALNETWORKHEADER

#include "NeuralNetworkLayer.h"


class NeuralNetwork {
public:
    NeuralNetworkLayer InputLayer;
    NeuralNetworkLayer HiddenLayer;
    NeuralNetworkLayer OutputLayer;
    int nNodesInput;
    int nNodesOutput;
    int nNodesHidden;

    void initialize(int nNodesInput, int nNodesHidden, int nNodesOutput);
    void cleanUp();
    void setInput(int i, double value);
    double getOutput(int i);
    void setDesiredOutput(int i, double value);
    void feedForward();
    void backPropagate();
    int getMaxOutputID();
    double calculateError();
    void setLearningRate(double rate);
    void setLinearOutput(bool useLinear);
    void setMomentum(bool useMomentum, double factor);
    void dumpData(const char *filename);
};

#endif