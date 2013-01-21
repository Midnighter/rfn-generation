/*******************************************************************************
 * filename:    C_RandomNetworkInitialiser.hpp
 * description: Derived class for randomly initialising a network
 * author:      Moritz Beber
 * created:     2010-06-14
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


#ifndef _C_RANDOMNETWORKINITIALISER_HPP
#define	_C_RANDOMNETWORKINITIALISER_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// project
#include "C_NetworkInitialiser.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace rfn {

    class RandomNetworkInitialiser: public NetworkInitialiser {
        /* Derived From Singleton Pattern */
        DECLARE_DERIVED_SINGLETON(NetworkInitialiser, RandomNetworkInitialiser)

    public:
        /* Member Functions */
        // network init method
        virtual void init(ParameterManager* const parameters,
            Adjacency* const network);

    }; // class RandomNetworkInitialiser

} // namespace rfn


#endif	// _C_RANDOMNETWORKINITIALISER_HPP

