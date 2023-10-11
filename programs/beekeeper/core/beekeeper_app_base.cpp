#include <core/beekeeper_app_base.hpp>

#include <boost/exception/diagnostic_information.hpp>

namespace beekeeper {

init_data beekeeper_app_base::run( int argc, char** argv )
{
  set_program_options();

  auto _init_data = initialize( argc, argv );

  if( should_start_loop() )
    start();

  return _init_data;
}

}
