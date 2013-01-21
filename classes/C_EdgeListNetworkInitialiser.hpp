/*******************************************************************************
 * filename:    C_EdgeListNetworkInitialiser.hpp
 * description: Derived class for pre-determined network initialisation
 * author:      Moritz Beber
 * created:     2011-01-24
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


#ifndef _C_EDGELISTNETWORKINITIALISER_HPP
#define	_C_EDGELISTNETWORKINITIALISER_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// project
#include "C_NetworkInitialiser.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace rfn {

    class EdgeListNetworkInitialiser: public NetworkInitialiser {
        /* Derived From Singleton Pattern */
        DECLARE_DERIVED_SINGLETON(NetworkInitialiser, EdgeListNetworkInitialiser)

    public:
        /* Member Functions */
        // network init method
        virtual void init(ParameterManager* const parameters,
            Adjacency* const network);

    }; // class EdgeListNetworkInitialiser

} // namespace rfn


#endif	// _C_EDGELISTNETWORKINITIALISER_HPP

