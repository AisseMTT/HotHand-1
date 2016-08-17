#include "MyModel.h"
#include "DNest4/code/DNest4.h"
#include "Data.h"
#include <sstream>
#include <iomanip>

namespace HotHand
{

MyModel::MyModel()
:n(Data::instance.get_shots().size())
,prob(Data::instance.get_shots().size())
{

}

void MyModel::from_prior(DNest4::RNG& rng)
{
    mu = 5*rng.randn();

    DNest4::Cauchy c(0.0, 5.0);
    L = exp(c.generate(rng));
    sigma = exp(log(1E-3) + log(1E5)*rng.rand());

    for(auto& _n: n)
        _n = rng.randn();

    assemble();
}

void MyModel::assemble()
{
    double alpha = exp(-1.0/L);
    double beta = sigma*sqrt(1.0 - alpha*alpha);

    // Mean zero
    prob[0] = sigma*n[0];
    for(size_t i=1; i<prob.size(); ++i)
        prob[i] = alpha*prob[i-1] + beta*n[i];

    // Add mu
    for(auto& _prob: prob)
        _prob += mu;

    // Convert from logit to probability
    for(auto& _prob: prob)
        _prob = exp(_prob)/(exp(_prob) + 1.0);
}

double MyModel::perturb(DNest4::RNG& rng)
{
	double logH = 0.0;

    int option = rng.rand_int(2);
    if(option == 0)
    {
        // Change a hyperparameter
        int which = rng.rand_int(3);
        if(which == 0)
        {
            logH -= -0.5*pow(mu/5, 2);
            mu += 5*rng.randh();
            logH += -0.5*pow(mu/5, 2);
        }
        else if(which == 1)
        {
            DNest4::Cauchy c(0.0, 5.0);
            L = log(L);
            logH += c.perturb(L, rng);
            L = exp(L);
        }
        else
        {
            sigma = log(sigma);
            sigma += log(1E5)*rng.randh();
            DNest4::wrap(sigma, log(1E-3), log(100.0));
            sigma = exp(sigma);
        }
    }
    else
    {
        option = rng.rand_int(2);
        if(option == 0)
        {
            int which = rng.rand_int(n.size());
            logH -= -0.5*pow(n[which], 2);
            n[which] += rng.randh();
            logH += -0.5*pow(n[which], 2);
        }
        else
        {
            int reps = static_cast<int>(pow(10.0, 2*rng.rand()));
            for(int i=0; i<reps; ++i)
            {
                int which = rng.rand_int(n.size());
                n[which] = rng.randn();
            }
        }
    }

    assemble();

	return logH;
}

double MyModel::log_likelihood() const
{
	double logL = 0.0;

    // Alias for data
    const auto& shots = Data::instance.get_shots();
    for(size_t i=0; i<shots.size(); ++i)
    {
        if(shots[i])
            logL += log(prob[i]);
        else
            logL += log(1.0 - prob[i]);

        if(std::isnan(logL) || std::isinf(logL))
            return -1E300;
    }

	return logL;
}

void MyModel::print(std::ostream& out) const
{
    out<<std::setprecision(12);
    out<<mu<<' '<<L<<' '<<sigma;
    for(auto p: prob)
        out<<' '<<p;
}

std::string MyModel::description() const
{
    std::stringstream s;
    s<<"mu, L, sigma";
    for(size_t i=0; i<prob.size(); ++i)
        s<<", prob["<<i<<"]";
	return s.str();
}

} // namespace HotHand

