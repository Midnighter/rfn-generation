/*******************************************************************************
 * filename:    C_RobustParameterManager.cpp
 * description: Implementation of 'RobustParameterManager' class
 * author:      Moritz Beber
 * created:     2010-07-02
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


/*******************************************************************************
 * Includes
 ******************************************************************************/


// project
#include "C_RobustParameterManager.hpp"


/*******************************************************************************
 * Constructors & Destructor
 ******************************************************************************/


/*
 * Default constructor
 */
rfn::RobustParameterManager::RobustParameterManager():
    ParameterManager(),
    _min_robustness(0.0),
    _robust_threshold(0.0),
    _robust_factor(0.0)
{}

/*
 * Destructor
 */
rfn::RobustParameterManager::~RobustParameterManager()
{}


/*******************************************************************************
 * Member Functions
 ******************************************************************************/


void
rfn::RobustParameterManager::write_to_binary_stream(bfsys::ofstream& out)
{
    rfn::ParameterManager::write_to_binary_stream(out);
    out.write((char*)&this->_min_robustness, sizeof(double));
    out.write((char*)&this->_robust_threshold, sizeof(double));
    out.write((char*)&this->_robust_factor, sizeof(double));
}

void
rfn::RobustParameterManager::read_from_binary_stream(bfsys::ifstream& in)
{
    rfn::ParameterManager::read_from_binary_stream(in);
    in.read((char*)&this->_min_robustness, sizeof(double));
    in.read((char*)&this->_robust_threshold, sizeof(double));
    in.read((char*)&this->_robust_factor, sizeof(double));
}
