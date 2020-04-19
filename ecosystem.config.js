const ms = require('human-interval')

module.exports = {
  apps : [
    {
      name: "runAdjustJob",
      script: 'util/do.js',
      args: ['execadjjob', 'setstakediff'],
      restart_delay: ms('10 minutes'),
    }
  ]
}
