#pragma once
#define MLPACK_ENABLE_ANN_SERIALIZATION
#include <mlpack/core.hpp>
#include <mlpack/methods/ann/ffn.hpp>

using namespace mlpack;
using namespace arma;

class Guesser
{

	public:
		Guesser() {}
		~Guesser(){}
		void predict(mat dataset);
	private:
		Row<size_t> getLabels(const mat& predOut);
};

