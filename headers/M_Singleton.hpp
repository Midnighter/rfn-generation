/*******************************************************************************
 * filename:    M_Singleton.hpp
 * description: Macros that generate skeleton singleton pattern classes
 * author:      Moritz Beber
 * created:     2010-06-08
 * copyright:   Jacobs University Bremen. All rights reserved.
 *******************************************************************************
 * Three macros are provided that provide the skeleton for classes that implement
 * the singleton pattern. Importantly, the base class is inheritable. The design
 * is such that only one instance of the base class or child classes can exist.
 *
 * The instance is dynamically allocated on the heap and will only be destroyed
 * once the program exits.
 *
 * The 'instance' member function of the base class should be overloaded so that
 * child classes are dynamically created through it. It can do so because it is
 * declared a 'friend' in all derived classes.
 *
 * The copy-constructor and assignment operator are unavailable for classes that
 * use the singleton pattern.
 *
 * The default (and any other) constructor(s) and the destructor should at least
 * be defined as empty in any implementation.
 ******************************************************************************/


#ifndef _M_SINGLETON_HPP
#define	_M_SINGLETON_HPP


/*
 * Necessary declarations for creating a base class implementing the singleton
 * pattern.
 */
#define DECLARE_BASE_SINGLETON(CLASS)                                          \
    protected:                                                                 \
        CLASS();                                                               \
        virtual ~CLASS();                                                      \
                                                                               \
    private:                                                                   \
        static CLASS* _instance;                                               \
        CLASS(const CLASS&);                                                   \
        CLASS& operator=(const CLASS&);                                        \
        CLASS& operator=(CLASS&);                                              \
                                                                               \
        class Guard {                                                          \
            public:                                                            \
                ~Guard();                                                      \
        };                                                                     \
        friend class Guard;                                                    \
                                                                               \
    public:                                                                    \
        static CLASS* instance();


/*
 * Necessary definitions of base class member functions. The default 'instance'
 * member function should be overloaded if child classes are used. If 'CLASS'
 * resides within one or more namespaces you should invoke this macro with the
 * full specifier or employ the appropriate 'using' directives.
 */
#define DEFINE_BASE_SINGLETON(CLASS)                                           \
    CLASS* CLASS::_instance = NULL;                                            \
                                                                               \
    CLASS::Guard::~Guard()                                                     \
    {                                                                          \
        if (CLASS::_instance) {                                                \
            delete CLASS::_instance;                                           \
            CLASS::_instance = NULL;                                           \
        }                                                                      \
    }

/*
 * 'static CLASS* instance()' is not defined in this macro in case the base
 * is supposed to be 'pure virtual'. An example is given here:
 */

/*
CLASS*
CLASS::instance()
{
    if (CLASS::_instance == 0) {
        static CLASS::Guard g;
        CLASS::_instance = new CLASS;
    }
    return CLASS::_instance;
}
*/


/*
 * Necessary declarations for inheriting from a base singleton pattern class. If
 * 'BASE' resides within one or more namespaces you should invoke this macro with
 * the full specifier or employ the appropriate 'using' directives.
 */
#define DECLARE_DERIVED_SINGLETON(BASE, DERIVED)                               \
    friend class BASE;                                                         \
                                                                               \
    protected:                                                                 \
        DERIVED();                                                             \
        virtual ~DERIVED();                                                    \
                                                                               \
    private:                                                                   \
        DERIVED(const DERIVED&);                                               \
        DERIVED& operator=(const DERIVED&);                                    \
        DERIVED& operator=(DERIVED&);


#endif	// _M_SINGLETON_HPP

