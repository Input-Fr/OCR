#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double sigmoid_derivative(double x) {
    return x * (1.0 - x);
}

typedef struct
{
    double* weightsInputHidden;
    double* weightsHiddenOutput;
    double* biasesHidden;
    double* biasesOutput;
    double* hiddenLayerValues;
    double* outputLayerValues;
    int* sizes;

}Network;

double randomNormal()
{
    return ((double)rand() / RAND_MAX) * 2.0 - 1.0;
}

void initNetwork(Network* network, int* sizes)
{
    network->sizes = sizes;
    network->hiddenLayerValues = calloc(network->sizes[1],sizeof(double));
    network->outputLayerValues = calloc(network->sizes[2],sizeof(double));
    network->weightsInputHidden = malloc(sizes[0]*sizes[1]*sizeof(double));
    network->weightsHiddenOutput = malloc(sizes[1]*sizes[2]*sizeof(double));
    network->biasesHidden = malloc(network->sizes[1]*sizeof(double));
    network->biasesOutput = malloc(network->sizes[2]*sizeof(double));

    for (int i = 0; i < network->sizes[0]*network->sizes[1]; i++) {
        network->weightsInputHidden[i] = randomNormal();
    }
    for (int i = 0; i < network->sizes[1]*network->sizes[2]; i++) {
        network->weightsHiddenOutput[i] = randomNormal();
    }

    for (int i = 0; i < network->sizes[1]; i++) {
        network->biasesHidden[i] = randomNormal();
    }

    for (int i = 0; i < network->sizes[2]; i++) {
        network->biasesOutput[i] = randomNormal();
    }
}

void destroyNetwork(Network* network)
{
    free(network->weightsInputHidden);
    free(network->weightsHiddenOutput);
    free(network->hiddenLayerValues);
    free(network->outputLayerValues);
    free(network);
}

void feedForward(Network* network, double input[network->sizes[0]])
{
    int * sizes = network->sizes;
    for(int i = 0; i < network->sizes[1];i++)
    {
        network->hiddenLayerValues[i] = 0;
        for(int j = 0; j < network->sizes[0];j++)
        {
            network->hiddenLayerValues[i] += input[j]*network->weightsInputHidden[j*sizes[1]+i];
        }
        double bias = network->biasesHidden[i];
        network->hiddenLayerValues[i] = sigmoid(network->hiddenLayerValues[i]+bias);
    }

    for(int i = 0; i < network->sizes[2];i++)
    {
        network->outputLayerValues[i] = 0;
        for(int j = 0; j < network->sizes[1];j++)
        {
            network->outputLayerValues[i] += network->hiddenLayerValues[j]*network->weightsHiddenOutput[j*sizes[2]+i];
        }
        double bias = network->biasesOutput[i];
        network->outputLayerValues[i] = sigmoid(network->outputLayerValues[i]+bias);
    }

}

void backPropagation(Network* network, double err, double learningRate, double * input)
{

    int *sizes = network->sizes;

    double deltaOutput[sizes[2]]; // inutile pour la fonction XOR mais à garder pour les réseaux plus complexes
    for (int i = 0; i < sizes[2]; ++i) {
        deltaOutput[i] = err * sigmoid_derivative(network->outputLayerValues[i]); // on calcul d'importance du neurone dans l'erreur d'output
    }

    // deltas pour le layer du milieu
    double deltaHidden[sizes[1]];

    // mise a jour des weight/biais du layer du milieu à l'output et calcul des deltas pour le layer du milieu
    for (int i = 0; i < sizes[1]; i++) {
        deltaHidden[i] = 0;
        for (int j = 0; j < sizes[2]; j++) {
            network->weightsHiddenOutput[i * sizes[2] + j] += learningRate * deltaOutput[j] * network->hiddenLayerValues[i];
            deltaHidden[i] += deltaOutput[j] * network->weightsHiddenOutput[i * sizes[2] + j];
            network->biasesOutput[j] += learningRate * deltaOutput[j];
        }
        deltaHidden[i] *= sigmoid_derivative(network->hiddenLayerValues[i]);
    }


    // mise a jour des weights/biais de l'input au layer du milieu
    for (int i = 0; i < sizes[0]; i++) {
        for (int j = 0; j < sizes[1]; j++) {
            network->weightsInputHidden[i * sizes[1] + j] += learningRate * deltaHidden[j] * input[i];
            network->biasesHidden[j] += learningRate * deltaHidden[j];
        }
    }


}

void trainNetwork(Network* network,double learningRate,int epochs)
{
    // modèle d'entrainement pour la fonction XOR
    double allInput[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    double wantedOutput [4] = {0, 1, 1, 0};

    for (int e = 0; e < epochs; e++)
    {
        for (int i = 0; i < 4; i++) // on teste sur tout les input/output
        {
            feedForward(network, allInput[i]);

            double err = wantedOutput[i] - network->outputLayerValues[0];
            backPropagation(network,err,learningRate,allInput[i]);
        }
    }
}

void testNetwork(Network * network)
{
    // modèle d'entrainement pour la fonction XOR
    double allInput[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};

    for (int i = 0; i < 4; i++) {
        feedForward(network, allInput[i]);

        printf("Input: %f %f, Output: %f\n", allInput[i][0], allInput[i][1], network->outputLayerValues[0]);
    }
}

int main()
{


    int sizes[] = {2,10,1};

    double learningRate = 0.1;

    int epochs = 20000;

    Network* network = malloc(sizeof(Network));
    initNetwork(network,sizes);
    printf("-------------------- AVANT ENTRAINEMENT --------------------\n");

    testNetwork(network);

    trainNetwork(network,learningRate,epochs);

    printf("-------------------- APRES ENTRAINEMENT --------------------\n");

    testNetwork(network);

    destroyNetwork(network);
    return 0;
}

