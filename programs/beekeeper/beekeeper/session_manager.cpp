#include <beekeeper/session_manager.hpp>
#include <beekeeper/session.hpp>
#include <beekeeper/time_manager.hpp>

namespace beekeeper {

session_manager::session_manager()
{
  time = std::make_shared<time_manager>();
}

std::shared_ptr<session_base> session_manager::create_session( const std::string& notifications_endpoint, const std::string& token, std::shared_ptr<time_manager_base> time )
{
  return std::make_shared<session>( notifications_endpoint, token, time );
}

} //beekeeper