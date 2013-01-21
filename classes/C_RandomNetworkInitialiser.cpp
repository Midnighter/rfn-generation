/*******************************************************************************
 * filename:    C_RandomNetworkInitialiser.cpp
 * description: Implementation of 'RandomNetworkInitialiser' class
 * author:      Moritz Beber
 * created:     2010-06-14
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


/*******************************************************************************
 * Includes
 ******************************************************************************/


// boost
#include <boost/random/bernoulli_distribution.hpp>
#include <boost/random/variate_generator.hpp>

// project
#include "C_RandomNetworkInitialiser.hpp"


/*******************************************************************************
 * Constructors & Destructor
 ******************************************************************************/


/*
 * Default constructor
 */
rfn::RandomNetworkInitialiser::RandomNetworkInitialiser():
    rfn::NetworkInitialiser()
{}

/*
 * Destructor
 */
rfn::RandomNetworkInitialiser::~RandomNetworkInitialiser()
{}


/*******************************************************************************
 * Member Functions
 ******************************************************************************/


/*
 * Randomly intialise the given network
 */
void
rfn::RandomNetworkInitialiser::init(rfn::ParameterManager* const parameters,
    rfn::Adjacency* const network)
{
    boost::bernoulli_distribution<double> b_dist(parameters->connectivity());
    boost::variate_generator<rfn::RandGen&,
        boost::bernoulli_distribution<double> >
        bool_gen(parameters->rng(), b_dist);
    for (short unsigned i = 0; i < parameters->nodes_end_middle(); ++i) {
        // only input and middle nodes can have outgoing links
        for (short unsigned j = parameters->nodes_in();
            j < parameters->nodes_total(); ++j)
        {
            // only middle and output nodes have incoming links
            if (parameters->regards_structure(i, j)) {
                if (bool_gen()) {
                    boost::add_edge(i, j, *network);
                }
            }
        }
    }
}
