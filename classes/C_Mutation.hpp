/*******************************************************************************
 * filename:    C_Mutation.hpp
 * description: Pure virtual base class for mutating a network (singleton)
 * author:      Moritz Beber
 * created:     2010-04-16
 * copyright:   Jacobs University Bremen. All rights reserved.
 ******************************************************************************/


#ifndef _C_MUTATION_HPP
#define	_C_MUTATION_HPP


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
    
    class Mutation {
        /* Singleton Pattern Declaration */
        DECLARE_BASE_SINGLETON(Mutation)

    protected:
        /* Data Members */
        ParameterManager* _parameters;
        
    public:
        /* Member Functions */
        // _instance retrieval method
        static Mutation* instance(const MutationScheme scheme);
        // pure virtual 'network' modifying method
        virtual void mutate(Adjacency* const network) = 0;
        // _parameters set method
        void parameters(ParameterManager* const p)
        {
            this->_parameters = p;
        }

    }; // class Mutation

} // namespace rfn


#endif	// _C_MUTATION_HPP

