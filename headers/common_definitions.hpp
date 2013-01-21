/*******************************************************************************
 * filename:    common_definitions.hpp
 * description: Some commonly used and exchangeable aliases and typedefs
 * author:      Moritz Beber
 * created:     2010-04-12
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 * Of course, the appropriate headers/libraries need to be included/linked in
 * any project using this header.
 ******************************************************************************/


#ifndef COMMON_DEFINITIONS_HPP
#define	COMMON_DEFINITIONS_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// std
#include <cstdio>
#include <cerrno>
#include <vector>

// boost
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include <boost/filesystem.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/property_maps/container_property_map.hpp>
#include <boost/random/lagged_fibonacci.hpp>
#include <boost/program_options.hpp>

// gsl
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_errno.h>


/*******************************************************************************
 * Namespace Aliases
 ******************************************************************************/


//namespace bblas = boost::numeric::ublas;
namespace bacc = boost::accumulators;
namespace bfsys = boost::filesystem;
namespace bpo = boost::program_options;


/*******************************************************************************
 * Typedefs
 ******************************************************************************/


namespace rfn {

//    typedef boost::adjacency_list<boost::setS, boost::vecS, boost::bidirectionalS,
//        boost::no_property, boost::no_property, boost::no_property, boost::listS>
//        adj_list;
    typedef boost::adjacency_matrix<boost::directedS> Adjacency;
    typedef boost::graph_traits<Adjacency>::vertex_descriptor Vertex;
    typedef boost::container_property_map<Adjacency, Vertex,
        std::vector<boost::default_color_type> > ColourMap;
    typedef boost::lagged_fibonacci607 RandGen;
    typedef bacc::accumulator_set<double, bacc::stats<bacc::tag::mean,
        bacc::tag::variance> > Statistics;
    typedef gsl_matrix Matrix;
//    typedef flens::SparseGeMatrix<flens::CRS<double> > fsmatrix;
//    typedef flens::GeMatrix<flens::FullStorage<double, flens::ColMajor> > fdmatrix;
    
} // namespace rfn

/*******************************************************************************
 * Common Variables
 ******************************************************************************/


namespace rfn {
    
    enum FunctionalNetworkScheme {
        flow_evolved,
        link_robust,
        node_robust,
        noise_robust
    };

    enum MutationScheme {
        link_mutation,
        path_mutation
    };

    enum EssentialityScheme {
        node_essentiality,
        link_essentiality
    };

    enum PatternInitScheme {
        random_k,
        complexity,
        high_complexity,
        low_complexity,
        equal_complexity,
        equal_spread_complexity
    };

    enum NetworkInitScheme {
        random,
        list
    };

} // namespace rfn


/*******************************************************************************
 * Common Macros
 ******************************************************************************/


#ifdef VERBOSE
    #define CHECK_CRITICAL_GSL_STATUS(STATUS, FILENAME, LINE)                  \
        if (STATUS) {                                                          \
            std::cerr << "GSL Status Error: " << gsl_strerror(STATUS) << " in "\
                << FILENAME << ":" << LINE << std::endl;                       \
            std::exit(STATUS);                                                 \
        }
#else
    #define CHECK_CRITICAL_GSL_STATUS(STATUS, FILENAME, LINE)                  \
        if (STATUS) {                                                          \
            std::exit(STATUS);                                                 \
        }
#endif // VERBOSE

#ifdef VERBOSE
    #define CHECK_CRITICAL_GSL_POINTER(POINTER, FILENAME, LINE)                \
        if (POINTER == NULL) {                                                 \
            std::cerr << "GSL Pointer Error: " << gsl_strerror(GSL_ENOMEM)     \
                << " in " << FILENAME << ":" << LINE << std::endl;             \
        std::exit(GSL_ENOMEM);                                                 \
    }
#else
    #define CHECK_CRITICAL_GSL_POINTER(POINTER, FILENAME, LINE)                \
        if (POINTER == NULL) {                                                 \
        std::exit(GSL_ENOMEM);                                                 \
    }
#endif // VERBOSE

#define CHECK_CRITICAL_POINTER(POINTER)                                        \
    if (POINTER == NULL) {                                                     \
        std::perror(NULL);                                                     \
        std::exit(errno);                                                      \
    }

#define GSL_EPSILON std::numeric_limits<double>::epsilon()


#endif	// COMMON_DEFINITIONS_HPP

