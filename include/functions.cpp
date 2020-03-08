
bool boidcronjobs::is_boid_user(const name& user) {
  power_table _power = power_table(name("boidcomtoken"), user.value);
  auto itr = _power.find(user.value);
  if (itr != _power.end()) return true;
  else return false;
}