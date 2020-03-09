
bool boidcronjobs::is_boid_user(const name& user) {
  power_table _power = power_table(name("boidcomtoken"), user.value);
  auto itr = _power.find(user.value);
  if (itr != _power.end()) return true;
  else return false;
}

auto boidcronjobs::get_config(){
  config_table _config(get_self(), get_self().value);
  auto conf = _config.get();
  return conf;
} 