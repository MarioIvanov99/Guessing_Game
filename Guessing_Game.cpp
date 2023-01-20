#define MLPACK_ENABLE_ANN_SERIALIZATION
#include <mlpack/core.hpp>
#include <mlpack/methods/ann/ffn.hpp>

using namespace mlpack;
using namespace arma;
using namespace std;

Row<size_t> getLabels(const mat& predOut)
{
    Row<size_t> predLabels(predOut.n_cols);
    for (uword i = 0; i < predOut.n_cols; ++i)
    {
        predLabels(i) = predOut.col(i).index_max();
    }
    return predLabels;
}

int main()
{
    FFN<NegativeLogLikelihood, RandomInitialization> model;
    //Load trained model using mlpack's example cnn
    //https://github.com/mlpack/examples
    data::Load("model.bin", "cnn", model);
    mat predOut;
    mat dataset;
    Row<size_t> predLabels = getLabels(predOut);

    cout << "Predicting on test set..." << endl;

    data::Load("mnist_single.csv", dataset, true);
    dataset = dataset / 256.0; //Normalize data

    model.Predict(dataset, predOut); //Get prediction from one image
    predLabels = getLabels(predOut);

    for (int i = 0; i < predLabels.n_elem; i++) {
        cout << "Label [" << i << "]: " << predLabels[i] << endl;
    }


}