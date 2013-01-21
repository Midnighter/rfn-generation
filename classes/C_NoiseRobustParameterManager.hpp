/*******************************************************************************
 * filename:    C_NoiseRobustParameterManager.hpp
 * description: Derived class for holding noise robust parameters
 * author:      Moritz Beber
 * created:     2010-07-21
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


#ifndef _C_NOISEROBUSTPARAMETERMANAGER_HPP
#define	_C_NOISEROBUSTPARAMETERMANAGER_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// project
#include "C_ParameterManager.hpp"
#include "C_RobustParameterManager.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace rfn {

    class NoiseRobustParameterManager: public RobustParameterManager {
        /* Derived From Singleton Pattern */
        DECLARE_DERIVED_SINGLETON(RobustParameterManager,
            NoiseRobustParameterManager)
        friend class ParameterManager;

    protected:
        /* Data Members */
        double _noise_level;

    public:
        /* Member Functions */
        // save this object to a binary stream
        virtual void write_to_binary_stream(bfsys::ofstream& out);
        // load this object from a binary stream
        virtual void read_from_binary_stream(bfsys::ifstream& in);
        // _noise_level get method
        double noise_level() const
        {
            return this->_noise_level;
        }
        // _noise_level set method
        void noise_level(const double w)
        {
            this->_noise_level = w;
        }

    }; // class NoiseRobustParameterManager

} // namespace rfn


#endif	// _C_NOISEROBUSTPARAMETERMANAGER_HPP

