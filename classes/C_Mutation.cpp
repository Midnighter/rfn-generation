/*******************************************************************************
 * filename:    C_Mutation.cpp
 * description: Implementation of 'Mutation' class
 * author:      Moritz Beber
 * created:     2010-04-16
 * copyright:   Jacobs University Bremen. All rights reserved.
 ******************************************************************************/


/*******************************************************************************
 * Includes
 ******************************************************************************/


// std
#include <iostream>
#include <cstdlib>

// project
#include "C_Mutation.hpp"
#include "C_PathMutation.hpp"
#include "C_LinkMutation.hpp"


/*******************************************************************************
 * Necessary Singleton Pattern Definitions
 ******************************************************************************/


DEFINE_BASE_SINGLETON(rfn::Mutation)


/*******************************************************************************
 * Constructors & Destructor
 ******************************************************************************/


/*
 * Default constructor
 */
rfn::Mutation::Mutation():
    _parameters(rfn::ParameterManager::instance())
{}

/*
 * Destructor
 */
rfn::Mutation::~Mutation()
{
    this->_parameters = NULL; // singleton destroys itself
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/


/*
 * Default instance generation for singleton
 */
rfn::Mutation*
rfn::Mutation::instance()
{
    if (rfn::Mutation::_instance == NULL) {
        std::cerr << "Warning: Instance not generated yet." << std::endl;
    }
    return rfn::Mutation::_instance;
}

/*
 * Instance generation for singleton
 */
rfn::Mutation*
rfn::Mutation::instance(const rfn::MutationScheme scheme)
{
    if (rfn::Mutation::_instance == NULL) {
        static rfn::Mutation::Guard g;
        switch (scheme) {
            case rfn::path_mutation:
                rfn::Mutation::_instance = new rfn::PathMutation;
                break;
            case rfn::link_mutation:
                rfn::Mutation::_instance = new rfn::LinkMutation;
                break;
        }
    }
    return rfn::Mutation::_instance;
}
