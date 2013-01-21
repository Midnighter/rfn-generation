/*******************************************************************************
 * filename:    C_LowComplexityPatternInitialiser.cpp
 * description: Implementation of 'HighComplexityPatternInitialiser' class
 * author:      Moritz Beber
 * created:     2011-01-18
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
#include "C_LowComplexityPatternInitialiser.hpp"


/*******************************************************************************
 * Constructors & Destructor
 ******************************************************************************/


/*
 * Default constructor
 */
rfn::LowComplexityPatternInitialiser::LowComplexityPatternInitialiser():
    rfn::RandomPatternInitialiser()
{}

/*
 * Destructor
 */
rfn::LowComplexityPatternInitialiser::~LowComplexityPatternInitialiser()
{}


/*******************************************************************************
 * Member Functions
 ******************************************************************************/


/*
 * Randomly intialise the given pattern using 'k' entries. Until the ideal
 * pattern fulfills the complexity requirements.
 */
void
rfn::LowComplexityPatternInitialiser::init(rfn::ParameterManager* const params,
    rfn::Matrix* const pattern)
{
    // initialise
    gsl_matrix_set_zero(pattern);
    // 'values' holds pattern fractions
    double* values = new double[params->activated_k()];
    std::memset(values, 0, sizeof(double) * params->activated_k());
    short unsigned i = 0u;
    short unsigned j = 0u;
    // fill the values
    double epsilon = sqrt(params->error_threshold() * 2.0);
    values[0] = 1.0 - (double)(params->activated_k() - 1.0) * epsilon;
    for (i = 1u; i < params->activated_k(); ++i) {
        values[i] = epsilon;
    }
    // for a low complexity pattern all output column vectors overlap in their
    // entries
    for (j = 0u; j < params->nodes_in(); ++j) {
        for (i = 0u; i < params->activated_k(); ++i) {
            gsl_matrix_set(pattern, i, j, values[i]);
        }
    }
#ifdef VERBOSE
    std::cout << this->complexity(params, pattern) << std::endl;
#endif // VERBOSE
    delete[] values;
}

