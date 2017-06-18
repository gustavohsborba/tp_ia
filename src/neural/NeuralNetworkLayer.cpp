#include "NeuralNetworkLayer.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

NeuralNetworkLayer::NeuralNetworkLayer() {
    ParentLayer = nullptr;
    ChildLayer = nullptr;
    LinearOutput = false;
    UseMomentum = false;
    MomentumFactor = 0.9;
}

void NeuralNetworkLayer::initialize(int NumNodes, NeuralNetworkLayer *parent, NeuralNetworkLayer *child) {
    int i, j;

    // Allocate memory
    NeuronValues = new double[NumberOfNodes];
    DesiredValues = new double[NumberOfNodes];
    Errors = new double[NumberOfNodes];

    if (parent != nullptr) {
        ParentLayer = parent;
    }

    if (child != nullptr) {
        ChildLayer = child;


        Weights = new double *[NumberOfNodes];
        WeightChanges = new double *[NumberOfNodes];
        for (i = 0; i < NumberOfNodes; i++) {
            Weights[i] = new double[NumberOfChildNodes];
            WeightChanges[i] = new double[NumberOfChildNodes];
        }

        BiasValues = new double[NumberOfChildNodes];
        BiasWeights = new double[NumberOfChildNodes];
    } else {
        Weights = nullptr;
        BiasValues = nullptr;
        BiasWeights = nullptr;
    }

    // Make sure everything contains zeros
    for (i = 0; i < NumberOfNodes; i++) {
        NeuronValues[i] = 0;
        DesiredValues[i] = 0;
        Errors[i] = 0;

        if (ChildLayer != nullptr)
            for (j = 0; j < NumberOfChildNodes; j++) {
                Weights[i][j] = 0;
                WeightChanges[i][j] = 0;
            }
    }

    if (ChildLayer != nullptr)
        for (j = 0; j < NumberOfChildNodes; j++) {
            BiasValues[j] = -1;
            BiasWeights[j] = 0;
        }

}

void NeuralNetworkLayer::cleanUp() {
    int i;

    delete NeuronValues;
    delete DesiredValues;
    delete Errors;

    if (Weights != nullptr) {
        for (i = 0; i < NumberOfNodes; i++) {
            delete Weights[i];
            delete WeightChanges[i];
        }

        delete Weights;
        delete WeightChanges;
    }

    if (BiasValues != nullptr) delete BiasValues;
    if (BiasWeights != nullptr) delete BiasWeights;
}

void NeuralNetworkLayer::randomizeWeights() {
    int i, j;
    int min = 0;
    int max = 200;
    int number;

    srand((unsigned) time(nullptr));

    for (i = 0; i < NumberOfNodes; i++) {
        for (j = 0; j < NumberOfChildNodes; j++) {
            number = (((abs(rand()) % (max - min + 1)) + min));

            if (number > max)
                number = max;

            if (number < min)
                number = min;

            Weights[i][j] = number / 100.0f - 1;
        }
    }

    for (j = 0; j < NumberOfChildNodes; j++) {
        number = (((abs(rand()) % (max - min + 1)) + min));

        if (number > max)
            number = max;

        if (number < min)
            number = min;

        BiasWeights[j] = number / 100.0f - 1;
    }
}

void NeuralNetworkLayer::calculateErrors() {
    int i, j;
    double sum;

    if (ChildLayer == nullptr) // output layer
    {
        for (i = 0; i < NumberOfNodes; i++) {
            Errors[i] = (DesiredValues[i] - NeuronValues[i]) * NeuronValues[i] * (1.0f - NeuronValues[i]);
        }
    } else if (ParentLayer == nullptr) { // input layer
        for (i = 0; i < NumberOfNodes; i++) {
            Errors[i] = 0.0f;
        }
    } else { // hidden layer
        for (i = 0; i < NumberOfNodes; i++) {
            sum = 0;
            for (j = 0; j < NumberOfChildNodes; j++) {
                sum += ChildLayer->Errors[j] * Weights[i][j];
            }
            Errors[i] = sum * NeuronValues[i] * (1.0f - NeuronValues[i]);
        }
    }
}

void NeuralNetworkLayer::adjustWeights() {
    int i, j;
    double dw;

    if (ChildLayer != nullptr) {
        for (i = 0; i < NumberOfNodes; i++) {
            for (j = 0; j < NumberOfChildNodes; j++) {
                dw = LearningRate * ChildLayer->Errors[j] * NeuronValues[i];
                Weights[i][j] += dw + MomentumFactor * WeightChanges[i][j];
                WeightChanges[i][j] = dw;
            }
        }

        for (j = 0; j < NumberOfChildNodes; j++) {
            BiasWeights[j] += LearningRate * ChildLayer->Errors[j] * BiasValues[j];
        }
    }
}

void NeuralNetworkLayer::calculateNeuronValues() {
    int i, j;
    double x;

    if (ParentLayer != nullptr) {
        for (j = 0; j < NumberOfNodes; j++) {
            x = 0;
            for (i = 0; i < NumberOfParentNodes; i++) {
                x += ParentLayer->NeuronValues[i] * ParentLayer->Weights[i][j];
            }
            x += ParentLayer->BiasValues[j] * ParentLayer->BiasWeights[j];

            if ((ChildLayer == nullptr) && LinearOutput)
                NeuronValues[j] = x;
            else
                NeuronValues[j] = 1.0f / (1 + exp(-x));
        }
    }
}

