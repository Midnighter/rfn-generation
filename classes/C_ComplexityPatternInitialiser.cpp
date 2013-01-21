/*******************************************************************************
 * filename:    C_ComplexityPatternInitialiser.cpp
 * description: Implementation of 'ComplexityPatternInitialiser' class
 * author:      Moritz Beber
 * created:     2010-10-17
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


/*******************************************************************************
 * Includes
 ******************************************************************************/


// gsl
#include <gsl/gsl_math.h>

// project
#include "C_ComplexityPatternInitialiser.hpp"


/*******************************************************************************
 * Constructors & Destructor
 ******************************************************************************/


/*
 * Default constructor
 */
rfn::ComplexityPatternInitialiser::ComplexityPatternInitialiser():
    rfn::RandomPatternInitialiser()
{}

/*
 * Destructor
 */
rfn::ComplexityPatternInitialiser::~ComplexityPatternInitialiser()
{}


/*******************************************************************************
 * Member Functions
 ******************************************************************************/


/*
 * Randomly intialise the given pattern using 'k' entries. Until the ideal
 * pattern fulfills the complexity requirements.
 */
void
rfn::ComplexityPatternInitialiser::init(rfn::ParameterManager* const params,
    rfn::Matrix* const pattern)
{
    double complexity = 0.0;
    unsigned max = 1000000u;
    unsigned status = 0u;
    /*
     * Continue to randomly initialise while 'complexity' < 'lower_bound' or
     * 'complexity' > 'upper_bound' up until a maximum number of tries.
     */
    do {
        if (status == max) {
            throw "Maximum number of random initialisations reached";
        }
        rfn::RandomPatternInitialiser::init(params, pattern);
        complexity = this->complexity(params, pattern);
        ++status;
    } while (gsl_fcmp(complexity, params->upper_bound(), GSL_EPSILON) == 1 ||
        gsl_fcmp(complexity, params->lower_bound(), GSL_EPSILON) == -1);
}
