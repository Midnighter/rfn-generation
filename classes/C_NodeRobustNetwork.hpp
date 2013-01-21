/*******************************************************************************
 * filename:    C_NodeRobustNetwork.hpp
 * description: Derived class for node robust networks
 * author:      Moritz Beber
 * created:     2010-07-05
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


#ifndef _C_NODEROBUSTNETWORK_HPP
#define	_C_NODEROBUSTNETWORK_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// project
#include "C_RobustFunctionalNetwork.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace rfn {

    class NodeRobustNetwork: public RobustFunctionalNetwork {

    public:
        /* Constructors & Destructor */
        NodeRobustNetwork();
        NodeRobustNetwork(const NodeRobustNetwork& c);
        virtual ~NodeRobustNetwork();
        // polymorphic "copy constructor"
        virtual FlowDistributionNetwork* clone()
        {
            return new NodeRobustNetwork(*this);
        }

        /* Operators */
        virtual FlowDistributionNetwork& operator=(const FlowDistributionNetwork& a);
        virtual FlowDistributionNetwork& operator=(FlowDistributionNetwork& a);
        NodeRobustNetwork& operator=(const NodeRobustNetwork& a);
        NodeRobustNetwork& operator=(NodeRobustNetwork& a);

    protected:
        /* Internal Functions */
        virtual void compute_robustness();

    }; // class NodeRobustNetwork

} // namespace rfn


#endif	// _C_NODEROBUSTNETWORK_HPP

