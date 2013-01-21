/*******************************************************************************
 * filename:    C_EqualComplexityPatternInitialiser.cpp
 * description: Implementation of 'EqualComplexityPatternInitialiser' class
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
#include "C_EqualComplexityPatternInitialiser.hpp"


/*******************************************************************************
 * Constructors & Destructor
 ******************************************************************************/


/*
 * Default constructor
 */
rfn::EqualComplexityPatternInitialiser::EqualComplexityPatternInitialiser():
    rfn::RandomPatternInitialiser()
{}

/*
 * Destructor
 */
rfn::EqualComplexityPatternInitialiser::~EqualComplexityPatternInitialiser()
{}


/*******************************************************************************
 * Member Functions
 ******************************************************************************/


/*
 * Intialise the ideal output pattern with equal amounts over all activated 'k'
 * output nodes
 */
void
rfn::EqualComplexityPatternInitialiser::init(rfn::ParameterManager* const params,
    rfn::Matrix* const pattern)
{
    // initialise
    gsl_matrix_set_zero(pattern);
    short unsigned i = 0u;
    short unsigned j = 0u;
    // 'value' holds pattern fraction
    double value = 1.0 / (double)params->activated_k();
    // for a low complexity pattern all output column vectors overlap in their
    // entries
    for (j = 0u; j < params->nodes_in(); ++j) {
        for (i = 0u; i < params->activated_k(); ++i) {
            gsl_matrix_set(pattern, i, j, value);
        }
    }
}
