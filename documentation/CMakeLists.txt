find_package(Doxygen)

if(DOXYGEN_FOUND)
  configure_file(libparc.doxygen.in libparc.doxygen @ONLY)
  add_custom_target(doc
	${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/libparc.doxygen
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMENT "Running Doxygen" VERBATIM )

  install(DIRECTORY images/            DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/images)
  install(DIRECTORY doxygen-extras/    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/doxygen-extras)
  install(DIRECTORY examples/          DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/examples)
  install(FILES DoxygenLayout.xml       DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif(DOXYGEN_FOUND)
