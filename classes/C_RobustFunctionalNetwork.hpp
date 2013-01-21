/*******************************************************************************
 * filename:    C_RobustFunctionalNetwork.hpp
 * description: Derived class for robust functional networks
 * author:      Moritz Beber
 * created:     2010-07-05
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


#ifndef _C_ROBUSTFUNCTIONALNETWORK_HPP
#define	_C_ROBUSTFUNCTIONALNETWORK_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// project
#include "C_FlowDistributionNetwork.hpp"
#include "C_RobustParameterManager.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace rfn {

    class RobustFunctionalNetwork: public FlowDistributionNetwork {

    protected:
        /* Data Members */
        bool _robust_changed;
        double _robustness;
        RobustParameterManager* _robust_parameters;

    public:
        /* Constructors & Destructor */
        RobustFunctionalNetwork();
        RobustFunctionalNetwork(const RobustFunctionalNetwork& c);
        virtual ~RobustFunctionalNetwork();
        // polymorphic "copy constructors"
        virtual FlowDistributionNetwork* clone() = 0;

        /* Operators */
        virtual FlowDistributionNetwork& operator=(const FlowDistributionNetwork& a);
        virtual FlowDistributionNetwork& operator=(FlowDistributionNetwork& a);
        RobustFunctionalNetwork& operator=(const RobustFunctionalNetwork& a);
        RobustFunctionalNetwork& operator=(RobustFunctionalNetwork& a);

        /* Member Functions */
        // save this object to a binary stream
        virtual void write_to_binary_stream(bfsys::ofstream& out,
            const unsigned long iteration);
        // read this object from a binary stream
        virtual unsigned long read_from_binary_stream(bfsys::ifstream& in);
        // change the network
        virtual void mutate()
        {
            this->_changed = true;
            this->_robust_changed = true;
            this->_mutation->mutate(this->_network);
        }
        // get and if necessary compute robustness
        double robustness();
        // removes unused nodes
        virtual void prune()
        {
            FlowDistributionNetwork::prune();
            this->_robust_changed = true;
        }

    protected:
        /* Internal Functions */
        virtual void compute_robustness() = 0;

    }; // class RobustFunctionalNetwork

} // namespace rfn


#endif	// _C_ROBUSTFUNCTIONALNETWORK_HPP

