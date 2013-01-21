/*******************************************************************************
 * filename:    C_LinkRobustNetwork.hpp
 * description: Derived class for link robust networks
 * author:      Moritz Beber
 * created:     2010-07-20
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


#ifndef _C_LINKROBUSTNETWORK_HPP
#define	_C_LINKROBUSTNETWORK_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// project
#include "C_RobustFunctionalNetwork.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace rfn {

    class LinkRobustNetwork: public RobustFunctionalNetwork {

    public:
        /* Constructors & Destructor */
        LinkRobustNetwork();
        LinkRobustNetwork(const LinkRobustNetwork& c);
        virtual ~LinkRobustNetwork();
        // polymorphic "copy constructor"
        virtual FlowDistributionNetwork* clone()
        {
            return new LinkRobustNetwork(*this);
        }

        /* Operators */
        virtual FlowDistributionNetwork& operator=(const FlowDistributionNetwork& a);
        virtual FlowDistributionNetwork& operator=(FlowDistributionNetwork& a);
        LinkRobustNetwork& operator=(const LinkRobustNetwork& a);
        LinkRobustNetwork& operator=(LinkRobustNetwork& a);

    protected:
        /* Internal Functions */
        virtual void compute_robustness();

    }; // class LinkRobustNetwork

} // namespace rfn


#endif	// _C_LINKROBUSTNETWORK_HPP

