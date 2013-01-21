/*******************************************************************************
 * filename:    C_EdgeListNetworkInitialiser.cpp
 * description: Implementation of 'EdgeListNetworkInitialiser' class
 * author:      Moritz Beber
 * created:     2011-01-24
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


/*******************************************************************************
 * Includes
 ******************************************************************************/


// boost
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>

// project
#include "C_EdgeListNetworkInitialiser.hpp"


/*******************************************************************************
 * Constructors & Destructor
 ******************************************************************************/


/*
 * Default constructor
 */
rfn::EdgeListNetworkInitialiser::EdgeListNetworkInitialiser():
    rfn::NetworkInitialiser()
{}

/*
 * Destructor
 */
rfn::EdgeListNetworkInitialiser::~EdgeListNetworkInitialiser()
{}


/*******************************************************************************
 * Member Functions
 ******************************************************************************/


/*
 * Initialise a given network from a predetermined edge list
 */
void
rfn::EdgeListNetworkInitialiser::init(rfn::ParameterManager* const parameters,
    rfn::Adjacency* const network)
{
    bfsys::ifstream input("/home/mbeber/software/"
            "rfn5/half_density_topology.tsv");
    std::string line;
    std::vector<std::string> split_vec;
    split_vec.reserve(2u);
    unsigned src = 0u;
    unsigned tar = 0u;
    while (std::getline(input, line)) {
        boost::trim(line);
        if (boost::istarts_with(line, "#") || line == "") {
            continue;
        }
        boost::split(split_vec, line, boost::is_any_of(" \t,"));
        if (split_vec.size() >= 2) {
            try {
                src = boost::lexical_cast<unsigned>(split_vec[0]);
                tar = boost::lexical_cast<unsigned>(split_vec[1]);
            }
            catch (boost::bad_lexical_cast& err) {
                std::cerr << err.what() << std::endl;
            }
            boost::add_edge(src, tar, *network);
        }
        else {
            throw "Bad line format!";
        }
        split_vec.clear();
    }
}
