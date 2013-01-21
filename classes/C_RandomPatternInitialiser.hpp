/*******************************************************************************
 * filename:    C_RandomPatternInitialiser.hpp
 * description: Derived class for ranomdly initialising an ideal pattern
 * author:      Moritz Beber
 * created:     2010-06-10
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


#ifndef _C_RANDOMPATTERNINITIALISER_HPP
#define	_C_RANDOMPATTERNINITIALISER_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// project
#include "C_PatternInitialiser.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace rfn {

    class RandomPatternInitialiser: public PatternInitialiser {
        /* Derived From Singleton Pattern */
        DECLARE_DERIVED_SINGLETON(PatternInitialiser, RandomPatternInitialiser)
        
    public:
        /* Member Functions */
        // ideal pattern initialisation
        virtual void init(ParameterManager* const parameters,
            Matrix* const pattern);

    }; // class RandomPatternInitialiser

} // namespace rfn


#endif	// _C_RANDOMPATTERNINITIALISER_HPP

