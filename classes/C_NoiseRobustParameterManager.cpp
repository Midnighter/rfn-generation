/*******************************************************************************
 * filename:    C_NoiseRobustParameterManager.cpp
 * description: Implementation of 'NoiseRobustParameterManager' class
 * author:      Moritz Beber
 * created:     2010-07-21
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


/*******************************************************************************
 * Includes
 ******************************************************************************/


// project
#include "C_NoiseRobustParameterManager.hpp"


/*******************************************************************************
 * Constructors & Destructor
 ******************************************************************************/


/*
 * Default constructor
 */
rfn::NoiseRobustParameterManager::NoiseRobustParameterManager():
    RobustParameterManager(),
    _noise_level(0.0)
{}

/*
 * Destructor
 */
rfn::NoiseRobustParameterManager::~NoiseRobustParameterManager()
{}


/*******************************************************************************
 * Member Functions
 ******************************************************************************/


void
rfn::NoiseRobustParameterManager::write_to_binary_stream(bfsys::ofstream& out)
{
    rfn::RobustParameterManager::write_to_binary_stream(out);
    out.write((char*)&this->_noise_level, sizeof(double));
}

void
rfn::NoiseRobustParameterManager::read_from_binary_stream(bfsys::ifstream& in)
{
    rfn::RobustParameterManager::read_from_binary_stream(in);
    in.read((char*)&this->_noise_level, sizeof(double));
}
