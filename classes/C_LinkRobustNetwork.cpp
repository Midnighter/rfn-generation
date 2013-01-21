/*******************************************************************************
 * filename:    C_LinkRobustNetwork.cpp
 * description: Implementation of the 'LinkRobustNetwork' class
 * author:      Moritz Beber
 * created:     2010-07-20
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 * 
 ******************************************************************************/


/*******************************************************************************
 * Includes
 ******************************************************************************/


// gsl
#include <gsl/gsl_math.h>

// project
#include "C_LinkRobustNetwork.hpp"


/*******************************************************************************
 * Constructors & Destructor
 ******************************************************************************/


/*
 * Default constructor
 */
rfn::LinkRobustNetwork::LinkRobustNetwork():
    rfn::RobustFunctionalNetwork()
{}

/*
 * Copy constructor
 */
rfn::LinkRobustNetwork::LinkRobustNetwork(
    const rfn::LinkRobustNetwork& c):
    rfn::RobustFunctionalNetwork(c)
{}

/*
 * Destructor
 */
rfn::LinkRobustNetwork::~LinkRobustNetwork()
{}


/*******************************************************************************
 * Operators
 ******************************************************************************/


/*
 * Virtual const assignment operator, all networks following an initial one are
 * copy-constructed during the evolution thus here we only transfer the bare
 * minimum values needed.
 */
rfn::FlowDistributionNetwork&
rfn::LinkRobustNetwork::operator=(const rfn::FlowDistributionNetwork& a)
{
    if (this != &a) {
        // expect only networks of same type to be assigned
        assert(typeid(*this) == typeid(a));
        // call superclass assignment
        rfn::RobustFunctionalNetwork::operator=(a);
    }
    return *this;
}

/*
 * Virtual assignment operator, all networks following an initial one are copy-
 * constructed during the evolution thus here we only transfer the bare minimum
 * values needed.
 */
rfn::FlowDistributionNetwork&
rfn::LinkRobustNetwork::operator=(rfn::FlowDistributionNetwork& a)
{
    if (this != &a) {
        // expect only networks of same type to be assigned
        assert(typeid(*this) == typeid(a));
        // call superclass assignment
        rfn::RobustFunctionalNetwork::operator=(a);
    }
    return *this;
}

/*
 * Const assignment operator of this class calls the virtual operator so that
 * all values are properly assigned.
 */
rfn::LinkRobustNetwork&
rfn::LinkRobustNetwork::operator=(const rfn::LinkRobustNetwork& a)
{
    if (this != &a) {
        // expect only networks of same type to be assigned
        assert(typeid(*this) == typeid(a));
        // cast to virtual assignment operator type
        const FlowDistributionNetwork& other = static_cast
            <const FlowDistributionNetwork&>(a);
        // call the virtual assignment operator with its proper signature
        this->operator=(other);
    }
    return *this;
}

/*
 * Assignment operator of this class calls the virtual operator so that
 * all values are properly assigned.
 */
rfn::LinkRobustNetwork&
rfn::LinkRobustNetwork::operator=(rfn::LinkRobustNetwork& a)
{
    if (this != &a) {
        // expect only networks of same type to be assigned
        assert(typeid(*this) == typeid(a));
        // cast to virtual assignment operator type
        FlowDistributionNetwork& other = static_cast
            <FlowDistributionNetwork&>(a);
        // call the virtual assignment operator with its proper signature
        this->operator=(other);
    }
    return *this;
}


/*******************************************************************************
 * Internal Functions
 ******************************************************************************/


void
rfn::LinkRobustNetwork::compute_robustness()
{
    this->_robustness = 0.0;
    // make a colour map with all nodes set to white
    std::vector<boost::default_color_type> colours(this->num_nodes(),
        boost::color_traits<boost::default_color_type>::white());
    rfn::ColourMap colour_map(colours, *this->_network);
    // get all accessible nodes
    this->accessible_nodes(colour_map);
    short unsigned i = 0;
    short unsigned src = 0;
    short unsigned tar = 0;
    boost::graph_traits<Adjacency>::out_edge_iterator eit, eit_end;
    rfn::LinkRobustNetwork next(*this);
    // control input nodes
    for (short unsigned n = 0; n < this->_parameters->nodes_in(); ++n)
    {
        for (boost::tie(eit, eit_end) = boost::out_edges(n, *this->_network);
            eit != eit_end; ++eit)
        {
            // delete link
            src = boost::source(*eit, *next._network);
            tar = boost::target(*eit, *next._network);
            boost::remove_edge(src, tar, *next._network);
            next.compute_output();
            next.compute_flow_error();
            if (gsl_fcmp(next._flow_error,
                this->_robust_parameters->robust_threshold(),
                GSL_EPSILON) < 0)
            {
                this->_robustness += 1.0;
            }
            // reset the network
            boost::add_edge(src, tar, *next._network);
            ++i;
        }
    }
    // control middle nodes
    for (short unsigned n = this->_parameters->nodes_in();
        n < this->_parameters->nodes_end_middle(); ++n)
    {
// reversion to initial state
//        // only links from accessible nodes should be considered for robustness
//        if (boost::get(colour_map, n) != boost::white_color) {
            // disregard virtual sources and sinks in the middle layer
            if (boost::in_degree(n, *this->_network) >= 1
                    && boost::out_degree(n, *this->_network) >= 1)
            {
                for (boost::tie(eit, eit_end) =
                    boost::out_edges(n, *this->_network); eit != eit_end; ++eit)
                {
                    // delete link
                    src = boost::source(*eit, *next._network);
                    tar = boost::target(*eit, *next._network);
                    boost::remove_edge(src, tar, *next._network);
                    next.compute_output();
                    next.compute_flow_error();
                    if (gsl_fcmp(next._flow_error,
                        this->_robust_parameters->robust_threshold(),
                        GSL_EPSILON) < 0)
                    {
                        this->_robustness += 1.0;
                    }
                    // reset the network
                    boost::add_edge(src, tar, *next._network);
                    ++i;
                }
            }
//        }
    }
    // robustness is a fraction of all links applicable
    if (i > 0) {
        this->_robustness /= (double)i;
    }
}
