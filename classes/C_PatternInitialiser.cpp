/*******************************************************************************
 * filename:    C_PatternInitialiser.cpp
 * description: Implementation of 'PatternInitialiser' class
 * author:      Moritz Beber
 * created:     2010-06-09
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


/*******************************************************************************
 * Includes
 ******************************************************************************/


// std
#include <iostream>
#include <cstdlib>

// gsl
#include <gsl/gsl_blas.h>

// project
#include "C_PatternInitialiser.hpp"
#include "C_RandomPatternInitialiser.hpp"
#include "C_ComplexityPatternInitialiser.hpp"
#include "C_HighComplexityPatternInitialiser.hpp"
#include "C_LowComplexityPatternInitialiser.hpp"
#include "C_EqualComplexityPatternInitialiser.hpp"
#include "C_EqualSpreadComplexityPatternInitialiser.hpp"


/*******************************************************************************
 * Necessary Singleton Pattern Definitions
 ******************************************************************************/


DEFINE_BASE_SINGLETON(rfn::PatternInitialiser)


/*******************************************************************************
 * Constructor & Destructor
 ******************************************************************************/


/*
 * Default constructor
 */
rfn::PatternInitialiser::PatternInitialiser()
{}

/*
 * Destructor
 */
rfn::PatternInitialiser::~PatternInitialiser()
{}


/*******************************************************************************
 * Member Functions
 ******************************************************************************/


/*
 * Default instance generation for singleton
 */
rfn::PatternInitialiser*
rfn::PatternInitialiser::instance()
{
    if (rfn::PatternInitialiser::_instance == NULL) {
        std::cerr << "Error: Instance not generated yet. Returning NULL pointer!"
            << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return rfn::PatternInitialiser::_instance;
}

/*
 * Instance generation for singleton
 */
rfn::PatternInitialiser*
rfn::PatternInitialiser::instance(const rfn::PatternInitScheme scheme)
{
    if (rfn::PatternInitialiser::_instance == NULL) {
        static rfn::PatternInitialiser::Guard g;
        switch (scheme) {
            case rfn::random_k:
                rfn::PatternInitialiser::_instance = new
                    rfn::RandomPatternInitialiser;
                break;
            case rfn::complexity:
                rfn::PatternInitialiser::_instance = new
                    rfn::ComplexityPatternInitialiser;
                break;
            case rfn::high_complexity:
                rfn::PatternInitialiser::_instance = new
                    rfn::HighComplexityPatternInitialiser;
                break;
            case rfn::low_complexity:
                rfn::PatternInitialiser::_instance = new
                    rfn::LowComplexityPatternInitialiser;
                break;
            case rfn::equal_complexity:
                rfn::PatternInitialiser::_instance = new
                    rfn::EqualComplexityPatternInitialiser;
                break;
            case rfn::equal_spread_complexity:
                rfn::PatternInitialiser::_instance = new
                    rfn::EqualSpreadComplexityPatternInitialiser;
                break;
        }

    }
    return rfn::PatternInitialiser::_instance;
}


/*******************************************************************************
 * Internal Functions
 ******************************************************************************/

double
rfn::PatternInitialiser::complexity(
    rfn::ParameterManager* const parameters, rfn::Matrix* const pattern)
{
    int status = 0;
    double sum = 0.0;
    double tmp = 0.0;
    gsl_vector_view one;
    gsl_vector_view other;
    for (unsigned first = 0u; first < (parameters->nodes_in() - 1u); ++first) {
        for (unsigned second = first + 1; second < parameters->nodes_in();
            ++second)
        {
            one = gsl_matrix_column(pattern, first);
            other = gsl_matrix_column(pattern, second);
            status = gsl_blas_ddot (&one.vector, &other.vector, &tmp);
            CHECK_CRITICAL_GSL_STATUS(status, __FILE__, __LINE__);
            sum += tmp;
        }
    }
    return sum;
}
