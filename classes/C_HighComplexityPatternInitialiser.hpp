/*******************************************************************************
 * filename:    C_HighComplexityPatternInitialiser.hpp
 * description:
 * author:      Moritz Beber
 * contact:     m (dot) beber (at) jacobs (dash) university (dot) de
 * created:     2011-01-18
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


#ifndef C_HIGHCOMPLEXITYPATTERNINITIALISER_HPP
#define	C_HIGHCOMPLEXITYPATTERNINITIALISER_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// project
#include "C_RandomPatternInitialiser.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace rfn {

    class HighComplexityPatternInitialiser: public RandomPatternInitialiser {
        /* Derived From Singleton Pattern */
        DECLARE_DERIVED_SINGLETON(PatternInitialiser,\
            HighComplexityPatternInitialiser)


    public:
        /* Member Functions */
        // ideal pattern according to a complexity
        virtual void init(ParameterManager* const parameters,
            Matrix* const pattern);

    }; // class HighComplexityPatternInitialiser

} // namespace rfn


#endif	// C_HIGHCOMPLEXITYPATTERNINITIALISER_HPP

