/*******************************************************************************
 * filename:    C_RobustEvolution.hpp
 * description: Derived class for running evolution of a robust network
 * author:      Moritz Beber
 * created:     2010-06-11
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


#ifndef _C_ROBUSTEVOLUTION_HPP
#define	_C_ROBUSTEVOLUTION_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// project
#include "C_Evolution.hpp"
#include "C_RobustParameterManager.hpp"
#include "C_RobustFunctionalNetwork.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace rfn {

    class RobustEvolution: public Evolution {
        
        /* Derived From Singleton Pattern */
        DECLARE_DERIVED_SINGLETON(Evolution, RobustEvolution)

    protected:
        /* Data Members */
        RobustParameterManager* _robust_parameters;

    public:
        /* Member Functions */
        virtual void run(FlowDistributionNetwork* const f_net);

    protected:
        /* Internal Functions */
        // perform one evolutionary step
        void step(RobustFunctionalNetwork* const rf_net);
        
    };
}

#endif	// _C_ROBUSTEVOLUTION_HPP

