#include <eosio/eosio.hpp>
#include <croneos.hpp>

using namespace std;
using namespace eosio;

CONTRACT boidcronjobs : public contract {
  public:
    using contract::contract;

    ACTION autoclaim(name user, uint32_t interval, bool cancel);


  private:

};
