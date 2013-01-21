/*******************************************************************************
 * filename:    C_RobustFunctionalNetwork.cpp
 * description: Implementation of the 'RobustFunctionalNetwork' class
 * author:      Moritz Beber
 * created:     2010-07-05
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
#include "C_RobustFunctionalNetwork.hpp"


/*******************************************************************************
 * Constructors & Destructor
 ******************************************************************************/


/*
 * Default constructor
 */
rfn::RobustFunctionalNetwork::RobustFunctionalNetwork():
    rfn::FlowDistributionNetwork(),
    _robust_changed(true),
    _robustness(0.0),
    _robust_parameters(NULL)
{
    this->_robust_parameters = dynamic_cast<rfn::RobustParameterManager*>
        (this->_parameters);
    
}

/*
 * Copy constructor
 */
rfn::RobustFunctionalNetwork::RobustFunctionalNetwork(
    const RobustFunctionalNetwork& c):
    rfn::FlowDistributionNetwork(c),
    _robust_changed(c._robust_changed),
    _robustness(c._robustness),
    _robust_parameters(c._robust_parameters)
{}

/*
 * Destructor
 */
rfn::RobustFunctionalNetwork::~RobustFunctionalNetwork()
{
    this->_robust_parameters = NULL; // singleton destroys itself
}


/*******************************************************************************
 * Operators
 ******************************************************************************/


/*
 * Virtual const assignment operator, all networks following an initial one are
 * copy-constructed during the evolution thus here we only transfer the bare
 * minimum values needed.
 */
rfn::FlowDistributionNetwork&
rfn::RobustFunctionalNetwork::operator=(const rfn::FlowDistributionNetwork& a)
{
    if (this != &a) {
        // expect only networks of same type to be assigned
        assert(typeid(*this) == typeid(a));
        // call superclass assignment
        rfn::FlowDistributionNetwork::operator=(a);
        // cast to this class from possibly derived classes
        const rfn::RobustFunctionalNetwork& b = static_cast
            <const RobustFunctionalNetwork&>(a);
        // use 'b' for further assignments
        this->_robust_changed = b._robust_changed;
        this->_robustness = b._robustness;
        this->_robust_parameters = b._robust_parameters;
    }
    return *this;
}

/*
 * Virtual assignment operator, all networks following an initial one are copy-
 * constructed during the evolution thus here we only transfer the bare minimum
 * values needed.
 */
rfn::FlowDistributionNetwork&
rfn::RobustFunctionalNetwork::operator=(rfn::FlowDistributionNetwork& a)
{
    if (this != &a) {
        // expect only networks of same type to be assigned
        assert(typeid(*this) == typeid(a));
        // call superclass assignment
        rfn::FlowDistributionNetwork::operator=(a);
        // cast to this class from possibly derived classes
        rfn::RobustFunctionalNetwork& b = static_cast
            <RobustFunctionalNetwork&>(a);
        // use 'b' for further assignments
        this->_robust_changed = b._robust_changed;
        this->_robustness = b._robustness;
        this->_robust_parameters = b._robust_parameters;
    }
    return *this;
}

/*
 * Const assignment operator of this class calls the virtual operator so that
 * all values are properly assigned.
 */
rfn::RobustFunctionalNetwork&
rfn::RobustFunctionalNetwork::operator=(const RobustFunctionalNetwork& a)
{
    if (this != &a) {
        // expect only networks of same type to be assigned
        assert(typeid(*this) == typeid(a));
        // cast to virtual assignment operator type
        const FlowDistributionNetwork& other = static_cast
            <const FlowDistributionNetwork&>(a);
        // call the virtual assignment operator with its proper signature
        this->operator=(other);
    }
    return *this;
}

/*
 * Assignment operator of this class calls the virtual operator so that
 * all values are properly assigned.
 */
rfn::RobustFunctionalNetwork&
rfn::RobustFunctionalNetwork::operator=(RobustFunctionalNetwork& a)
{
    if (this != &a) {
        // expect only networks of same type to be assigned
        assert(typeid(*this) == typeid(a));
        // cast to virtual assignment operator type
        FlowDistributionNetwork& other = static_cast
            <FlowDistributionNetwork&>(a);
        // call the virtual assignment operator with its proper signature
        this->operator=(other);
    }
    return *this;
}


/*******************************************************************************
 * Member Functions
 ******************************************************************************/


void
rfn::RobustFunctionalNetwork::write_to_binary_stream(bfsys::ofstream& out,
    const unsigned long iteration)
{
    rfn::FlowDistributionNetwork::write_to_binary_stream(out, iteration);
    out.write((char*)&this->_robustness, sizeof(double));
}

unsigned long
rfn::RobustFunctionalNetwork::read_from_binary_stream(bfsys::ifstream& in)
{
    unsigned long iteration = 0L;
    iteration = rfn::FlowDistributionNetwork::read_from_binary_stream(in);
    in.read((char*)&this->_robustness, sizeof(double));
    return iteration;
}

double
rfn::RobustFunctionalNetwork::robustness()
{
    if (this->_robust_changed) {
        if (gsl_fcmp(this->_flow_error,
            this->_robust_parameters->robust_threshold(), GSL_EPSILON) < 0)
        {
            // flow error is below threshold, i.e., robustness is relevant
            this->compute_robustness();
        }
        else {
            // flow error above threshold means the network cannot be robust
            this->_robustness = 0.0;
        }
        this->_robust_changed = false;
    }
    return this->_robustness;
}
