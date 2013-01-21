/*******************************************************************************
 * filename:    CPathMutation.hpp
 * description: Derived class for mutating paths in a network
 * author:      Moritz Beber
 * created:     2010-04-16
 * copyright:   Jacobs University Bremen. All rights reserved.
 ******************************************************************************/


#ifndef _CPATHMUTATION_HPP
#define	_CPATHMUTATION_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// boost
#include <boost/random/bernoulli_distribution.hpp>
#include <boost/random/uniform_smallint.hpp>
#include <boost/random/variate_generator.hpp>

// project
#include "common_definitions.hpp"
#include "C_Mutation.hpp"
#include "C_ParameterManager.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace rfn {
    
    class PathMutation: public Mutation {
        /* Derived From Singleton Pattern */
        DECLARE_DERIVED_SINGLETON(Mutation, PathMutation)

    protected:
        /* Data Members */
        typedef boost::bernoulli_distribution<double> Chance;
        typedef boost::variate_generator<RandGen&, Chance > Yes;
        typedef boost::uniform_smallint<short unsigned> Small;
        typedef boost::variate_generator<rfn::RandGen&, Small> Small_Number;
        Chance* _b_dist;
        Yes* _bool_gen;
        Small* _in_dist;
        Small_Number* _in_gen;
        Small* _mid_dist;
        Small_Number* _mid_gen;
        Small* _out_dist;
        Small_Number* _out_gen;
        Small* _us_dist;
        Small_Number* _len_gen;
        
    public:
        /* Member Functions */
        // 'network' modifying method
        virtual void mutate(Adjacency* const network);

    }; // class PathMutation

} // namespace rfn


#endif	// _CPATHMUTATION_HPP

