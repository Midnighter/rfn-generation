/*******************************************************************************
 * filename:    C_FlowDistributionNetwork.cpp
 * description: Implementation of 'FlowDistributionNetwork' class
 * author:      Moritz Beber
 * created:     2010-04-12
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 * 'FlowDistributionNetwork's that are copy constructed share the same
 * '_ideal_pattern'.
 ******************************************************************************/


/*******************************************************************************
 * Includes
 ******************************************************************************/


// std
#include <cstdlib>
#include <iostream>

// gsl
#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_blas.h>

// boost
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/pending/queue.hpp>

// project
#include "C_FlowDistributionNetwork.hpp"


/*******************************************************************************
 * Constructors & Destructor
 ******************************************************************************/


/*
 * Default constructor
 */
rfn::FlowDistributionNetwork::FlowDistributionNetwork():
    _original(true),
    _not_assigned(false),
    _changed(true),
    _flow_error(0.0),
    _network_name(""),
    _network(NULL),
    _ideal_pattern(NULL),
    _output_pattern(NULL),
    _network_init(NULL),
    _pattern_init(NULL),
    _mutation(NULL),
    _parameters(rfn::ParameterManager::instance()),
    _history(NULL)
{
    if (this->_parameters != NULL) {
        // network
        this->_network = new Adjacency(this->_parameters->nodes_total());
        CHECK_CRITICAL_POINTER(this->_network);
        // ideal pattern
        this->_ideal_pattern = gsl_matrix_calloc(this->_parameters->nodes_out(),
            this->_parameters->nodes_in());
        CHECK_CRITICAL_GSL_POINTER(this->_ideal_pattern, __FILE__, __LINE__);
        // output pattern
        this->_output_pattern = gsl_matrix_calloc(this->_parameters->nodes_out(),
            this->_parameters->nodes_in());
        CHECK_CRITICAL_GSL_POINTER(this->_output_pattern, __FILE__, __LINE__);
        // initialise network with starting connections
        this->_network_init = rfn::NetworkInitialiser::instance(
            this->_parameters->network_init());
        CHECK_CRITICAL_POINTER(this->_network_init);
        this->_network_init->init(this->_parameters, this->_network);
        // initialise ideal pattern
        this->_pattern_init = rfn::PatternInitialiser::instance(
            this->_parameters->pattern_init());
        CHECK_CRITICAL_POINTER(this->_pattern_init);
        this->_pattern_init->init(this->_parameters, this->_ideal_pattern);
        this->_mutation = this->_mutation->instance(this->_parameters->
            mutation_type());
        CHECK_CRITICAL_POINTER(this->_mutation);
        this->_history = new History();
        CHECK_CRITICAL_POINTER(this->_history);
    }
}

/*
 * Copy constructor
 */
rfn::FlowDistributionNetwork::FlowDistributionNetwork(
    const rfn::FlowDistributionNetwork& c):
    _original(false),
    _not_assigned(true),
    _changed(c._changed),
    _flow_error(c._flow_error),
    _network_name(c._network_name),
    _network(NULL),
    _ideal_pattern(c._ideal_pattern),
    _output_pattern(NULL),
    _network_init(NULL), // no need to carry pointer, already initialised
    _pattern_init(NULL), // no need to carry pointer, already initialised
    _mutation(c._mutation),
    _parameters(c._parameters),
    _history(NULL) // only original accesses this
{
    this->_network = new Adjacency(*c._network);
    CHECK_CRITICAL_POINTER(this->_network);
    this->_output_pattern = gsl_matrix_calloc(this->_parameters->nodes_out(),
        this->_parameters->nodes_in());
    CHECK_CRITICAL_GSL_POINTER(this->_output_pattern, __FILE__, __LINE__);
    int status = 0; // for gsl return status check
    status = gsl_matrix_memcpy(this->_output_pattern, c._output_pattern); // may not be necessary
    CHECK_CRITICAL_GSL_STATUS(status, __FILE__, __LINE__);
}

/*
 * Destructor
 */
rfn::FlowDistributionNetwork::~FlowDistributionNetwork()
{
    if (this->_original) {
        // only original network should control this memory
        gsl_matrix_free(this->_ideal_pattern);
        delete this->_network;
        History::iterator hist_it;
        for (hist_it = this->_history->begin();
            hist_it != this->_history->end(); ++hist_it)
        {
            delete hist_it->second;
            hist_it->second = NULL;
        }
        delete this->_history;
    }
    if (this->_not_assigned) {
        // network was not assigned so we need to remove this memory
        delete this->_network;
    }
    this->_network = NULL;
    this->_ideal_pattern = NULL;
    this->_history = NULL;
    gsl_matrix_free(this->_output_pattern); // memory under own control
    this->_output_pattern = NULL;
    this->_network_init = NULL; // singleton class destructs at program exit
    this->_pattern_init = NULL; // singleton class destructs at program exit
    this->_mutation = NULL; // singleton class destructs at program exit
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
rfn::FlowDistributionNetwork::operator=(const rfn::FlowDistributionNetwork& a)
{
    if (this != &a) {
        // expect only networks of same type to be assigned
        assert(typeid(*this) == typeid(a));
        // we assigned the other network in this iteration, prevent delete of
        // a._network
        rfn::FlowDistributionNetwork& other = const_cast
            <rfn::FlowDistributionNetwork&>(a);
        other._not_assigned = false;
        this->_changed = a._changed;
        this->_flow_error = a._flow_error;
        delete this->_network;
        this->_network = a._network;
        int status = 0; // for gsl return status check
        status = gsl_matrix_memcpy(this->_output_pattern, a._output_pattern);
        CHECK_CRITICAL_GSL_STATUS(status, __FILE__, __LINE__);
    }
    return *this;
}

/*
 * Virtual assignment operator, all networks following an initial one are copy-
 * constructed during the evolution thus here we only transfer the bare minimum
 * values needed.
 */
rfn::FlowDistributionNetwork&
rfn::FlowDistributionNetwork::operator=(rfn::FlowDistributionNetwork& a)
{
    if (this != &a) {
        // expect only networks of same type to be assigned
        assert(typeid(*this) == typeid(a));
        a._not_assigned = false;
        this->_changed = a._changed;
        this->_flow_error = a._flow_error;
        delete this->_network;
        this->_network = a._network;
        int status = 0; // for gsl return status check
        status = gsl_matrix_memcpy(this->_output_pattern, a._output_pattern);
        CHECK_CRITICAL_GSL_STATUS(status, __FILE__, __LINE__);
    }
    return *this;
}


/*******************************************************************************
 * Member Functions
 ******************************************************************************/


void
rfn::FlowDistributionNetwork::save(const unsigned long time)
{
    History::iterator hist_it(this->_history->end());
    // insert a copy of current state at the end (we progress in time)
    this->_history->insert(hist_it,
        std::pair<unsigned long, rfn::FlowDistributionNetwork*>(time, this->clone()));
}

void
rfn::FlowDistributionNetwork::write_to_binary_stream(bfsys::ofstream& out,
    const unsigned long iteration)
{
    unsigned i = 0;
    unsigned j = 0;
    out.write((char*)&iteration, sizeof(unsigned long));
    i = this->_parameters->seed();
    out.write((char*)&i, sizeof(unsigned));
    double tmp = this->_parameters->connectivity();
    out.write((char*)&tmp, sizeof(double));
    out.write((char*)&this->_flow_error, sizeof(double));
    // saving edges
    // number of edges
    j = (unsigned)boost::num_edges(*this->_network);
    out.write((char*)&j, sizeof(unsigned));
    boost::graph_traits<Adjacency>::out_edge_iterator oe_it, oe_it_end;
    for (i = 0; i < this->_parameters->nodes_end_middle(); ++i) {
        for (boost::tie(oe_it, oe_it_end) = boost::out_edges(i, *this->_network);
            oe_it != oe_it_end; ++oe_it)
        {
            out.write((char*)&i, sizeof(unsigned));
            j = (unsigned)boost::target(*oe_it, *this->_network);
            out.write((char*)&j, sizeof(unsigned));
        }
    }
    // saving ideal output pattern
    double value = 0.0;
    for (i = 0; i < this->_parameters->nodes_out(); ++i) {
        for (j = 0; j < this->_parameters->nodes_in(); ++j) {
            value = gsl_matrix_get(this->_ideal_pattern, i, j);
            out.write((char*)&value, sizeof(double));
        }
    }
}

unsigned long
rfn::FlowDistributionNetwork::read_from_binary_stream(bfsys::ifstream& in)
{
    unsigned long iteration = 0L;
    in.read((char*)&iteration, sizeof(unsigned long));
    unsigned i = 0u;
    in.read((char*)&i, sizeof(unsigned));
    this->_parameters->seed(i);
    double tmp = 0.0;
    in.read((char*)&tmp, sizeof(double));
    this->_parameters->connectivity(tmp);
    in.read((char*)&this->_flow_error, sizeof(double));
    // loading edges
    // number of edges
    unsigned num_edges = 0u;
    in.read((char*)&num_edges, sizeof(unsigned));
    unsigned u = 0u;
    unsigned v = 0u;
    for (i = 0u; i < num_edges; ++i) {
        in.read((char*)&u, sizeof(unsigned));
        in.read((char*)&v, sizeof(unsigned));
        boost::add_edge(u, v, *this->_network);
    }
    // loading ideal output pattern
    for (u = 0; u < this->_parameters->nodes_out(); ++u) {
        for (v = 0; v < this->_parameters->nodes_in(); ++v) {
            in.read((char*)&tmp, sizeof(double));
            gsl_matrix_set(this->_ideal_pattern, u, v, tmp);
        }
    }
    return iteration;
}

void
rfn::FlowDistributionNetwork::store()
{
    History::iterator hist_it;
    std::stringstream filename;
    bfsys::ofstream output;
    for (hist_it = this->_history->begin();
        hist_it != this->_history->end(); ++hist_it)
    {
        filename << this->_network_name << "_" << hist_it->first << ".dat";
        output.open(filename.str(), std::ios::binary);
        hist_it->second->write_to_binary_stream(output, hist_it->first);
        output.close();
        filename.str("");
    }
}

double
rfn::FlowDistributionNetwork::compute_complexity()
{
    int status = 0;
    double sum = 0.0;
    double tmp = 0.0;
    gsl_vector_view first;
    gsl_vector_view second;
    for (short unsigned i = 0; i < this->_parameters->nodes_in() - 1; ++i) {
        for (short unsigned j = i + 1; j < this->_parameters->nodes_in(); ++j)
        {
            first = gsl_matrix_column(this->_ideal_pattern, i);
            second = gsl_matrix_column(this->_ideal_pattern, j);
            status = gsl_blas_ddot (&first.vector, &second.vector, &tmp);
            CHECK_CRITICAL_GSL_STATUS(status, __FILE__, __LINE__);
            sum += tmp;
        }
    }
    return sum;
}

void
rfn::FlowDistributionNetwork::recover()
{
    // recover from error by not allowing this mutation
    // unrealistic output pattern should ensure large flow error
    gsl_matrix_set_all(this->_output_pattern, 2.0);
}

void
rfn::FlowDistributionNetwork::prune()
{
    // make a colour map with all nodes set to white
    std::vector<boost::default_color_type> colours(
        boost::num_vertices(*this->_network),
        boost::color_traits<boost::default_color_type>::white()
        );
    rfn::ColourMap colour_map(colours, *this->_network);
    // get all accessible nodes
    this->accessible_nodes(colour_map);
    // control middle nodes
    for (short unsigned n = this->_parameters->nodes_in();
        n < this->_parameters->nodes_end_middle(); ++n)
    {
        // remove inaccessible nodes
        if (boost::get(colour_map, n) == boost::white_color) {
            boost::clear_vertex(n, *this->_network);
        }
        // remove virtual sources and sinks
        else if (boost::in_degree(n, *this->_network) == 0
            || boost::out_degree(n, *this->_network) == 0)
        {
            boost::clear_vertex(n, *this->_network);
        }
    }
    this->_changed = true;
}


/*******************************************************************************
 * Internal Functions
 ******************************************************************************/


void
rfn::FlowDistributionNetwork::compute_flow_error()
{
    this->_flow_error = 0.0;
    for (short unsigned i = 0; i < this->_parameters->nodes_out(); ++i) {
        for (short unsigned j = 0; j < this->_parameters->nodes_in(); ++j) {
            this->_flow_error += gsl_pow_2(
                gsl_matrix_get(this->_ideal_pattern, (size_t)i, (size_t)j) -
                gsl_matrix_get(this->_output_pattern, (size_t)i, (size_t)j));
        }
    }
    this->_flow_error /= (double)(2 * this->_parameters->nodes_in());
}

void
rfn::FlowDistributionNetwork::compute_output()
{
    rfn::Matrix* sle = gsl_matrix_calloc(this->_parameters->nodes_total(),
        this->_parameters->nodes_total());
    CHECK_CRITICAL_GSL_POINTER(sle, __FILE__, __LINE__);
    this->generate_sle(sle);
    this->generate_output(sle);
    gsl_matrix_free(sle);
}

void
rfn::FlowDistributionNetwork::generate_output(rfn::Matrix* const sle)
{
    gsl_matrix_set_zero(this->_output_pattern);
    // declare other variables
    gsl_vector* x = gsl_vector_calloc(this->_parameters->nodes_total());
    CHECK_CRITICAL_GSL_POINTER(x, __FILE__, __LINE__);
    gsl_permutation* p = gsl_permutation_calloc(
        this->_parameters->nodes_total());
    CHECK_CRITICAL_GSL_POINTER(p, __FILE__, __LINE__);
    gsl_vector_view output = gsl_vector_subvector(x,
        this->_parameters->nodes_end_middle(),
        this->_parameters->nodes_out());
    int signum = 0;
    // decompose matrix
    int status = 0;
    status = gsl_linalg_LU_decomp(sle, p, &signum);
    if (status != 0) {
        throw "Warning: Possibly singular matrix.";
    }
    for (short unsigned n = 0; n < this->_parameters->nodes_in(); ++n) {
        // re-initialise new input node
        status = gsl_vector_set_basis(x, n);
        CHECK_CRITICAL_GSL_STATUS(status, __FILE__, __LINE__);
        status = gsl_linalg_LU_svx(sle, p, x);
        if (status != 0) {
            std::cerr << "GSL Status Error: " << gsl_strerror(status)
                << std::endl;
//            this->print_gsl_matrix(sle);
            std::cerr << "Continuing..." << std::endl;
            break;
        }
        CHECK_CRITICAL_GSL_STATUS(status, __FILE__, __LINE__);
        status = gsl_matrix_set_col(this->_output_pattern, (size_t)n,
            &output.vector);
        CHECK_CRITICAL_GSL_STATUS(status, __FILE__, __LINE__);
    }
    if (status != 0) {
        this->recover();
    }
    gsl_vector_free(x);
    gsl_permutation_free(p);
}

void
rfn::FlowDistributionNetwork::generate_sle(rfn::Matrix* const sle)
{
    // set up the system, where each row 1 * 'i' minus all the incident edges
    // from sources 'j' / out-degree('j')
    boost::graph_traits<rfn::Adjacency>::in_edge_iterator ieit, ieit_end;
    gsl_matrix_set_identity(sle);
    for (short unsigned i = this->_parameters->nodes_in() - 1;
        i < this->_parameters->nodes_total(); ++i)
    {
        for (boost::tie(ieit, ieit_end) = boost::in_edges(i, *(this->_network));
            ieit != ieit_end; ++ieit)
        {
            gsl_matrix_set(sle, i, boost::source(*ieit, *(this->_network)),
                -1.0 / (double)boost::out_degree(
                boost::source(*ieit, *(this->_network)), *(this->_network)));
        }
    }
}

/*
 * Print a gsl matrix to cout in a nice format
 */
void
rfn::FlowDistributionNetwork::print_gsl_matrix(rfn::Matrix* m)
{
    using std::cout;
    using std::endl;
    cout.setf(std::ios::fixed);
    cout.precision(4);
    for (size_t i = 0; i < m->size1; ++i) {
        for (size_t j = 0; j < m->size2; ++j) {
            cout << gsl_matrix_get(m, i, j) << " ";
        }
        cout << endl;
    }
}

/*
 * Generate a map of accessible nodes
 */
void
rfn::FlowDistributionNetwork::accessible_nodes(rfn::ColourMap& colour_map)
{
    // queue for discovered nodes
    boost::queue<rfn::Vertex> queue;
    // bfs visitor
    boost::bfs_visitor<boost::null_visitor> vis;
    
    for (short unsigned n = 0; n < this->_parameters->nodes_in(); ++n) {
        // use breadth first visit which is a bfs that does not initialise the
        // colour map itself
        boost::breadth_first_visit(*this->_network, n, queue, vis,
            colour_map);
    }
}