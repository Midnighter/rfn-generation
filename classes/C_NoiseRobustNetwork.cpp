/*******************************************************************************
 * filename:    C_NoiseRobustNetwork.cpp
 * description: Implementation of 'NoiseRobustNetwork' class
 * author:      Moritz Beber
 * created:     2010-07-21
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
#include "C_NoiseRobustNetwork.hpp"


/*******************************************************************************
 * Constructors & Destructor
 ******************************************************************************/


/*
 * Default constructor
 */
rfn::NoiseRobustNetwork::NoiseRobustNetwork():
    rfn::RobustFunctionalNetwork(),
    _val_dist(NULL),
    _val_gen(NULL),
    _noise_parameters(NULL)
{
    // tie rng to get a number between 0 and 1
    this->_val_dist = new Real;
    CHECK_CRITICAL_POINTER(this->_val_dist);
    this->_val_gen = new RealNumber(this->_parameters->rng(), *this->_val_dist);
    CHECK_CRITICAL_POINTER(this->_val_gen);
    this->_noise_parameters = static_cast<NoiseRobustParameterManager*>
        (this->_parameters);
}

/*
 * Copy constructor
 */
rfn::NoiseRobustNetwork::NoiseRobustNetwork(
    const rfn::NoiseRobustNetwork& c):
    rfn::RobustFunctionalNetwork(c),
    _val_dist(NULL),
    _val_gen(NULL),
    _noise_parameters(c._noise_parameters)
{
    // tie rng to get a number between 0 and 1
    // don't want to copy state of the random number generator
    this->_val_dist = new Real;
    CHECK_CRITICAL_POINTER(this->_val_dist);
    this->_val_gen = new RealNumber(this->_parameters->rng(), *this->_val_dist);
    CHECK_CRITICAL_POINTER(this->_val_gen);
}

/*
 * Destructor
 */
rfn::NoiseRobustNetwork::~NoiseRobustNetwork()
{
    delete this->_val_gen;
    this->_val_gen = NULL;
    delete this->_val_dist;
    this->_val_dist = NULL;
    this->_noise_parameters = NULL; // singleton destructs itself
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
rfn::NoiseRobustNetwork::operator=(const rfn::FlowDistributionNetwork& a)
{
    if (this != &a) {
        // expect only networks of same type to be assigned
        assert(typeid(*this) == typeid(a));
        // call superclass assignment
        rfn::RobustFunctionalNetwork::operator=(a);
        const NoiseRobustNetwork& other = static_cast
            <const NoiseRobustNetwork&>(a);
        this->_noise_parameters = other._noise_parameters;
    }
    return *this;
}

/*
 * Virtual assignment operator, all networks following an initial one are copy-
 * constructed during the evolution thus here we only transfer the bare minimum
 * values needed.
 */
rfn::FlowDistributionNetwork&
rfn::NoiseRobustNetwork::operator=(rfn::FlowDistributionNetwork& a)
{
    if (this != &a) {
        // expect only networks of same type to be assigned
        assert(typeid(*this) == typeid(a));
        // call superclass assignment
        rfn::RobustFunctionalNetwork::operator=(a);
        NoiseRobustNetwork& other = static_cast
            <NoiseRobustNetwork&>(a);
        this->_noise_parameters = other._noise_parameters;
    }
    return *this;
}

/*
 * Const assignment operator of this class calls the virtual operator so that
 * all values are properly assigned.
 */
rfn::NoiseRobustNetwork&
rfn::NoiseRobustNetwork::operator=(const rfn::NoiseRobustNetwork& a)
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
rfn::NoiseRobustNetwork&
rfn::NoiseRobustNetwork::operator=(rfn::NoiseRobustNetwork& a)
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
 * Internal Functions
 ******************************************************************************/


void
rfn::NoiseRobustNetwork::compute_robustness()
{
    rfn::NoiseRobustNetwork next(*this);
    this->_robustness = 0.0;
    Matrix* sle = gsl_matrix_calloc(this->_parameters->nodes_total(),
        this->_parameters->nodes_total());
    CHECK_CRITICAL_GSL_POINTER(sle, __FILE__, __LINE__);
    Matrix* weights = gsl_matrix_calloc(this->_parameters->nodes_total(),
        this->_parameters->nodes_total());
    CHECK_CRITICAL_GSL_POINTER(weights, __FILE__, __LINE__);
    const unsigned num_realisations = 10 * this->_parameters->nodes_middle();
    for (unsigned n = 0; n < num_realisations; ++n)
    {
        next.generate_noise_sle(sle, weights);
        next.generate_output(sle);
        next.compute_flow_error();
        if (gsl_fcmp(next._flow_error,
            this->_robust_parameters->robust_threshold(), GSL_EPSILON) < 0)
        {
            this->_robustness += 1.0;
        }
    }
    // robustness is a fraction of all nodes applicable
    this->_robustness /= (double)num_realisations;
    gsl_matrix_free(sle);
    gsl_matrix_free(weights);
}

void
rfn::NoiseRobustNetwork::generate_noise_sle(rfn::Matrix* const sle,
    rfn::Matrix* const weights)
{
    // matrix orientations are: link from j to i is recorded at element ij
    gsl_matrix_set_zero(weights);
    short unsigned i = 0;
    short unsigned j = 0;
    boost::graph_traits<rfn::Adjacency>::edge_descriptor edge;
    for (j = 0; j < this->_parameters->nodes_end_middle(); ++j) {
        for (i = this->_parameters->nodes_in();
            i < this->_parameters->nodes_total(); ++i)
        {
            if (i == j) {
                // no self-links allowed
                continue;
            }
            if (this->_parameters->is_input(j)
                && this->_parameters->is_middle(i))
            {
                // input nodes can only leak to middle nodes
                this->set_noise(weights, i, j, edge);
            }
            else if (this->_parameters->is_middle(j)
                && this->_parameters->is_middle(i))
            {
                // outgoing links between middle nodes themselves
                this->set_noise(weights, i, j, edge);
            }
            else if (this->_parameters->is_middle(j)
                && this->_parameters->is_output(i))
            {
                // links from middle nodes to output nodes
                this->set_noise(weights, i, j, edge);
            }
        }
    }
    gsl_matrix_set_identity(sle);
    double weights_sum = 0.0;
    for (j = 0; j < this->_parameters->nodes_end_middle(); ++j) {
        gsl_vector_const_view outgoing_weights =
            gsl_matrix_const_column(weights, j);
        weights_sum = this->sum(&outgoing_weights.vector);
        for (i = this->_parameters->nodes_in();
            i < this->_parameters->nodes_total(); ++i)
        {
            if (i != j && gsl_matrix_get(weights, i, j) != 0.0) {
                gsl_matrix_set(sle, i, j, -1.0 * gsl_matrix_get(weights, i, j)
                    / weights_sum);
            }
        }
    }
}

void
rfn::NoiseRobustNetwork::set_noise(rfn::Matrix* const weights,
    const short unsigned i, const short unsigned j,
    boost::graph_traits<rfn::Adjacency>::edge_descriptor& edge)
{
    bool found;
    boost::tie(edge, found) = boost::edge(j, i, *this->_network);
    if (found) {
        // flow on existing links is slightly reduced
        gsl_matrix_set(weights, i, j, 1.0 -
            this->_noise_parameters->noise_level()
            * (*this->_val_gen)());
    }
    else {
        // a small leakage flow goes over inexistant links
        gsl_matrix_set(weights, i, j,
            this->_noise_parameters->noise_level()
            * (*this->_val_gen)());
    }
}

double
rfn::NoiseRobustNetwork::sum(const gsl_vector* const outgoing_weights) const
{
    double sum = 0.0;
    for (short unsigned i = 0; i < outgoing_weights->size; ++i) {
        sum += gsl_vector_get(outgoing_weights, i);
    }
    return sum;
}