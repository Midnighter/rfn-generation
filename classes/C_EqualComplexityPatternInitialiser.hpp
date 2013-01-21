/*******************************************************************************
 * filename:    C_EqualComplexityPatternInitialiser.hpp
 * description:
 * author:      Moritz Beber
 * contact:     m (dot) beber (at) jacobs (dash) university (dot) de
 * created:     2011-02-11
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


#ifndef C_EQUALCOMPLEXITYPATTERNINITIALISER_HPP
#define	C_EQUALCOMPLEXITYPATTERNINITIALISER_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// project
#include "C_RandomPatternInitialiser.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace rfn {

    class EqualComplexityPatternInitialiser: public RandomPatternInitialiser {
        /* Derived From Singleton Pattern */
        DECLARE_DERIVED_SINGLETON(PatternInitialiser,\
            EqualComplexityPatternInitialiser)


    public:
        /* Member Functions */
        // ideal pattern according to a complexity
        virtual void init(ParameterManager* const parameters,
            Matrix* const pattern);

    }; // class EqualComplexityPatternInitialiser

} // namespace rfn


#endif	// C_EQUALCOMPLEXITYPATTERNINITIALISER_HPP

