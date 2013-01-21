/*******************************************************************************
 * filename:    C_LowComplexityPatternInitialiser.hpp
 * description:
 * author:      Moritz Beber
 * contact:     m (dot) beber (at) jacobs (dash) university (dot) de
 * created:     2011-01-18
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


#ifndef C_LOWCOMPLEXITYPATTERNINITIALISER_HPP
#define	C_LOWCOMPLEXITYPATTERNINITIALISER_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// project
#include "C_RandomPatternInitialiser.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace rfn {

    class LowComplexityPatternInitialiser: public RandomPatternInitialiser {
        /* Derived From Singleton Pattern */
        DECLARE_DERIVED_SINGLETON(PatternInitialiser,\
            LowComplexityPatternInitialiser)


    public:
        /* Member Functions */
        // ideal pattern according to a complexity
        virtual void init(ParameterManager* const parameters,
            Matrix* const pattern);

    }; // class LowComplexityPatternInitialiser

} // namespace rfn


#endif	// C_LOWCOMPLEXITYPATTERNINITIALISER_HPP

