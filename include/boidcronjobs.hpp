#include <eosio/eosio.hpp>
#include <eosio/singleton.hpp>
#include <croneos.hpp>

// #define TOKEN_CONTRACT "token.boid"
#define TOKEN_CONTRACT "boidcomtoken"


using namespace std;
using namespace eosio;

CONTRACT boidcronjobs : public contract {


  private:
    TABLE contributors
    {
      eosio::name account;
      uint32_t donations = 0;
      uint32_t level = 1;
      uint32_t total_power_granted = 0;
      uint64_t primary_key() const { return account.value; }
    };

    using contributors_table = eosio::multi_index<"contributors"_n, contributors>;

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
    auto get_config();
    uint64_t now_ms = eosio::current_time_point().time_since_epoch().count()/1000;

    float convertRange(float OldValue, float OldMin, float NewMax, float NewMin, float OldMax){
      return (((OldValue - OldMin) * (NewMax - NewMin)) / (OldMax - OldMin)) + NewMin;
    }

    template <typename T>
    void cleanTable(name code, uint64_t account, const uint32_t batchSize) {
      T db(code, account);
      uint32_t counter = 0;
      auto itr = db.begin();
      while (itr != db.end() && counter++ < batchSize) {
        itr = db.erase(itr);
      }
    }

    public:
      using contract::contract;

      TABLE adjustjob {
        name action_name;
        float start_value;
        float end_value;
        uint64_t start_time_ms;
        uint64_t end_time_ms;
        uint64_t last_update_time_ms;
        uint64_t primary_key() const { return action_name.value; }
      };
      typedef eosio::multi_index<"adjustjobs"_n, adjustjob> adjustjobs_table;

      TABLE config {
        bool auto_claim = true;
        bool r1 = false;
        bool r2 = false;
        bool r3 = false;
      };
      typedef eosio::singleton<"config"_n, config> config_table;

      ACTION autoclaim(name user, uint32_t interval, extended_asset gas_fee ,bool registeronly );
      ACTION setconfig(config conf);
      ACTION canceljob(name user);
      ACTION addadjjob(adjustjob new_adjustjob);
      ACTION execadjjob(name action_name);
      ACTION deladjjob(name action_name);


};
