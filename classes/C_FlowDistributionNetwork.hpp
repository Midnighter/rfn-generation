/*******************************************************************************
 * filename:    C_FlowDistributionNetwork.hpp
 * description: Base class for flow distribution networks
 * author:      Moritz Beber
 * created:     2010-04-12
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 * This class models the basic flow distribution networks discussed in Pablo F.
 * Kaluza's PhD thesis titled "Evolutionary Engineering of Complex Functional
 * Networks".
 ******************************************************************************/


#ifndef _C_FLOWDISTRIBUTIONNETWORK_HPP
#define	_C_FLOWDISTRIBUTIONNETWORK_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// std
#include <map>

// project
#include "common_definitions.hpp"
#include "C_ParameterManager.hpp"
#include "C_NetworkInitialiser.hpp"
#include "C_PatternInitialiser.hpp"
#include "C_Mutation.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace rfn {

    class FlowDistributionNetwork {

    protected:
        typedef std::map<unsigned long, FlowDistributionNetwork*> History;
        typedef std::pair<History::iterator, bool> HistoryInsertReturn;
        /* Data Members */
        bool _original;
        bool _not_assigned;
        bool _changed;
        double _flow_error;
        std::string _network_name;
        Adjacency* _network;
        Matrix* _ideal_pattern;
        Matrix* _output_pattern;
        NetworkInitialiser* _network_init;
        PatternInitialiser* _pattern_init;
        Mutation* _mutation;
        ParameterManager* _parameters;
        History* _history;

    public:
        /* Constructors & Destructor */
        FlowDistributionNetwork();
        FlowDistributionNetwork(const FlowDistributionNetwork& c);
        virtual ~FlowDistributionNetwork();
        // polymorphic "copy constructor"
        virtual FlowDistributionNetwork* clone()
        {
            return new FlowDistributionNetwork(*this);
        }

        /* Operators */
        virtual FlowDistributionNetwork& operator=(const FlowDistributionNetwork& a);
        virtual FlowDistributionNetwork& operator=(FlowDistributionNetwork& a);

        /* Member Functions */
        // store all networks in history to files
        void store();
        // save this object to a binary stream
        virtual void write_to_binary_stream(bfsys::ofstream& out,
            const unsigned long iteration);
        // read this object from a binary stream
        virtual unsigned long read_from_binary_stream(bfsys::ifstream& in);
        // record the current network status
        void save(const unsigned long time);
        // change the network
        virtual void mutate()
        {
            this->_changed = true;
            this->_mutation->mutate(this->_network);
        }
        // compute complexity of the output pattern
        double compute_complexity();
        // recover from a bad connectivity
        void recover();
        // remove superfluous nodes
        virtual void prune();
        // get number of nodes
        unsigned int num_nodes() const
        {
            return (unsigned int)boost::num_vertices(*this->_network);
        }
        // get number of links
        unsigned int num_links() const
        {
            return (unsigned int)boost::num_edges(*this->_network);
        }
        // difference between actual and ideal output pattern
        double flow_error()
        {
            if (this->_changed) {
                this->compute_output();
                this->compute_flow_error();
                this->_changed = false;
            }
            return this->_flow_error;
        }
        // _network_name get method
        std::string name() const
        {
            return this->_network_name;
        }
        // _network_name set method
        void name(const std::string n)
        {
            this->_network_name = n;
        }
        // _ideal_pattern get method
        Matrix const* ideal_pattern() const
        {
            return this->_ideal_pattern;
        }
        // _network get method
        Adjacency const* network() const
        {
            return this->_network;
        }
        History* history()
        {
            return this->_history;
        }

    protected:
        /* Internal Functions */
        void compute_flow_error();
        void compute_output();
        void generate_output(Matrix* const sle);
        void generate_sle(Matrix* const sle);
        void print_gsl_matrix(Matrix* const m);
        void accessible_nodes(ColourMap& colour_map);

    }; // class FlowDistributionNetwork

} // namespace rfn


#endif	// _C_FLOWDISTRIBUTIONNETWORK_HPP

