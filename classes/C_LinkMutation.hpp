/*******************************************************************************
 * filename:    C_LinkMutation.hpp
 * description: Derived class for mutating single links in a network
 * author:      Moritz Beber
 * created:     2010-04-16
 * copyright:   Jacobs University Bremen. All rights reserved.
 ******************************************************************************/


#ifndef _C_LINKMUTATION_HPP
#define	_C_LINKMUTATION_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// project
#include "common_definitions.hpp"
#include "C_Mutation.hpp"
#include "C_ParameterManager.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace rfn {
    
    class LinkMutation: public Mutation {
        /* Derived From Singleton Pattern */
        DECLARE_DERIVED_SINGLETON(Mutation, LinkMutation)

    protected:
        /* Data Members */
        typedef boost::bernoulli_distribution<double> Chance;
        typedef boost::variate_generator<RandGen&, Chance > Yes;
        typedef boost::uniform_smallint<short unsigned> Small;
        typedef boost::variate_generator<rfn::RandGen&, Small> Small_Number;
        Chance* _b_dist;
        Yes* _bool_gen;
        Small* _node_dist;
        Small_Number* _node_gen;

    public:
        /* Member Functions */
        // 'network' modifying method
        virtual void mutate(Adjacency* const network);

    }; // class LinkMutation

} // namespace rfn


#endif	// _C_LINKMUTATION_HPP

