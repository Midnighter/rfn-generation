/*******************************************************************************
 * filename:    C_RandomPatternInitialiser.cpp
 * description: Implementation of 'RandomPatternInitialiser' class
 * author:      Moritz Beber
 * created:     2010-06-10
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


/*******************************************************************************
 * Includes
 ******************************************************************************/


// std
#include <cstring>
#include <set>
#include <iostream>
#include <cstdlib>

// boost
#include <boost/random/uniform_smallint.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

// project
#include "C_RandomPatternInitialiser.hpp"


/*******************************************************************************
 * Constructors & Destructor
 ******************************************************************************/


/*
 * Default constructor
 */
rfn::RandomPatternInitialiser::RandomPatternInitialiser():
    rfn::PatternInitialiser()
{}

/*
 * Destructor
 */
rfn::RandomPatternInitialiser::~RandomPatternInitialiser()
{}


/*******************************************************************************
 * Member Functions
 ******************************************************************************/


/*
 * Randomly intialise the given pattern using 'k' entries.
 */
void
rfn::RandomPatternInitialiser::init(rfn::ParameterManager* const params,
    rfn::Matrix* const pattern)
{
    // initialise
    gsl_matrix_set_zero(pattern);
    // tie rng for selecting nodes
    boost::uniform_smallint<short unsigned> us_dist(0,
        params->nodes_out() - 1);
    boost::variate_generator<rfn::RandGen&, boost::uniform_smallint
        <short unsigned> > node_gen(params->rng(), us_dist);
    // tie rng for pattern values
    boost::uniform_real<double> val_dist;
    boost::variate_generator<rfn::RandGen&, boost::uniform_real<double> >
        val_gen(params->rng(), val_dist);
    // other variables
    // 'values' holds pattern fractions
    double* values = new double[params->activated_k()];
    std::memset(values, 0, sizeof(double) * params->activated_k());
    // 'nodes' holds randomly chosen output nodes
    std::set<short unsigned> nodes;
    short unsigned i = 0;
    short unsigned j = 0;
    double sum = 0.0;
    // 'status' checks gsl return values
    std::set<short unsigned>::const_iterator set_it;
    for (j = 0; j < params->nodes_in(); ++j) {
        sum = 0.0;
        for (i = 0; i < params->activated_k(); ++i) {
            values[i] = val_gen();
            sum += values[i];

        }
        while (nodes.size() < params->activated_k()) {
            nodes.insert(node_gen());
        }
        for (set_it = nodes.begin(), i = 0; set_it != nodes.end(),
            i < params->activated_k(); ++set_it, ++i)
        {
            gsl_matrix_set(pattern, *set_it, j, values[i] / sum);
        }
        nodes.clear();
    }
    delete[] values;
}