PROJECT(classes)

SET(LIB_SOURCES C_Evolution.cpp C_FlowDistributionNetwork.cpp C_LinkMutation.cpp
    C_LinkRobustNetwork.cpp C_Mutation.cpp C_NetworkBuilder.cpp
    C_NetworkInitialiser.cpp C_NodeRobustNetwork.cpp C_ParameterManager.cpp
    C_PathMutation.cpp C_PatternInitialiser.cpp C_RandomNetworkInitialiser.cpp
    C_RandomPatternInitialiser.cpp C_RobustEvolution.cpp
    C_RobustFunctionalNetwork.cpp C_RobustParameterManager.cpp
    C_NoiseRobustParameterManager.cpp C_NoiseRobustNetwork.cpp
    C_ComplexityPatternInitialiser.cpp C_HighComplexityPatternInitialiser.cpp
    C_LowComplexityPatternInitialiser.cpp C_EdgeListNetworkInitialiser.cpp
    C_EqualComplexityPatternInitialiser.cpp
    C_EqualSpreadComplexityPatternInitialiser.cpp)

#AUX_SOURCE_DIRECTORY(. LIB_SOURCES)
ADD_LIBRARY("rfn_classes" SHARED ${LIB_SOURCES})
SET_TARGET_PROPERTIES("rfn_classes" PROPERTIES LIBRARY_OUTPUT_DIRECTORY
    ${RFN5_SOURCE_DIR}/lib)
GET_TARGET_PROPERTY(INSTALL "rfn_classes" LIBRARY_OUTPUT_DIRECTORY)
#MESSAGE(STATUS "Building library " "rfn_classes" " in location "
#    "'${INSTALL}'")

# installation
INSTALL(TARGETS "rfn_classes"
    RUNTIME DESTINATION "bin"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    )
