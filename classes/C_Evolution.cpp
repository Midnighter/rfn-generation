/*******************************************************************************
 * filename:    C_Evolution.cpp
 * description: Implementation of 'Evolution' class
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
#include "C_Evolution.hpp"
#include "C_RobustEvolution.hpp"


/*******************************************************************************
 * Necessary Singleton Pattern Definitions
 ******************************************************************************/


DEFINE_BASE_SINGLETON(rfn::Evolution)


/*******************************************************************************
 * Constructor & Destructor
 ******************************************************************************/


/*
 * Default constructor
 */
rfn::Evolution::Evolution():
    _current_time(0L),
    _val_dist(NULL),
    _val_gen(NULL),
    _parameters(rfn::ParameterManager::instance())
{
    // tie rng to get a number between 0 and 1
    this->_val_dist = new Real;
    CHECK_CRITICAL_POINTER(this->_val_dist);
    this->_val_gen = new Real_Number(this->_parameters->rng(), *this->_val_dist);
    CHECK_CRITICAL_POINTER(this->_val_gen);
}

/*
 * Destructor
 */
rfn::Evolution::~Evolution()
{
    delete this->_val_gen;
    this->_val_gen = NULL;
    delete this->_val_dist;
    this->_val_dist = NULL;
    this->_parameters = NULL; // singleton destroys itself
}


/*******************************************************************************
 * Member Functions
 ******************************************************************************/


/*
 * Default instance generation for singleton
 */
rfn::Evolution*
rfn::Evolution::instance()
{
    if (rfn::Evolution::_instance == NULL) {
        std::cerr << "Warning: Instance not generated yet." << std::endl;
    }
    return rfn::Evolution::_instance;
}

/*
 * Instance generation for singleton
 */
rfn::Evolution*
rfn::Evolution::instance(const rfn::FunctionalNetworkScheme scheme)
{
    if (rfn::Evolution::_instance == NULL) {
        static rfn::Evolution::Guard g;
        switch (scheme) {
            case rfn::flow_evolved:
                rfn::Evolution::_instance = new rfn::Evolution;
                break;
            default:
                rfn::Evolution::_instance = new rfn::RobustEvolution;
                break;
        }

    }
    return rfn::Evolution::_instance;
}


/*
 * Evolution for functional flow networks
 */
void
rfn::Evolution::run(rfn::FlowDistributionNetwork* const f_net)
{
    using std::cout;
    using std::endl;
    unsigned when = 0u;
    bool store = false;
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
        if (this->_current_time % 200 == 0) {
            cout << "\033[0F\033[2Ktime: " << this->_current_time <<
                " flow error: " << f_net->flow_error() << endl;
        }
#endif
        if (store) {
            if (this->_current_time % when == 0) {
                f_net->save(this->_current_time);
            }
        }
        // proceed in evolution
        this->step(f_net);
        // check for abortion criteria
        if (gsl_fcmp(f_net->flow_error(), this->_parameters->error_threshold(),
            GSL_EPSILON) < 0)
        {
            // flow error is smaller than the required threshold
            break;
        }
    }
#ifdef VERBOSE
    cout << "\033[0F\033[2Ktime: " << this->_current_time <<
                " flow error: " << f_net->flow_error() << endl;
#endif
    f_net->prune();
#ifdef VERBOSE
    cout << "time: " << this->_current_time <<
                " flow error: " << f_net->flow_error() << endl;
    cout << endl;
#endif
    if (store) {
        f_net->store();
    }
    std::stringstream filename;
    filename << f_net->name() << "_final.dat";
    bfsys::ofstream output(filename.str(), std::ios::binary);
    f_net->write_to_binary_stream(output, (this->_current_time <=
        this->_parameters->max_time()) ? this->_current_time :
        this->_parameters->max_time());
    output.close();
}

/*
 * One evolutionary step
 */
void
rfn::Evolution::step(rfn::FlowDistributionNetwork* const f_net)
{
    // calculate flow error
    rfn::FlowDistributionNetwork next(*f_net);
    next.mutate();
    if (gsl_fcmp(next.flow_error(), f_net->flow_error(), GSL_EPSILON) < 0) {
        // flow error is smaller than old one
        *f_net = next;
    }
    else {
        // small probability of accepting change anyway to avoid local minima
        if (gsl_fcmp((*this->_val_gen)(), gsl_sf_exp((f_net->flow_error() -
            next.flow_error()) / (this->_parameters->annealing_factor() *
            f_net->flow_error())), GSL_EPSILON) < 0) {
            *f_net = next;
        }
    }
}