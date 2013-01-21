/*******************************************************************************
 * filename:    CMatrix.hpp
 * description: Adapter class for a unified matrix interface
 * author:      Moritz Beber
 * created:     2010-04-12
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 * Currently will work with a typedef, an adapter class is too much work at the
 * moment and of questionable performance due to 'virtual' mechanism.
 * Nevertheless, a skeletton is provided here.
 ******************************************************************************/

#ifndef _CMATRIX_HPP
#define	_CMATRIX_HPP


/*******************************************************************************
 * Includes
 ******************************************************************************/


// project
#include "../headers/common_definitions.hpp"


/*******************************************************************************
 * Declarations
 ******************************************************************************/


namespace meb {

    template<class D>
    class Matrix {
        public:
            // get method
            virtual D& operator()(unsigned i, unsigned j) = 0;
            // set method
            virtual void operator()(unsigned i, unsigned j) = 0;
            // maybe matrix views
    }; // class Matrix

} // namespace meb

#endif	// _CMATRIX_HPP

