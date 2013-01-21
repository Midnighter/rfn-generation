/*******************************************************************************
 * filename:    C_ParameterManager.hpp
 * description: Singleton class that stores system parameter values
 * author:      Moritz Beber
 * created:     2010-04-12
 * copyright:   Jacobs University Bremen. All rights reserved.
 ******************************************************************************/


#ifndef _C_PARAMETERMANAGER_HPP
#define	_C_PARAMETERMANAGER_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// std
#include <string>

// boost
#include <boost/filesystem/fstream.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>


// project
#include "common_definitions.hpp"
#include "M_Singleton.hpp"
#include "C_NetworkInitialiser.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace rfn {

    class ParameterManager {
        /* Singleton Pattern Declaration */
        DECLARE_BASE_SINGLETON(ParameterManager)

    private:
        /* Data Members */
        unsigned _seed;
        short unsigned _nodes_in;
        short unsigned _nodes_middle;
        short unsigned _nodes_out;
        unsigned _nodes_end_middle;
        unsigned _nodes_total;
        short unsigned _activated_k;
        double _error_threshold;
        double _annealing_factor;
        unsigned long _max_time;
        short unsigned _snapshots;
        double _connectivity;
        double _add_mutation;
        double _lower_bound;
        double _upper_bound;
        FunctionalNetworkScheme _network_type;
        NetworkInitScheme _network_init;
        PatternInitScheme _pattern_init;
        MutationScheme _mutation_type;
        static RandGen _rng;

    public:
        /* Member Functions */
        // _instance retrieval method
        static ParameterManager* instance(const FunctionalNetworkScheme scheme);
        // save this object to a binary stream
        virtual void write_to_binary_stream(bfsys::ofstream& out);
        // load this object from a binary stream
        virtual void read_from_binary_stream(bfsys::ifstream& in);
        // ascertain legal network structure
        bool regards_structure(const short unsigned src,
            const short unsigned tar) const;
        // _seed get method
        unsigned seed() const
        {
            return this->_seed;
        }
        // _seed set method
        void seed(const unsigned s)
        {
            this->_seed = s;
            ParameterManager::_rng.seed(s);
        }
        // _nodes_in get method
        short unsigned nodes_in() const
        {
            return this->_nodes_in;
        }
        // _nodes_in set method
        void nodes_in(const short unsigned n)
        {
            this->_nodes_in = n;
            recompute_end();
            recompute_total();
        }
        // _nodes_middle get method
        short unsigned nodes_middle() const
        {
            return this->_nodes_middle;
        }
        // _nodes_middle set method
        void nodes_middle(const short unsigned n)
        {
            this->_nodes_middle = n;
            recompute_end();
            recompute_total();
        }
        // _nodes_out get method
        short unsigned nodes_out() const
        {
            return this->_nodes_out;
        }
        // _nodes_out set method
        void nodes_out(const short unsigned n)
        {
            this->_nodes_out = n;
            recompute_total();
        }
        // _nodes_end_middle get method
        unsigned nodes_end_middle() const
        {
            return this->_nodes_end_middle;
        }
        // _nodes_total get method
        unsigned nodes_total() const
        {
            return this->_nodes_total;
        }
        // _activated_k get method
        short unsigned activated_k() const
        {
            return this->_activated_k;
        }
        // _activated_k set method
        void activated_k(const short unsigned k)
        {
            this->_activated_k = k;
        }
        // _error_threshold get method
        double error_threshold() const
        {
            return this->_error_threshold;
        }
        // _error_threshold set method
        void error_threshold(const double e)
        {
            this->_error_threshold = e;
        }
        // _annealing_factor get method
        double annealing_factor() const
        {
            return this->_annealing_factor;
        }
        // _annealing_factor set method
        void annealing_factor(const double t)
        {
            this->_annealing_factor = t;
        }
        // _max_time get method
        unsigned long max_time() const
        {
            return this->_max_time;
        }
        // _max_time set method
        void max_time(const unsigned long t)
        {
            this->_max_time = t;
        }
        // _snapshots get method
        short unsigned snapshots() const
        {
            return this->_snapshots;
        }
        // _snapshots set method
        void snapshots(const short unsigned t)
        {
            this->_snapshots = t;
        }
        // _connectivity get method
        double connectivity() const
        {
            return this->_connectivity;
        }
        // _connectivity set method
        void connectivity(const double c)
        {
            this->_connectivity = c;
        }
        // _add_mutation get method
        double add_mutation() const
        {
            return this->_add_mutation;
        }
        // _add_mutation set method
        void add_mutation(const double p)
        {
            this->_add_mutation = p;
        }
        // _lower_bound get method
        double lower_bound() const
        {
            return this->_lower_bound;
        }
        // _lower_bound set method
        void lower_bound(const double b)
        {
            this->_lower_bound = b;
        }
        // _upper_bound get method
        double upper_bound() const
        {
            return this->_upper_bound;
        }
        // _upper_bound set method
        void upper_bound(const double b)
        {
            this->_upper_bound = b;
        }
        // random number generator retrieval
        static RandGen& rng()
        {
            return ParameterManager::_rng;
        }
        // _network_type get method
        FunctionalNetworkScheme network_type() const
        {
            return this->_network_type;
        }
//        // _network_type set method
//        void network_type(const FunctionalNetworkScheme s)
//        {
//            this->_network_type = s;
//        }
        // _network_init get method
        NetworkInitScheme network_init() const
        {
            return this->_network_init;
        }
        // _network_init set method
        void network_init(const NetworkInitScheme s)
        {
            this->_network_init = s;
        }
        // _pattern_init get method
        PatternInitScheme pattern_init() const
        {
            return this->_pattern_init;
        }
        // _pattern_init set method
        void pattern_init(const PatternInitScheme s)
        {
            this->_pattern_init = s;
        }
        // _mutation_type get method
        MutationScheme mutation_type() const
        {
            return this->_mutation_type;
        }
        // _mutation_type set method
        void mutation_type(const MutationScheme s)
        {
            this->_mutation_type = s;
        }
        // check whether a node belongs to the input layer
        bool is_input(const short unsigned n) const
        {
            return (n < this->_nodes_in);
        }
        // check whether a node belongs to the middle layer
        bool is_middle(const unsigned n) const
        {
            return (n >= this->_nodes_in && n < this->_nodes_end_middle);
        }
        // check whether a node belongs to the output layer
        bool is_output(const unsigned n) const
        {
            return (n >= this->_nodes_end_middle && n < this->_nodes_total);
        }

    protected:
        /* Internal Functions */
        // find the type of network
        FunctionalNetworkScheme find_network_type(short unsigned foo);
        // find the initialisation of network
        NetworkInitScheme find_network_init(const short unsigned type);
        // find the type of network
        PatternInitScheme find_pattern_init(const short unsigned type);
        // find the type of network
        MutationScheme find_mutation_type(const short unsigned type);
        // recompute node index boundary
        void recompute_end()
        {
            this->_nodes_end_middle = this->_nodes_in + this->_nodes_middle;
        }
        // recompute total node index
        void recompute_total()
        {
            this->_nodes_total = this->_nodes_end_middle + this->_nodes_out;
        }

    }; // class ParameterManager

} // namespace rfn


#endif	// _C_PARAMETERMANAGER_HPP

