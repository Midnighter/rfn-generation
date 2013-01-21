CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

###############################################################################
# Edit this section to suit your needs
###############################################################################

# adapt these paths to your system
SET(BOOST_LIB_PATH ~/libraries/boost/lib)
SET(BOOST_INCLUDE_PATH ~/libraries/boost/include)
SET(GSL_LIB_PATH /usr/lib64)
SET(GSL_INCLUDE_PATH /usr/include)

# if you want to install with "make install" use either:
# make install -DESTDIR=/your/path
# or edit the SET command from "" to "/your/path" and then simply make install
# remember to add /your/path/lib to your LD_LIBRARY_PATH
SET(CMAKE_INSTALL_PREFIX "")
MESSAGE("Installation directory='${CMAKE_INSTALL_PREFIX}'")

###############################################################################
# The following section should be left alone unless you know your stuff
###############################################################################

# project description
PROJECT(RFN5)

# project version
SET(RFN5_VERSION_MAJOR 5)
SET(RFN5_VERSION_MINOR 3)
SET(RFN5_VERSION_PATCH 1)

# generate configuration file
CONFIGURE_FILE("rfn_config.hpp.in" "headers/rfn_config.hpp" @ONLY)


# define used macros
MACRO(CREATE_EXECUTABLE NAME SOURCES LIBRARIES)
    ADD_EXECUTABLE(${NAME} "${SOURCES}")
    TARGET_LINK_LIBRARIES(${NAME} ${LIBRARIES})
    SET_TARGET_PROPERTIES("rfn_classes" PROPERTIES RUNTIME_OUTPUT_DIRECTORY
        "${RFN5_SOURCE_DIR}/bin")
    GET_TARGET_PROPERTY(INSTALL ${NAME} RUNTIME_OUTPUT_DIRECTORY)
#    MESSAGE(STATUS "Building executable " "'${NAME}'" " in location "
#        "'${INSTALL}'")
ENDMACRO(CREATE_EXECUTABLE)

# compiler options
# in order to turn off debugging and choose optimisations run
# cmake -DDEBUG=OFF .
OPTION(DEBUG "Build the project using debugging code" ON)

IF(DEBUG)
    MESSAGE(STATUS "Building in debug mode.")
    ADD_DEFINITIONS(-Wall -Wextra -Wno-deprecated -ggdb3 -ansi
        -DVERBOSE)
ELSE(DEBUG)
    MESSAGE(STATUS "Building in optimisation mode.")
    ADD_DEFINITIONS(-w -O3 -DNDEBUG -DHAVE_INLINE -ansi)
ENDIF(DEBUG)

# install path use cmake -DLOCATION=path/to/install
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RFN5_SOURCE_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${RFN5_SOURCE_DIR}/lib)

# define project properties
#AUX_SOURCE_DIRECTORY("source" GRAPH_SOURCES)
SET(BUILD_FILES "simulation")
SET(GSL_LIBS gsl gslcblas)
SET(BOOST_LIBS boost_system boost_filesystem boost_program_options boost_graph)
SET(COMMON_LIBS "rfn_classes" "${GSL_LIBS}" "${BOOST_LIBS}")
INCLUDE_DIRECTORIES("headers" "classes" ${BOOST_INCLUDE_PATH} ${GSL_INCLUDE_PATH})
LINK_DIRECTORIES(${BOOST_LIB_PATH} ${GSL_LIB_PATH})

ADD_SUBDIRECTORY(classes)

# build executables note quotation marks around list variable to pass all
# contents as one to macro
FOREACH(build ${BUILD_FILES})
    SET(SOURCES "${build}.cpp")
    CREATE_EXECUTABLE(${build} ${SOURCES} "${COMMON_LIBS}")
ENDFOREACH(build)

# installation
INSTALL(TARGETS "simulation"
    RUNTIME DESTINATION "bin"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    )
