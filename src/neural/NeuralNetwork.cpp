#include "NeuralNetwork.h"
#include <climits>
#include <cmath>
#include <cstdio>


//---------------------------------------------------------------------------
/*
Book:           AI for Game Developers
Authors:        David M. Bourg & Glenn Seemann
Example:        Neural Networks, Chapter 14
*/
//---------------------------------------------------------------------------

void NeuralNetwork::initialize(int nNodesInput, int nNodesHidden, int nNodesOutput) {
    InputLayer.NumberOfNodes = nNodesInput;
    InputLayer.NumberOfChildNodes = nNodesHidden;
    InputLayer.NumberOfParentNodes = 0;
    InputLayer.initialize(nNodesInput, NULL, &HiddenLayer);
    InputLayer.randomizeWeights();

    HiddenLayer.NumberOfNodes = nNodesHidden;
    HiddenLayer.NumberOfChildNodes = nNodesOutput;
    HiddenLayer.NumberOfParentNodes = nNodesInput;
    HiddenLayer.initialize(nNodesHidden, &InputLayer, &OutputLayer);
    HiddenLayer.randomizeWeights();

    OutputLayer.NumberOfNodes = nNodesOutput;
    OutputLayer.NumberOfChildNodes = 0;
    OutputLayer.NumberOfParentNodes = nNodesHidden;
    OutputLayer.initialize(nNodesOutput, &HiddenLayer, NULL);

}

void NeuralNetwork::cleanUp() {
    InputLayer.cleanUp();
    HiddenLayer.cleanUp();
    OutputLayer.cleanUp();
}

void NeuralNetwork::setInput(int i, double value) {
    if ((i >= 0) && (i < InputLayer.NumberOfNodes)) {
        InputLayer.NeuronValues[i] = value;
    }
}

double NeuralNetwork::getOutput(int i) {
    if ((i >= 0) && (i < OutputLayer.NumberOfNodes)) {
        return OutputLayer.NeuronValues[i];
    }

    return (double) INT_MAX; // to indicate an error
}

void NeuralNetwork::setDesiredOutput(int i, double value) {
    if ((i >= 0) && (i < OutputLayer.NumberOfNodes)) {
        OutputLayer.DesiredValues[i] = value;
    }
}

void NeuralNetwork::feedForward() {
    InputLayer.calculateNeuronValues();
    HiddenLayer.calculateNeuronValues();
    OutputLayer.calculateNeuronValues();
}

void NeuralNetwork::backPropagate() {
    OutputLayer.calculateErrors();
    HiddenLayer.calculateErrors();

    HiddenLayer.adjustWeights();
    InputLayer.adjustWeights();
}

int NeuralNetwork::getMaxOutputID() {
    int i, id;
    double maxval;

    maxval = OutputLayer.NeuronValues[0];
    id = 0;

    for (i = 1; i < OutputLayer.NumberOfNodes; i++) {
        if (OutputLayer.NeuronValues[i] > maxval) {
            maxval = OutputLayer.NeuronValues[i];
            id = i;
        }
    }

    return id;
}

double NeuralNetwork::calculateError() {
    int i;
    double error = 0;

    for (i = 0; i < OutputLayer.NumberOfNodes; i++) {
        error += pow(OutputLayer.NeuronValues[i] - OutputLayer.DesiredValues[i], 2);
    }

    error = error / OutputLayer.NumberOfNodes;

    return error;
}

void NeuralNetwork::setLearningRate(double rate) {
    InputLayer.LearningRate = rate;
    HiddenLayer.LearningRate = rate;
    OutputLayer.LearningRate = rate;
}

void NeuralNetwork::setLinearOutput(bool useLinear) {
    InputLayer.LinearOutput = useLinear;
    HiddenLayer.LinearOutput = useLinear;
    OutputLayer.LinearOutput = useLinear;
}

void NeuralNetwork::setMomentum(bool useMomentum, double factor) {
    InputLayer.UseMomentum = useMomentum;
    HiddenLayer.UseMomentum = useMomentum;
    OutputLayer.UseMomentum = useMomentum;

    InputLayer.MomentumFactor = factor;
    HiddenLayer.MomentumFactor = factor;
    OutputLayer.MomentumFactor = factor;

}

void NeuralNetwork::dumpData(const char *filename) {
    FILE *f;
    int i, j;

    f = fopen(filename, "w");

    fprintf(f, "--------------------------------------------------------\n");
    fprintf(f, "Input Layer\n");
    fprintf(f, "--------------------------------------------------------\n");
    fprintf(f, "\n");
    fprintf(f, "Node Values:\n");
    fprintf(f, "\n");
    for (i = 0; i < InputLayer.NumberOfNodes; i++)
        fprintf(f, "(%d) = %f\n", i, InputLayer.NeuronValues[i]);
    fprintf(f, "\n");
    fprintf(f, "Weights:\n");
    fprintf(f, "\n");
    for (i = 0; i < InputLayer.NumberOfNodes; i++)
        for (j = 0; j < InputLayer.NumberOfChildNodes; j++)
            fprintf(f, "(%d, %d) = %f\n", i, j, InputLayer.Weights[i][j]);
    fprintf(f, "\n");
    fprintf(f, "Bias Weights:\n");
    fprintf(f, "\n");
    for (j = 0; j < InputLayer.NumberOfChildNodes; j++)
        fprintf(f, "(%d) = %f\n", j, InputLayer.BiasWeights[j]);

    fprintf(f, "\n");
    fprintf(f, "\n");

    fprintf(f, "--------------------------------------------------------\n");
    fprintf(f, "Hidden Layer\n");
    fprintf(f, "--------------------------------------------------------\n");
    fprintf(f, "\n");
    fprintf(f, "Weights:\n");
    fprintf(f, "\n");
    for (i = 0; i < HiddenLayer.NumberOfNodes; i++)
        for (j = 0; j < HiddenLayer.NumberOfChildNodes; j++)
            fprintf(f, "(%d, %d) = %f\n", i, j, HiddenLayer.Weights[i][j]);
    fprintf(f, "\n");
    fprintf(f, "Bias Weights:\n");
    fprintf(f, "\n");
    for (j = 0; j < HiddenLayer.NumberOfChildNodes; j++)
        fprintf(f, "(%d) = %f\n", j, HiddenLayer.BiasWeights[j]);

    fprintf(f, "\n");
    fprintf(f, "\n");

    fprintf(f, "--------------------------------------------------------\n");
    fprintf(f, "Output Layer\n");
    fprintf(f, "--------------------------------------------------------\n");
    fprintf(f, "\n");
    fprintf(f, "Node Values:\n");
    fprintf(f, "\n");
    for (i = 0; i < OutputLayer.NumberOfNodes; i++)
        fprintf(f, "(%d) = %f\n", i, OutputLayer.NeuronValues[i]);
    fprintf(f, "\n");


    fclose(f);
}