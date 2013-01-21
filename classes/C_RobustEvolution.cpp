/*******************************************************************************
 * filename:    C_RobustEvolution.cpp
 * description: Implementation of 'RobustEvolution' class
 * author:      Moritz Beber
 * created:     2010-06-11
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


/*******************************************************************************
 * Includes
 ******************************************************************************/


// std
#include <iostream>

// gsl
#include <gsl/gsl_math.h>
#include <gsl/gsl_sf_exp.h>

// project
#include "C_RobustEvolution.hpp"


/*******************************************************************************
 * Constructors & Destructor
 ******************************************************************************/


/*
 * Default constructor
 */
rfn::RobustEvolution::RobustEvolution():
    rfn::Evolution(),
    _robust_parameters(NULL)
{
    this->_robust_parameters = dynamic_cast<rfn::RobustParameterManager*>
        (this->_parameters);
}

/*
 * Destructor
 */
rfn::RobustEvolution::~RobustEvolution()
{
    this->_robust_parameters = NULL; // singleton controls its destruction
}


/*******************************************************************************
 * Member Functions
 ******************************************************************************/


/*
 * Evolution of a robust network
 */
void
rfn::RobustEvolution::run(rfn::FlowDistributionNetwork* const f_net)
{
    using std::cout;
    using std::endl;
    rfn::RobustFunctionalNetwork* const rf_net = dynamic_cast
        <rfn::RobustFunctionalNetwork*>(f_net);
    unsigned when = 0u;
    bool store = false;
    bool intermediate = true;
    if (this->_parameters->snapshots() > 0) {
        store = true;
        when = this->_parameters->max_time() /
            this->_parameters->snapshots();
    }
#ifdef VERBOSE
    cout << endl;
#endif
    // use the actual _current_time for the evolution
    for (; this->_current_time <= this->_parameters->max_time();
        ++this->_current_time)
    {
#ifdef VERBOSE
        if (this->_current_time % 100 == 0) {
        cout << "\033[0F\033[2Ktime: " << this->_current_time <<
            " flow error: " << rf_net->flow_error() << " robustness: "
            << rf_net->robustness() << endl;
        }
#endif
        // store item in history
        if (store) {
            if (this->_current_time % when == 0) {
                rf_net->save(this->_current_time);
            }
        }
        // proceed in evolution
        this->step(rf_net);
        // check for abortion criteria
        if (gsl_fcmp(rf_net->robustness(),
            this->_robust_parameters->min_robustness(), GSL_EPSILON) >= 0)
        {
            // robustness is greater than or equal to the required threshold
            break;
        }
        if (intermediate && gsl_fcmp(f_net->flow_error(),
            this->_parameters->error_threshold(), GSL_EPSILON) < 0)
        {
            // flow error is smaller than the required threshold
            // save intermediate state
            std::stringstream filename;
            filename << rf_net->name() << "_flow.dat";
            bfsys::ofstream output(filename.str(), std::ios::binary);
            rf_net->write_to_binary_stream(output, this->_current_time);
            output.close();
            intermediate = false;
        }
    }
#ifdef VERBOSE
    cout << "\033[0F\033[2Ktime: " << this->_current_time <<
                " flow error: " << rf_net->flow_error() << " robustness: "
                << rf_net->robustness() << endl;
#endif
    rf_net->prune();
#ifdef VERBOSE
    cout << "time: " << this->_current_time <<
                " flow error: " << rf_net->flow_error() << " robustness: "
                << rf_net->robustness() << endl;
    cout << endl;
#endif
    if (store) {
        rf_net->store();
    }
    std::stringstream filename;
    filename << rf_net->name() << "_final.dat";
    bfsys::ofstream output(filename.str(), std::ios::binary);
    rf_net->write_to_binary_stream(output, (this->_current_time <=
        this->_parameters->max_time()) ? this->_current_time :
        this->_parameters->max_time());
    output.close();
}

/*
 * One evolutionary step
 */
void
rfn::RobustEvolution::step(rfn::RobustFunctionalNetwork* const rf_net)
{
    // calculate flow error
    rfn::RobustFunctionalNetwork* next(static_cast
        <rfn::RobustFunctionalNetwork*>(rf_net->clone()));
    CHECK_CRITICAL_POINTER(next);
    next->mutate();
    if (gsl_fcmp(rf_net->flow_error(), this->_parameters->error_threshold(),
        GSL_EPSILON) < 0)
    {
        if (gsl_fcmp(next->flow_error(), this->_parameters->error_threshold(),
            GSL_EPSILON) >= 0)
        {
            // mutated network not below flow error threshold, i.e., it cannot be
            // robust
            delete next;
            return;
        }
        // flow error is smaller than threshold: test robustness
        if (gsl_fcmp(next->robustness(), rf_net->robustness(), GSL_EPSILON) > 0)
        {
            // robustness is greater than old one
            *rf_net = *next;
        }
        else {
            // small probability of accepting change anyway to avoid local minima
            if (gsl_fcmp((*this->_val_gen)(), gsl_sf_exp((next->robustness() -
                rf_net->robustness()) /
                (this->_robust_parameters->robust_factor() *
                (1 - rf_net->robustness()))), GSL_EPSILON) < 0)
            {
                *rf_net = *next;
            }
        }
    }
    else {
        // flow error is larger than threshold: not functional yet
        if (gsl_fcmp(next->flow_error(), rf_net->flow_error(), GSL_EPSILON) < 0)
        {
            // flow error is smaller than old one
            *rf_net = *next;
        }
        else {
            // small probability of accepting change anyway to avoid local minima
            if (gsl_fcmp((*this->_val_gen)(), gsl_sf_exp((rf_net->flow_error() -
                next->flow_error()) / (this->_parameters->annealing_factor() *
                rf_net->flow_error())), GSL_EPSILON) < 0)
            {
                *rf_net = *next;
            }
        }
    }
    delete next;
}
