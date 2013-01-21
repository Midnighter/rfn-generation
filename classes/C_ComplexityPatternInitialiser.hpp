/*******************************************************************************
 * filename:    C_ComplexityPatternInitialiser.hpp
 * description:
 * author:      Moritz Beber
 * contact:     m (dot) beber (at) jacobs (dash) university (dot) de
 * created:     2010-10-17
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


#ifndef C_COMPLEXITYPATTERNINITIALISER_HPP
#define	C_COMPLEXITYPATTERNINITIALISER_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// project
#include "C_RandomPatternInitialiser.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace rfn {

    class ComplexityPatternInitialiser: public RandomPatternInitialiser {
        /* Derived From Singleton Pattern */
        DECLARE_DERIVED_SINGLETON(PatternInitialiser,\
            ComplexityPatternInitialiser)


    public:
        /* Member Functions */
        // ideal pattern according to a complexity
        virtual void init(ParameterManager* const parameters,
            Matrix* const pattern);

    }; // class ComplexityPatternInitialiser

} // namespace rfn


#endif	// C_COMPLEXITYPATTERNINITIALISER_HPP

