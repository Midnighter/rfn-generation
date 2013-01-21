/*******************************************************************************
 * filename:    C_PatternInitialiser.hpp
 * description: Singleton class that initialises the ideal pattern
 * author:      Moritz Beber
 * created:     2010-06-09
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


#ifndef _C_PATTERNINITIALISER_HPP
#define	_C_PATTERNINITIALISER_HPP


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
    
    class PatternInitialiser {
        /* Singleton Pattern Declaration */
        DECLARE_BASE_SINGLETON(PatternInitialiser)

    public:
        /* Member Functions */
        // _instance retrieval method
        static PatternInitialiser* instance(const PatternInitScheme scheme);
        // pure virtual ideal pattern init method
        virtual void init(ParameterManager* const parameters,
            Matrix* const pattern) = 0;

    protected:
        /* Internal Functions */
        double complexity(ParameterManager* const parameters,
            Matrix* const pattern);

    }; // class PatternInitialiser

} // namespace rfn


#endif	// _C_PATTERNINITIALISER_HPP

