#include <beekeeper/beekeeper_app.hpp>
#include <beekeeper/session_manager.hpp>

#include <core/beekeeper_wallet_manager.hpp>

#include <hive/plugins/webserver/webserver_plugin.hpp>

namespace beekeeper {

beekeeper_app::beekeeper_app()
{
}

beekeeper_app::~beekeeper_app()
{
}

std::string beekeeper_app::get_notifications_endpoint( const boost::program_options::variables_map& args )
{
  notifications_endpoint = beekeeper_app_init::get_notifications_endpoint( args );

  return notifications_endpoint;
}

void beekeeper_app::set_program_options()
{
  hive::utilities::notifications::add_program_options( options );

  beekeeper_app_init::set_program_options();
}

init_data beekeeper_app::initialize( int argc, char** argv )
{
  app.add_program_options( bpo::options_description(), options );
  app.set_app_name( "beekeeper" );
  app.set_version_string( check_version() );

  app.register_plugin<hive::plugins::webserver::webserver_plugin>();

  auto initializationResult = app.initialize<
                                hive::plugins::webserver::webserver_plugin >
                              ( argc, argv );
  start_loop = initializationResult.should_start_loop();

  if( !initializationResult.should_start_loop() )
    return { initializationResult.get_result_code() == appbase::initialization_result::ok, "" };
  else
  {
    auto _initialization = initialize_program_options();
    if( !_initialization.status )
      return _initialization;

    api_ptr = std::make_unique<beekeeper::beekeeper_wallet_api>( wallet_manager_ptr, app );

    app.notify_status( "beekeeper is starting" );

    return _initialization;
  }
}

void beekeeper_app::start()
{
  auto& _webserver_plugin = app.get_plugin<hive::plugins::webserver::webserver_plugin>();

  webserver_connection = _webserver_plugin.add_connection( [this](const hive::utilities::notifications::collector_t& collector )
  {
    instance->save_connection_details( collector );
  } );

  _webserver_plugin.start_webserver();

  app.startup();
  app.exec();
  ilog( "exited cleanly" );
}

const boost::program_options::variables_map& beekeeper_app::get_args() const
{
  return app.get_args();
}

bfs::path beekeeper_app::get_data_dir() const
{
  return app.data_dir();
}

void beekeeper_app::setup_notifications( const boost::program_options::variables_map& args )
{
  app.setup_notifications( args );
}

std::shared_ptr<beekeeper::beekeeper_wallet_manager> beekeeper_app::create_wallet( const boost::filesystem::path& cmd_wallet_dir, uint64_t cmd_unlock_timeout, uint32_t cmd_session_limit )
{
  instance = std::make_shared<beekeeper_instance>( app, cmd_wallet_dir, notifications_endpoint );
  return std::make_shared<beekeeper::beekeeper_wallet_manager>( std::make_shared<session_manager>( notifications_endpoint ), instance,
                                                                       cmd_wallet_dir, cmd_unlock_timeout, cmd_session_limit,
                                                                       [this]() { std::raise(SIGINT); } );
}

bool beekeeper_app::should_start_loop() const
{
  return start_loop;
};

init_data beekeeper_app::init( int argc, char** argv )
{
  try
  {
    return run( argc, argv );
  }
  catch ( const boost::exception& e )
  {
    elog(boost::diagnostic_information(e));
  }
  catch ( const fc::exception& e )
  {
    elog(e.to_detail_string());
  }
  catch ( const std::exception& e )
  {
    elog(e.what());
  }
  catch ( ... )
  {
    elog("unknown exception");
  }

  return init_data();
}

}
