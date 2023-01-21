#include "Guesser.h"
//#include <glad/glad.h>

int main()
{
    mat dataset;
    Guesser guess;

    data::Load("mnist_single.csv", dataset, true);
    dataset = dataset / 256.0; //Normalize data

    guess.predict(dataset);
}