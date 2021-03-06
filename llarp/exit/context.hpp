#ifndef LLARP_EXIT_CONTEXT_HPP
#define LLARP_EXIT_CONTEXT_HPP
#include <exit/policy.hpp>
#include <handlers/exit.hpp>

#include <string>
#include <unordered_map>

namespace llarp
{
  namespace exit
  {
    /// owner of all the exit endpoints
    struct Context : public util::IStateful
    {
      using Config_t = std::unordered_multimap< std::string, std::string >;

      Context(AbstractRouter *r);
      ~Context();

      void
      Tick(llarp_time_t now);

      void
      ClearAllEndpoints();

      util::StatusObject
      ExtractStatus() const override;

      /// send close to all exit sessions and remove all sessions
      void
      Stop();

      bool
      AddExitEndpoint(const std::string &name, const Config_t &config);

      bool
      ObtainNewExit(const llarp::PubKey &remote, const llarp::PathID_t &path,
                    bool permitInternet);

      llarp::exit::Endpoint *
      FindEndpointForPath(const llarp::PathID_t &path) const;

      /// calculate (pk, tx, rx) for all exit traffic
      using TrafficStats =
          std::unordered_map< PubKey, std::pair< uint64_t, uint64_t >,
                              PubKey::Hash >;

      void
      CalculateExitTraffic(TrafficStats &stats);

     private:
      AbstractRouter *m_Router;
      std::unordered_map< std::string,
                          std::unique_ptr< llarp::handlers::ExitEndpoint > >
          m_Exits;
      std::list< std::unique_ptr< llarp::handlers::ExitEndpoint > > m_Closed;
    };
  }  // namespace exit
}  // namespace llarp

#endif
