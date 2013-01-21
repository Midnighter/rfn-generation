/*******************************************************************************
 * filename:    C_RobustParameterManager.hpp
 * description: Derived class for holding additional parameters
 * author:      Moritz Beber
 * created:     2010-07-02
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 *
 ******************************************************************************/


#ifndef _C_ROBUSTPARAMETERMANAGER_HPP
#define	_C_ROBUSTPARAMETERMANAGER_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// project
#include "C_ParameterManager.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace rfn {

    class RobustParameterManager: public ParameterManager {
        /* Derived From Singleton Pattern */
        DECLARE_DERIVED_SINGLETON(ParameterManager, RobustParameterManager)

    protected:
        /* Data Members */
        double _min_robustness;
        double _robust_threshold;
        double _robust_factor;

    public:
        /* Member Functions */
        // save this object to a binary stream
        virtual void write_to_binary_stream(bfsys::ofstream& out);
        // read this object from a binary stream
        virtual void read_from_binary_stream(bfsys::ifstream& in);
        // _min_robustness get method
        double min_robustness() const
        {
            return this->_min_robustness;
        }
        // _min_robustness set method
        void min_robustness(const double r)
        {
            this->_min_robustness = r;
        }
        // _robust_threshold get method
        double robust_threshold() const
        {
            return this->_robust_threshold;
        }
        // _robust_threshold set method
        void robust_threshold(const double t)
        {
            this->_robust_threshold = t;
        }
        // _robust_factor get method
        double robust_factor() const
        {
            return this->_robust_factor;
        }
        // _robust_factor set method
        void robust_factor(const double f)
        {
            this->_robust_factor = f;
        }

    }; // class RobustParameterManager

} // namespace rfn


#endif	// _C_ROBUSTPARAMETERMANAGER_HPP

