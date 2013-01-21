/*******************************************************************************
 * filename:    C_NetworkBuilder.cpp
 * description: Implementation of 'NetworkBuilder' class
 * author:      Moritz Beber
 * created:     2010-04-16
 * copyright:   Jacobs University Bremen. All rights reserved.
 ******************************************************************************/


/*******************************************************************************
 * Includes
 ******************************************************************************/


// std
#include <iostream>

// project
#include "C_NetworkBuilder.hpp"
#include "C_NodeRobustNetwork.hpp"
#include "C_LinkRobustNetwork.hpp"
#include "C_NoiseRobustNetwork.hpp"


/*******************************************************************************
 * Necessary Singleton Pattern Definitions
 ******************************************************************************/


DEFINE_BASE_SINGLETON(rfn::NetworkBuilder)


/*******************************************************************************
 * Constructor & Destructor
 ******************************************************************************/


/*
 * Default Constructor
 */
rfn::NetworkBuilder::NetworkBuilder()
{}

/*
 * Destructor
 */
rfn::NetworkBuilder::~NetworkBuilder()
{}


/*******************************************************************************
 * Member Functions
 ******************************************************************************/


rfn::NetworkBuilder*
rfn::NetworkBuilder::instance()
{
    if (rfn::NetworkBuilder::_instance == NULL) {
        static rfn::NetworkBuilder::Guard g;
        rfn::NetworkBuilder::_instance = new rfn::NetworkBuilder;
    }
    return rfn::NetworkBuilder::_instance;
}

rfn::FlowDistributionNetwork*
rfn::NetworkBuilder::build(ParameterManager* const params, std::string name)
{
    rfn::FlowDistributionNetwork* rf_net = NULL;
    switch(params->network_type()) {
        // TODO: implement other schemes in construction
        case rfn::flow_evolved:
            rf_net = new rfn::FlowDistributionNetwork();
            break;
        case rfn::node_robust:
            rf_net = new rfn::NodeRobustNetwork();
            break;
        case rfn::link_robust:
            rf_net = new rfn::LinkRobustNetwork();
            break;
        case rfn::noise_robust:
            rf_net = new rfn::NoiseRobustNetwork();
            break;
        default:
            std::cerr << "Warning: No network type chosen." << std::endl;
//            this->_network = new RobustNetwork;
            break;
    }
    rf_net->name(name);
    return rf_net;
}