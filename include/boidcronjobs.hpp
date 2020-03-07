#include <eosio/eosio.hpp>
#include <croneos.hpp>

using namespace std;
using namespace eosio;

CONTRACT boidcronjobs : public contract {
  public:
    using contract::contract;

    ACTION autoclaim(name user, uint32_t interval, bool cancel);


  private:

  struct power {
    name acct;
    float quantity;
    asset total_power_bonus;
    asset total_stake_bonus;
    microseconds prev_claim_time;
    microseconds prev_bp_update_time;
    asset total_delegated;

    uint64_t primary_key() const { return acct.value; }
  };
  typedef eosio::multi_index<"powers"_n, power> power_table;


  bool is_boid_user(const name& user);


};
