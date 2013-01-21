/*******************************************************************************
 * filename:    C_PathMutation.cpp
 * description: Implementation of 'PathMutation' class
 * author:      Moritz Beber
 * created:     2010-04-16
 * copyright:   Jacobs University Bremen. All rights reserved.
 ******************************************************************************/


/*******************************************************************************
 * Includes
 ******************************************************************************/


// project
#include "C_PathMutation.hpp"


/*******************************************************************************
 * Constructors & Destructor
 ******************************************************************************/


/*
 * Default constructor
 */
rfn::PathMutation::PathMutation():
    rfn::Mutation(),
    _b_dist(NULL),
    _bool_gen(NULL),
    _in_dist(NULL),
    _in_gen(NULL),
    _mid_dist(NULL),
    _mid_gen(NULL),
    _out_dist(NULL),
    _out_gen(NULL),
    _us_dist(NULL),
    _len_gen(NULL)
{
    // tie the rng for decision of adding or removing the path
    this->_b_dist = new Chance(rfn::Mutation::_parameters->add_mutation());
    CHECK_CRITICAL_POINTER(this->_b_dist);
    this->_bool_gen = new Yes(rfn::Mutation::_parameters->rng(), *this->_b_dist);
    CHECK_CRITICAL_POINTER(this->_bool_gen);
    // tie the rngs for drawing node numbers to build a path
    // starting node
    this->_in_dist = new Small(0, rfn::Mutation::_parameters->nodes_in() - 1);
    CHECK_CRITICAL_POINTER(this->_in_dist);
    this->_in_gen = new Small_Number(rfn::Mutation::_parameters->rng(),
        *this->_in_dist);
    CHECK_CRITICAL_POINTER(this->_in_gen);
    // middle nodes
    this->_mid_dist = new Small(rfn::Mutation::_parameters->nodes_in(),
        rfn::Mutation::_parameters->nodes_end_middle() - 1);
    CHECK_CRITICAL_POINTER(this->_mid_dist);
    this->_mid_gen = new Small_Number(rfn::Mutation::_parameters->rng(),
        *this->_mid_dist);
    CHECK_CRITICAL_POINTER(this->_mid_gen);
    // end node
    this->_out_dist = new Small(rfn::Mutation::_parameters->nodes_end_middle(),
        rfn::Mutation::_parameters->nodes_total() - 1);
    CHECK_CRITICAL_POINTER(this->_out_dist);
    this->_out_gen = new Small_Number(rfn::Mutation::_parameters->rng(),
        *this->_out_dist);
    CHECK_CRITICAL_POINTER(this->_out_gen);
    // number of middle nodes the path should contain
    this->_us_dist = new Small(1, rfn::Mutation::_parameters->nodes_middle());
    CHECK_CRITICAL_POINTER(this->_us_dist);
    this->_len_gen = new Small_Number(rfn::Mutation::_parameters->rng(),
        *this->_us_dist);
    CHECK_CRITICAL_POINTER(this->_len_gen);
}

/*
 * Destructor
 */
rfn::PathMutation::~PathMutation()
{
    delete this->_b_dist;
    this->_b_dist = NULL;
    delete this->_bool_gen;
    this->_bool_gen = NULL;
    delete this->_in_dist;
    this->_in_dist = NULL;
    delete this->_in_gen;
    this->_in_gen = NULL;
    delete this->_mid_dist;
    this->_mid_dist = NULL;
    delete this->_mid_gen;
    this->_mid_gen = NULL;
    delete this->_out_dist;
    this->_out_dist = NULL;
    delete this->_out_gen;
    this->_out_gen = NULL;
    delete this->_us_dist;
    this->_us_dist = NULL;
    delete this->_len_gen;
    this->_len_gen = NULL;
}


/*******************************************************************************
 * Member Functions
 ******************************************************************************/


/*
 * Modify the 'network' by adding or removing a path from an input node, via a
 * random number of middle nodes, to an output node.
 */
void
rfn::PathMutation::mutate(rfn::Adjacency* const network)
{
    
    // construct a path
    short unsigned length = (*this->_len_gen)();
    short unsigned end = (*this->_out_gen)();
    short unsigned src = (*this->_in_gen)();
    short unsigned tar = 0;
    if ((*this->_bool_gen)()) {
        // we add a path
        for (short unsigned i = 0; i < length; ++i) {
            // draw middle nodes
            do {
                tar = (*this->_mid_gen)();
            } while (src == tar);
            boost::add_edge(src, tar, *network);
            src = tar;
        }
        boost::add_edge(src, end, *network);
    }
    else {
        // we remove a path
        for (short unsigned i = 0; i < length; ++i) {
            // draw middle nodes
            do {
                tar = (*this->_mid_gen)();
            } while (src == tar);
            boost::remove_edge(src, tar, *network);
            src = tar;
        }
        boost::remove_edge(src, end, *network);
    }
}

