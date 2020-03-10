#include <boidcronjobs.hpp>
#include <functions.cpp>

ACTION boidcronjobs::canceljob(name user) {
  check(eosio::has_auth(user) || eosio::has_auth(name("croneosexec1")) || eosio::has_auth(get_self()), "missing required authority");
  croneos::job::cancel_by_tag(get_self(), user, name(0), permission_level{get_self(), "active"_n});
}

ACTION boidcronjobs::autoclaim(name user, uint32_t interval, extended_asset gas_fee ,bool registeronly ) {

    //user needs to call this to start and cancel the scheduling
    //boid.cron or default exec account need to be able to call this to execute
    check(eosio::has_auth(user) || eosio::has_auth(name("croneosexec1")) || eosio::has_auth(get_self()), "missing required authority");
    //can check here if user is really a boid user and not an arbitrary account
    check(is_boid_user(user), "User not authorized to claim.");

    if (!registeronly) {
      //claim action inline on token contract boidcomtoken
      float percentage_to_stake = 0; //???
      bool issuer_claim = true; //???
      action(
          permission_level{ name("boidcomtoken"), "autoclaim"_n },//must have permission on the token contract to call claim!
          name("boidcomtoken"),
          "claim"_n,
          std::make_tuple(user, percentage_to_stake, issuer_claim)
      ).send();
    };

    if(get_config().auto_claim ){
        //schedule config
        croneos::job mycronjob;
        mycronjob.owner = get_self();
        mycronjob.tag = user;
        mycronjob.delay_sec = interval; // uint32_t
        mycronjob.expiration_sec = 60*60*1; // uint32_t
        // mycronjob.gas_fee = extended_asset(asset(50000, symbol(symbol_code("BOID"), 4) ), name("boidcomtoken") ); //1 BOID
        mycronjob.gas_fee = gas_fee;
        mycronjob.auto_pay_gas = false; //deposit gas fee for current job. (triggers a transfer)
        // mycronjob.custom_exec_permissions ={permission_level{"boid.cron"_n, "active"_n} };
        //mycronjob.auth_bouncer = name("boidvalidators");
        mycronjob.description = "Boid auto claim for user "+user.to_string();

        //submit send the job
        mycronjob.schedule(
        name("boidcronjobs"), //contract that holds the to be scheduled action
        name("autoclaim"), //its action name
        make_tuple(user, interval, gas_fee, false ), //the action data
        permission_level{get_self(), "active"_n} //authorization for scheduling NOT for execution of the scheduled job
        );
    }

}

ACTION boidcronjobs::setconfig(config conf){
    require_auth(get_self() );
    config_table _config(get_self(), get_self().value);
    _config.set(conf, get_self());
}
