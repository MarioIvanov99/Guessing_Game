#include "Guesser.h"

Row<size_t> Guesser::getLabels(const mat& predOut)
{
    Row<size_t> predLabels(predOut.n_cols);
    for (uword i = 0; i < predOut.n_cols; ++i)
    {
        predLabels(i) = predOut.col(i).index_max();
    }
    return predLabels;
}

void Guesser::predict(mat dataset) {
    FFN<NegativeLogLikelihood, RandomInitialization> model;
    //Load trained model using mlpack's example cnn
    //https://github.com/mlpack/examples
    data::Load("model.bin", "cnn", model);
    mat predOut;

    Row<size_t> predLabels = getLabels(predOut);

    cout << "Predicting on test set..." << endl;

    model.Predict(dataset, predOut); //Get prediction from one image
    predLabels = getLabels(predOut);

    for (int i = 0; i < predLabels.n_elem; i++) {
        cout << "Label [" << i << "]: " << predLabels[i] << endl;
    }
}
