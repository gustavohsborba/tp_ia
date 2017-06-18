#ifndef TP_FINAL_NEURALNETWORKLAYER_H
#define TP_FINAL_NEURALNETWORKLAYER_H


class NeuralNetworkLayer {
public:
    int NumberOfNodes;
    int NumberOfChildNodes;
    int NumberOfParentNodes;
    double **Weights;
    double **WeightChanges;
    double *NeuronValues;
    double *DesiredValues;
    double *Errors;
    double *BiasWeights;
    double *BiasValues;
    double LearningRate;

    bool LinearOutput;
    bool UseMomentum;
    double MomentumFactor;

    NeuralNetworkLayer *ParentLayer;
    NeuralNetworkLayer *ChildLayer;

    NeuralNetworkLayer();
    void initialize(int NumNodes, NeuralNetworkLayer *parent, NeuralNetworkLayer *child);
    void cleanUp();
    void randomizeWeights();
    void calculateErrors();
    void adjustWeights();
    void calculateNeuronValues();

};


#endif
