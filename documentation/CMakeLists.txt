find_package(Doxygen)

if(DOXYGEN_FOUND)
  configure_file(libparc.doxygen.in libparc.doxygen @ONLY)
  add_custom_target(doc
	${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/libparc.doxygen
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMENT "Running Doxygen" VERBATIM )
endif(DOXYGEN_FOUND)
