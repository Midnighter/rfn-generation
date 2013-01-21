/*******************************************************************************
 * filename:    C_EqualSpreadComplexityPatternInitialiser.cpp
 * description: Implementation of 'EqualSpreadComplexityPatternInitialiser' class
 * author:      Moritz Beber
 * created:     2011-02-11
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


/*******************************************************************************
 * Includes
 ******************************************************************************/


// gsl
#include <gsl/gsl_blas.h>
#include <gsl/gsl_math.h>

// project
#include "C_EqualSpreadComplexityPatternInitialiser.hpp"


/*******************************************************************************
 * Constructors & Destructor
 ******************************************************************************/


/*
 * Default constructor
 */
rfn::EqualSpreadComplexityPatternInitialiser::EqualSpreadComplexityPatternInitialiser():
    rfn::RandomPatternInitialiser()
{}

/*
 * Destructor
 */
rfn::EqualSpreadComplexityPatternInitialiser::~EqualSpreadComplexityPatternInitialiser()
{}


/*******************************************************************************
 * Member Functions
 ******************************************************************************/


/*
 * Intialise the ideal output pattern with equal amounts over all activated 'k'
 * output nodes
 */
void
rfn::EqualSpreadComplexityPatternInitialiser::init(rfn::ParameterManager* const params,
    rfn::Matrix* const pattern)
{
    // initialise
    gsl_matrix_set_zero(pattern);
    short unsigned i = 0u;
    short unsigned j = 0u;
    // 'value' holds pattern fraction
    double value = 1.0 / (double)params->activated_k();
    // for a higher complexity pattern the values are spread out equally
    short unsigned begin = 0u;
    for (j = 0u; j < params->nodes_in(); ++j) {
        for (i = 0u; i < params->activated_k(); ++i) {
            gsl_matrix_set(pattern, (i + begin) % params->nodes_out(), j,
                    value);
        }
        begin += 1u;
    }
}
