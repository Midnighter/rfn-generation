/*******************************************************************************
 * filename:    C_NetworkInitialiser.cpp
 * description: Implementation of 'PatternInitialiser' class
 * author:      Moritz Beber
 * created:     2010-06-14
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


/*******************************************************************************
 * Includes
 ******************************************************************************/


// std
#include <cstdlib>

// project
#include "C_NetworkInitialiser.hpp"
#include "C_RandomNetworkInitialiser.hpp"
#include "C_EdgeListNetworkInitialiser.hpp"


/*******************************************************************************
 * Necessary Singleton Pattern Definitions
 ******************************************************************************/


DEFINE_BASE_SINGLETON(rfn::NetworkInitialiser)


/*******************************************************************************
 * Constructor & Destructor
 ******************************************************************************/


/*
 * Default constructor
 */
rfn::NetworkInitialiser::NetworkInitialiser()
{}

/*
 * Destructor
 */
rfn::NetworkInitialiser::~NetworkInitialiser()
{}


/*******************************************************************************
 * Member Functions
 ******************************************************************************/


/*
 * Default instance generation for singleton
 */
rfn::NetworkInitialiser*
rfn::NetworkInitialiser::instance()
{
    if (rfn::NetworkInitialiser::_instance == NULL) {
        std::cerr << "Error: Instance not generated yet. Returning NULL pointer!"
            << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return rfn::NetworkInitialiser::_instance;
}

/*
 * Instance generation for singleton
 */
rfn::NetworkInitialiser*
rfn::NetworkInitialiser::instance(const rfn::NetworkInitScheme scheme)
{
    if (rfn::NetworkInitialiser::_instance == NULL) {
        static rfn::NetworkInitialiser::Guard g;
        switch (scheme) {
            case rfn::random:
                rfn::NetworkInitialiser::_instance = new
                    rfn::RandomNetworkInitialiser;
                break;
            case rfn::list:
                rfn::NetworkInitialiser::_instance = new
                    rfn::EdgeListNetworkInitialiser;
                break;
        }

    }
    return rfn::NetworkInitialiser::_instance;
}