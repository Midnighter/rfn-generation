/*******************************************************************************
 * filename:    C_ParameterManager.cpp
 * description: Implementation of 'ParameterManager' class
 * author:      Moritz Beber
 * created:     2010-04-12
 * copyright:   Jacobs University Bremen. All rights reserved.
 ******************************************************************************/


/*******************************************************************************
 * Includes
 ******************************************************************************/


// std
#include <iostream>

// project
#include "C_ParameterManager.hpp"
#include "C_RobustParameterManager.hpp"
#include "C_NoiseRobustParameterManager.hpp"
#include "rfn_config.hpp"


/*******************************************************************************
 * Static Variable Initialisation
 ******************************************************************************/


rfn::RandGen rfn::ParameterManager::_rng;


/*******************************************************************************
 * Necessary Singleton Pattern Definitions
 ******************************************************************************/


DEFINE_BASE_SINGLETON(rfn::ParameterManager)


/*******************************************************************************
 * Constructor & Destructor
 ******************************************************************************/


/*
 * Default constructor
 */
rfn::ParameterManager::ParameterManager():
    _seed(0),
    _nodes_in(0),
    _nodes_middle(0),
    _nodes_out(0),
    _nodes_end_middle(0),
    _nodes_total(0),
    _activated_k(0),
    _error_threshold(0.0),
    _annealing_factor(0.0),
    _max_time(0L),
    _snapshots(0u),
    _connectivity(0.0),
    _add_mutation(0.0),
    _lower_bound(0.0),
    _upper_bound(0.0),
    _network_type(rfn::flow_evolved),
    _network_init(rfn::random),
    _pattern_init(rfn::random_k),
    _mutation_type(rfn::path_mutation)
{}

/*
 * Destructor
 */
rfn::ParameterManager::~ParameterManager()
{}


/*******************************************************************************
 * Member Functions
 ******************************************************************************/


/*
 * Default instance generation for singleton
 */
rfn::ParameterManager*
rfn::ParameterManager::instance()
{
    if (rfn::ParameterManager::_instance == NULL) {
        std::cerr << "Warning: Instance not generated yet." << std::endl;
    }
    return rfn::ParameterManager::_instance;
}

/*
 * Instance generation for singleton
 */
rfn::ParameterManager*
rfn::ParameterManager::instance(const rfn::FunctionalNetworkScheme scheme)
{
    if (rfn::ParameterManager::_instance == NULL) {
        static rfn::ParameterManager::Guard g;
        switch (scheme) {
            case rfn::flow_evolved:
                rfn::ParameterManager::_instance = new rfn::ParameterManager();
                break;
            case rfn::noise_robust:
                ParameterManager::_instance = new NoiseRobustParameterManager();
                break;
            default:
                ParameterManager::_instance = new RobustParameterManager();
                break;
        }
        rfn::ParameterManager::_instance->_network_type = scheme;
    }
    return rfn::ParameterManager::_instance;
}

void
rfn::ParameterManager::write_to_binary_stream(bfsys::ofstream& out)
{
    short unsigned version = RFN5_VERSION_MAJOR;
    out.write((char*)&version, sizeof(short unsigned));
    out.write((char*)&this->_nodes_in, sizeof(short unsigned));
    out.write((char*)&this->_nodes_middle, sizeof(short unsigned));
    out.write((char*)&this->_nodes_out, sizeof(short unsigned));
    out.write((char*)&this->_activated_k, sizeof(short unsigned));
    out.write((char*)&this->_error_threshold, sizeof(double));
    out.write((char*)&this->_annealing_factor, sizeof(double));
    out.write((char*)&this->_error_threshold, sizeof(unsigned long));
    out.write((char*)&this->_snapshots, sizeof(short unsigned));
    out.write((char*)&this->_connectivity, sizeof(double));
    out.write((char*)&this->_add_mutation, sizeof(double));
    version = this->_network_type;
    out.write((char*)&version, sizeof(short unsigned));
    version = this->_network_init;
    out.write((char*)&version, sizeof(short unsigned));
    version = this->_pattern_init;
    out.write((char*)&version, sizeof(short unsigned));
    version = this->_mutation_type;
    out.write((char*)&version, sizeof(short unsigned));
}

void
rfn::ParameterManager::read_from_binary_stream(bfsys::ifstream& in)
{
    short unsigned version = 0u;
    in.read((char*)&version, sizeof(short unsigned));
    in.read((char*)&this->_nodes_in, sizeof(short unsigned));
    in.read((char*)&this->_nodes_middle, sizeof(short unsigned));
    in.read((char*)&this->_nodes_out, sizeof(short unsigned));
    in.read((char*)&this->_activated_k, sizeof(short unsigned));
    in.read((char*)&this->_error_threshold, sizeof(double));
    in.read((char*)&this->_annealing_factor, sizeof(double));
    in.read((char*)&this->_error_threshold, sizeof(unsigned long));
    in.read((char*)&this->_snapshots, sizeof(short unsigned));
    in.read((char*)&this->_connectivity, sizeof(double));
    in.read((char*)&this->_add_mutation, sizeof(double));
//    in.read((char*)&version, sizeof(short unsigned));
//    this->_network_type = this->find_network_type(version);
//    in.read((char*)&version, sizeof(short unsigned));
//    this->_network_init = this->find_network_init(version);
//    in.read((char*)&version, sizeof(short unsigned));
//    this->_pattern_init = this->find_pattern_init(version);
//    in.read((char*)&version, sizeof(short unsigned));
//    this->_mutation_type = this->find_mutation_type(version);
}

bool
rfn::ParameterManager::regards_structure(const short unsigned src,
    const short unsigned tar) const
{
    if (src == tar) {
        return false;
    }
    else if (this->is_input(src) && this->is_middle(tar)) {
        return true;
    }
    else if (this->is_middle(src) && this->is_middle(tar)) {
        return true;
    }
    else if (this->is_middle(src) && this->is_output(tar)) {
        return true;
    }
    else {
        return false;
    }
}


/*******************************************************************************
 * Internal Functions
 ******************************************************************************/


//rfn::FunctionalNetworkScheme
//rfn::ParameterManager::find_network_type(short unsigned foo)
//{
//    switch (foo) {
//        case (foo == rfn::flow_evolved):
//            return rfn::flow_evolved;
//        case (foo == rfn::link_robust):
//            return rfn::link_robust;
//        case (foo == rfn::node_robust):
//            return rfn::node_robust;
//        case (foo == rfn::noise_robust):
//            return rfn::noise_robust;
//    }
//}
//
//rfn::NetworkInitScheme
//rfn::ParameterManager::find_network_init(const short unsigned type)
//{
//    switch (type) {
//        case (type == (short unsigned)rfn::random):
//            return rfn::random;
//        case (type == (short unsigned)rfn::list):
//            return rfn::list;
//    }
//}
//
//rfn::PatternInitScheme
//rfn::ParameterManager::find_pattern_init(const short unsigned type)
//{
//    switch (type) {
//        case (type == (short unsigned)rfn::random_k):
//            return rfn::random_k;
//        case (type == (short unsigned)rfn::complexity):
//            return rfn::complexity;
//    }
//}
//
//rfn::MutationScheme
//rfn::ParameterManager::find_mutation_type(const short unsigned type)
//{
//    switch (type) {
//        case (type == (short unsigned)rfn::link_mutation):
//            return rfn::link_mutation;
//        case (type == (short unsigned)rfn::path_mutation):
//            return rfn::path_mutation;
//    }
//}
