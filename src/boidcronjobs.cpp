#include <boidcronjobs.hpp>

ACTION boidcronjobs::autoclaim(name user, uint32_t interval, bool cancel) {

    //user needs to call this to start and cancel the scheduling
    //boid.cron or default exec account need to be able to call this to execute
    check(eosio::has_auth(user) || eosio::has_auth(name("croneosexec1")) || eosio::has_auth(get_self()), "missing required authority");

    //can check here if user is really a boid user and not an arbitrary account

    if(cancel){
        //cancel flag true so delete the scheduled job from the user
        croneos::job::cancel_by_tag(get_self(), user, name(0), permission_level{get_self(), "active"_n});
        return;
    }

    //claim action inline on token contract boidcomtoken
    float percentage_to_stake = 0; //???
    bool issuer_claim = true; //???
    action(
        permission_level{ name("boidcomtoken"), "autoclaim"_n },//must have permission on the token contract to call claim!
        name("boidcomtoken"),
        "claim"_n,
        std::make_tuple(user, percentage_to_stake, issuer_claim)
    ).send();

    //schedule config
    croneos::job mycronjob;
    mycronjob.owner = get_self();
    mycronjob.tag = user;
    mycronjob.delay_sec = interval; // uint32_t
    mycronjob.expiration_sec = 60*60*24; // uint32_t
    mycronjob.gas_fee = extended_asset(asset(100000, symbol(symbol_code("BOID"), 4) ), name("boidcomtoken") ); //1 BOID
    mycronjob.auto_pay_gas = true; //deposit gas fee for current job. (triggers a transfer)
    // mycronjob.custom_exec_permissions ={permission_level{"boid.cron"_n, "active"_n} };
    //mycronjob.auth_bouncer = name("boidvalidators");
    mycronjob.description ="Boid auto claim for user "+user.to_string();

    //submit send the job
    mycronjob.schedule(
      name("boidcronjobs"), //contract that holds the to be scheduled action
      name("autoclaim"), //its action name
      make_tuple(user, interval, false ), //the action data
      permission_level{get_self(), "active"_n} //authorization for scheduling NOT for execution of the scheduled job
    );

}
