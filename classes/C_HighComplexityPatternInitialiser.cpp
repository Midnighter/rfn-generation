/*******************************************************************************
 * filename:    C_HighComplexityPatternInitialiser.cpp
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
#include "C_HighComplexityPatternInitialiser.hpp"


/*******************************************************************************
 * Constructors & Destructor
 ******************************************************************************/


/*
 * Default constructor
 */
rfn::HighComplexityPatternInitialiser::HighComplexityPatternInitialiser():
    rfn::RandomPatternInitialiser()
{}

/*
 * Destructor
 */
rfn::HighComplexityPatternInitialiser::~HighComplexityPatternInitialiser()
{}


/*******************************************************************************
 * Member Functions
 ******************************************************************************/


/*
 * Randomly intialise the given pattern using 'k' entries. Until the ideal
 * pattern fulfills the complexity requirements.
 */
void
rfn::HighComplexityPatternInitialiser::init(rfn::ParameterManager* const params,
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
    // for a high complexity pattern the highest entry in each column vector
    // should not overlap with any other
    short unsigned begin = 0u;
    for (j = 0u; j < params->nodes_in(); ++j) {
        for (i = 0u; i < params->activated_k(); ++i) {
            gsl_matrix_set(pattern, (i + begin) % params->nodes_out(), j,
                    values[i]);
        }
        begin += 1u;
    }
#ifdef VERBOSE
    std::cout << this->complexity(params, pattern) << std::endl;
#endif // VERBOSE
    delete[] values;
}

