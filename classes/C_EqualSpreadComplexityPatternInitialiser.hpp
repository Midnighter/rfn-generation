/*******************************************************************************
 * filename:    C_EqualSpreadComplexityPatternInitialiser.hpp
 * description:
 * author:      Moritz Beber
 * contact:     m (dot) beber (at) jacobs (dash) university (dot) de
 * created:     2011-02-11
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


#ifndef C_EQUALSPREADCOMPLEXITYPATTERNINITIALISER_HPP
#define	C_EQUALSPREADCOMPLEXITYPATTERNINITIALISER_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// project
#include "C_RandomPatternInitialiser.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace rfn {

    class EqualSpreadComplexityPatternInitialiser: public RandomPatternInitialiser {
        /* Derived From Singleton Pattern */
        DECLARE_DERIVED_SINGLETON(PatternInitialiser,\
            EqualSpreadComplexityPatternInitialiser)


    public:
        /* Member Functions */
        // ideal pattern according to a complexity
        virtual void init(ParameterManager* const parameters,
            Matrix* const pattern);

    }; // class EqualSpreadComplexityPatternInitialiser

} // namespace rfn


#endif	// C_EQUALSPREADCOMPLEXITYPATTERNINITIALISER_HPP

