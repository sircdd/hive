#include <core/beekeeper_wasm_api.hpp>

#include <core/beekeeper_wasm_app.hpp>
#include <core/utilities.hpp>

#include <fc/io/json.hpp>
#include <fc/optional.hpp>

#include <boost/exception/diagnostic_information.hpp>


namespace beekeeper {

  class beekeeper_api::impl
  {
  public:
    std::vector<std::string> params;

    beekeeper::beekeeper_wasm_app app;

    init_data init_impl();
  };

  void beekeeper_api::impl_deleter::operator()(beekeeper_api::impl* ptr) const
  {
    delete ptr;
  }

  beekeeper_api::beekeeper_api( const std::vector<std::string>& _params )
  {
    _impl = std::unique_ptr<impl, impl_deleter>(new impl);
    _impl->params = _params;
    _impl->params.insert( _impl->params.begin(), "path to exe" );
  }

  init_data beekeeper_api::impl::init_impl()
  {
    char** _params = nullptr;
    init_data _result;

    try
    {
      _params = new char*[ params.size() ];
      for( size_t i = 0; i < params.size(); ++i )
        _params[i] = static_cast<char*>( params[i].data() );

      _result = app.init( params.size(), _params );

      if( _params )
        delete[] _params;
    }
    catch(...)
    {
      if( _params )
        delete[] _params;
    }

    return _result;
  }

  template<typename T>
  std::string beekeeper_api::to_string(const T& src)
  {
    fc::variant _v;
    fc::to_variant(src, _v);
    return fc::json::to_string(_v);
  }

  template<typename result_type>
  result_type beekeeper_api::exception_handler(std::function<result_type()>&& method)
  {
    try
    {
      return method();
    }
    catch (const boost::exception& e)
    {
      ilog(boost::diagnostic_information(e));
    }
    catch (const fc::exception& e)
    {
      ilog(e.to_detail_string());
    }
    catch (const std::exception& e)
    {
      ilog(e.what());
    }
    catch (...)
    {
      ilog("unknown exception");
    }
    return result_type();
  }


  std::string beekeeper_api::init()
  {
    auto _method = [&, this]()
    {
      return to_string( _impl->init_impl() );
    };
    return exception_handler<std::string>( _method );
  }

  std::string beekeeper_api::create_session( const std::string& salt )
  {
    auto _method = [&, this]()
    {
      create_session_return _result{ _impl->app.get_wallet_manager()->create_session( salt, "notification endpoint"/*notifications_endpoint - not used here*/ ) };
      return to_string( _result );
    };
    return exception_handler<std::string>( _method );
  }

  void beekeeper_api::close_session( const std::string& token )
  {
    auto _method = [&, this]()
    {
      _impl->app.get_wallet_manager()->close_session( token );
    };
    return exception_handler<void>( _method );
  }

  std::string beekeeper_api::create( const std::string& token, const std::string& wallet_name, const std::string& password )
  {
    auto _method = [&, this]()
    {
      create_return _result;

      if( password.empty() )
        _result = { _impl->app.get_wallet_manager()->create( token, wallet_name ) };
      else
        _result = { _impl->app.get_wallet_manager()->create( token, wallet_name, password ) };

      return to_string( _result );
    };
    return exception_handler<std::string>( _method );
  }

  void beekeeper_api::unlock( const std::string& token, const std::string& wallet_name, const std::string& password )
  {
    auto _method = [&, this]()
    {
      _impl->app.get_wallet_manager()->unlock( token, wallet_name, password );
    };
    return exception_handler<void>( _method );
  }

  void beekeeper_api::open( const std::string& token, const std::string& wallet_name )
  {
    auto _method = [&, this]()
    {
      _impl->app.get_wallet_manager()->open( token, wallet_name );
    };
    return exception_handler<void>( _method );
  }

  void beekeeper_api::close( const std::string& token, const std::string& wallet_name )
  {
    auto _method = [&, this]()
    {
      _impl->app.get_wallet_manager()->close( token, wallet_name );
    };
    return exception_handler<void>( _method );
  }

  void beekeeper_api::set_timeout( const std::string& token, int32_t seconds )
  {
    auto _method = [&, this]()
    {
      _impl->app.get_wallet_manager()->set_timeout( token, seconds );
    };
    return exception_handler<void>( _method );
  }

  void beekeeper_api::lock_all( const std::string& token )
  {
    auto _method = [&, this]()
    {
      _impl->app.get_wallet_manager()->lock_all( token );
    };
    return exception_handler<void>( _method );
  }

  void beekeeper_api::lock( const std::string& token, const std::string& wallet_name )
  {
    auto _method = [&, this]()
    {
      _impl->app.get_wallet_manager()->lock( token, wallet_name );
    };
    return exception_handler<void>( _method );
  }

  std::string beekeeper_api::import_key( const std::string& token, const std::string& wallet_name, const std::string& wif_key )
  {
    auto _method = [&, this]()
    {
      import_key_return _result = { _impl->app.get_wallet_manager()->import_key( token, wallet_name, wif_key ) };
      return to_string( _result );
    };
    return exception_handler<std::string>( _method );
  }

  void beekeeper_api::remove_key( const std::string& token, const std::string& wallet_name, const std::string& password, const std::string& public_key )
  {
    auto _method = [&, this]()
    {
      _impl->app.get_wallet_manager()->remove_key( token, wallet_name, password, public_key );
    };
    return exception_handler<void>( _method );
  }

  std::string beekeeper_api::list_wallets( const std::string& token )
  {
    auto _method = [&, this]()
    {
      list_wallets_return _result = { _impl->app.get_wallet_manager()->list_wallets( token ) };
      return to_string( _result );
    };
    return exception_handler<std::string>( _method );
  }

  std::string beekeeper_api::get_public_keys( const std::string& token )
  {
    auto _method = [&, this]()
    {
      get_public_keys_return _result = { utility::get_public_keys( _impl->app.get_wallet_manager()->get_public_keys( token ) ) };
      return to_string( _result );
    };
    return exception_handler<std::string>( _method );
  }

  std::string beekeeper_api::sign_digest( const std::string& token, const std::string& public_key, const std::string& sig_digest )
  {
    auto _method = [&, this]()
    {
      signature_return _result = { _impl->app.get_wallet_manager()->sign_digest( token, utility::get_public_key( public_key ), digest_type( sig_digest ) ) };
      return to_string( _result );
    };
    return exception_handler<std::string>( _method );
  }

  std::string beekeeper_api::sign_transaction( const std::string& token, const std::string& transaction, const std::string& chain_id, const std::string& public_key, const std::string& sig_digest )
  {
    auto _method = [&, this]()
    {
      signature_return _result = { _impl->app.get_wallet_manager()->sign_transaction( token, transaction, chain_id_type( chain_id ), utility::get_public_key( public_key ), digest_type( sig_digest ) ) };
      return to_string( _result );
    };
    return exception_handler<std::string>( _method );
  }

  std::string beekeeper_api::get_info( const std::string& token )
  {
    auto _method = [&, this]()
    {
      return to_string( _impl->app.get_wallet_manager()->get_info( token ) );
    };
    return exception_handler<std::string>( _method );
  }

};