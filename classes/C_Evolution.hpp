/*******************************************************************************
 * filename:    C_Evolution.hpp
 * description: Singleton class that drives the network evolution
 * author:      Moritz Beber
 * created:     2010-06-09
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


#ifndef _C_EVOLUTION_HPP
#define	_C_EVOLUTION_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// boost
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

// project
#include "M_Singleton.hpp"
#include "C_FlowDistributionNetwork.hpp"
#include "C_ParameterManager.hpp"
//#include "C_HDF5Storage.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace rfn {

    class Evolution {
        /* Singleton Pattern Declaration */
        DECLARE_BASE_SINGLETON(Evolution)

    protected:
        /* Data Members */
        typedef boost::uniform_real<double> Real;
        typedef boost::variate_generator<rfn::RandGen&, Real> Real_Number;
        unsigned long _current_time;
        Real* _val_dist;
        Real_Number* _val_gen;
        ParameterManager* _parameters;

    public:
        /* Member Functions */
        // _instance retrieval method
        static Evolution* instance(const FunctionalNetworkScheme scheme);
        // drives evolution
        virtual void run(FlowDistributionNetwork* const f_net);
        // _current_time get method
        unsigned long current_time() const
        {
            return this->_current_time;
        }
        // _current_time set method
        void current_time(const unsigned long t)
        {
            this->_current_time = t;
        }
        // _parameters set method
        void parameters(ParameterManager* const p)
        {
            this->_parameters = p;
        }

    protected:
        /* Internal Functions */
        // perform one evolutionary step
        void step(FlowDistributionNetwork* const f_net);

    };

} // namespace rfn


#endif	// _C_EVOLUTION_HPP

