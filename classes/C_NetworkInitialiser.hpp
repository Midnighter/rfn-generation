/*******************************************************************************
 * filename:    C_NetworkInitialiser.hpp
 * description: Singleton pure virtual class for initialising a network
 * author:      Moritz Beber
 * created:     2010-06-14
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


#ifndef _C_NETWORKINITIALISER_HPP
#define	_C_NETWORKINITIALISER_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// project
#include "common_definitions.hpp"
#include "M_Singleton.hpp"
#include "C_ParameterManager.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace rfn {

    class ParameterManager;

    class NetworkInitialiser {
        /* Singleton Pattern Declaration */
        DECLARE_BASE_SINGLETON(NetworkInitialiser)

    public:
        /* Member Functions */
        // _instance retrieval method
        static NetworkInitialiser* instance(const NetworkInitScheme scheme);
        // pure virtual network init method
        virtual void init(ParameterManager* const parameters,
            Adjacency* const network) = 0;

    }; // class NetworkInitialiser

} // namespace rfn


#endif	// _C_NETWORKINITIALISER_HPP

