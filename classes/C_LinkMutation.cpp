/*******************************************************************************
 * filename:    C_LinkMutation.cpp
 * description: Implementation of 'LinkMutation' class
 * author:      Moritz Beber
 * created:     2010-04-16
 * copyright:   Jacobs University Bremen. All rights reserved.
 ******************************************************************************/


/*******************************************************************************
 * Includes
 ******************************************************************************/


// boost
#include <boost/random/bernoulli_distribution.hpp>
#include <boost/random/uniform_smallint.hpp>
#include <boost/random/variate_generator.hpp>

// project
#include "C_LinkMutation.hpp"


/*******************************************************************************
 * Constructors & Destructor
 ******************************************************************************/


/*
 * Default constructor
 */
rfn::LinkMutation::LinkMutation():
    rfn::Mutation(),
        _b_dist(NULL),
        _bool_gen(NULL),
        _node_dist(NULL),
        _node_gen(NULL)
{
    // tie the rng for decision of adding or removing the link
    this->_b_dist = new Chance(rfn::Mutation::_parameters->add_mutation());
    CHECK_CRITICAL_POINTER(this->_b_dist);
    this->_bool_gen = new Yes(rfn::Mutation::_parameters->rng(), *this->_b_dist);
    CHECK_CRITICAL_POINTER(this->_bool_gen);
    // tie rng for drawing node numbers
    this->_node_dist = new Small(0,
        rfn::Mutation::_parameters->nodes_total() - 1);
    CHECK_CRITICAL_POINTER(this->_node_dist);
    this->_node_gen = new Small_Number(rfn::Mutation::_parameters->rng(),
        *this->_node_dist);
    CHECK_CRITICAL_POINTER(this->_node_gen);
}

/*
 * Destructor
 */
rfn::LinkMutation::~LinkMutation()
{
    delete this->_b_dist;
    this->_b_dist = NULL;
    delete this->_bool_gen;
    this->_bool_gen = NULL;
    delete this->_node_dist;
    this->_node_dist = NULL;
    delete this->_node_gen;
    this->_node_gen = NULL;
}


/*******************************************************************************
 * Member Functions
 ******************************************************************************/


/*
 * Modify the 'network' by adding or removing a single link.
 */
void
rfn::LinkMutation::mutate(rfn::Adjacency* const network)
{
    short unsigned src = 0;
    do {
        src = (*this->_node_gen)();
    } while (rfn::Mutation::_parameters->is_output(src));
    short unsigned tar = 0;
    do {
        tar = (*this->_node_gen)();
    } while (!rfn::Mutation::_parameters->regards_structure(src, tar));
    if ((*this->_bool_gen)()) {
        // add potentially new link
        boost::add_edge(src, tar, *network);
    }
    else {
        // remove potentially existing link
        boost::remove_edge(src, tar, *network);
    }
}

