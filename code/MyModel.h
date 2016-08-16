#ifndef HotHand_MyModel
#define HotHand_MyModel

#include "DNest4/code/DNest4.h"
#include <ostream>

namespace HotHand
{

class MyModel
{
	private:
        // Hyperparameters of AR(1) process
        double mu, L, sigma;

        // Latent parameters with N(0, 1) priors
        std::vector<double> n;

        // Latent time series of probabilities
        std::vector<double> prob;

        void assemble();

	public:
		// Constructor only gives size of params
		MyModel();

		// Generate the point from the prior
		void from_prior(DNest4::RNG& rng);

		// Metropolis-Hastings proposals
		double perturb(DNest4::RNG& rng);

		// Likelihood function
		double log_likelihood() const;

		// Print to stream
		void print(std::ostream& out) const;

		// Return string with column information
		std::string description() const;
};

} // namespace HotHand

#endif

