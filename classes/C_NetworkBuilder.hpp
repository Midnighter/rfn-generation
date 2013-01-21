/*******************************************************************************
 * filename:    C_NetworkBuilder.hpp
 * description: Class that chooses the right components to build a specific
 *              network type
 * author:      Moritz Beber
 * created:     2010-04-16
 * copyright:   Jacobs University Bremen. All rights reserved.
 ******************************************************************************/

#ifndef _C_NETWORKBUILDER_HPP
#define	_C_NETWORKBUILDER_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// project
#include "common_definitions.hpp"
#include "M_Singleton.hpp"
#include "C_ParameterManager.hpp"
#include "C_FlowDistributionNetwork.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace rfn {

    class NetworkBuilder {
        /* Singleton Pattern Declaration */
        DECLARE_BASE_SINGLETON(NetworkBuilder)

    public:
        /* Member Functions */
        virtual FlowDistributionNetwork* build(ParameterManager* const params,
            std::string name);
            
    }; // class NetworkBuilder

} // namespace rfn


#endif	// _C_NETWORKBUILDER_HPP

