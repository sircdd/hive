file(GLOB HEADERS "include/hive/plugins/node_status_api/*.hpp")
add_library( node_status_api_plugin
             node_status_api.cpp
             node_status_api_plugin.cpp
             ${HEADERS} )

target_link_libraries( node_status_api_plugin chain_plugin json_rpc_plugin appbase )
target_include_directories( node_status_api_plugin PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

if( CLANG_TIDY_EXE )
   set_target_properties(
      node_status_api_plugin PROPERTIES
      CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
   )
endif( CLANG_TIDY_EXE )

install( TARGETS
   node_status_api_plugin

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
install( FILES ${HEADERS} DESTINATION "include/hive/node_status_api" )
