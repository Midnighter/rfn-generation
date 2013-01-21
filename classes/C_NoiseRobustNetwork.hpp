/*******************************************************************************
 * filename:    C_NoiseRobustNetwork.hpp
 * description: Derived class for noise robust networks
 * author:      Moritz Beber
 * created:     2010-07-21
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


#ifndef _C_NOISEROBUSTNETWORK_HPP
#define	_C_NOISEROBUSTNETWORK_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// boost
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

// project
#include "C_RobustFunctionalNetwork.hpp"
#include "C_NoiseRobustParameterManager.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace rfn {

    class NoiseRobustNetwork: public RobustFunctionalNetwork {

    protected:
        /* Data Members */
        typedef boost::uniform_real<double> Real;
        typedef boost::variate_generator<rfn::RandGen&, Real> RealNumber;
        Real* _val_dist;
        RealNumber* _val_gen;
        NoiseRobustParameterManager* _noise_parameters;
        
    public:
        /* Constructors & Destructor */
        NoiseRobustNetwork();
        NoiseRobustNetwork(const NoiseRobustNetwork& c);
        virtual ~NoiseRobustNetwork();
        // polymorphic "copy constructor"
        virtual FlowDistributionNetwork* clone()
        {
            return new NoiseRobustNetwork(*this);
        }

        /* Operators */
        virtual FlowDistributionNetwork& operator=(const FlowDistributionNetwork& a);
        virtual FlowDistributionNetwork& operator=(FlowDistributionNetwork& a);
        NoiseRobustNetwork& operator=(const NoiseRobustNetwork& a);
        NoiseRobustNetwork& operator=(NoiseRobustNetwork& a);

    protected:
        /* Internal Functions */
        virtual void compute_robustness();
        void generate_noise_sle(Matrix* const sle, Matrix* const weights);
        void set_noise(Matrix* const weights, const short unsigned i,
            const short unsigned j,
            boost::graph_traits<rfn::Adjacency>::edge_descriptor& edge);
        double sum(const gsl_vector* const outgoing_weights) const;

    }; // class NoiseRobustNetwork

} // namespace rfn


#endif	// _C_NOISEROBUSTNETWORK_HPP

