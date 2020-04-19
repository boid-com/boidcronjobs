const eosjs = require('../eosjs')()
const tapos = { blocksBehind: 6, expireSeconds: 10 }

async function doAction(name,data){
  try {
    if (!data) data = {}
    const contract = 'boidcronjobs'
    const authorization = [{actor:contract,permission: 'active'}]
    const account = contract
    const result = await eosjs.api.transact({actions: [{ account, name, data, authorization }]},tapos)
    console.log(result.transaction_id)
  } catch (error) {
    console.error(error.toString())
  }
}

async function addadjjob(data){
  doAction('addadjjob',{
    new_adjustjob:{
    param_name:"stake_difficulty",
    action_name:"setstakediff",
    start_time_ms:1587239907563,
    end_time_ms:1589778000000,
    end_value:  12750000000,
    start_value: 7500000000,
    last_update_time_ms:0
  }
})
}

async function execadjjob(action_name){
  doAction('execadjjob',{action_name})
}

async function deladjjob(action_name) {
  doAction('deladjjob',{action_name})
}

const methods = {addadjjob,execadjjob,deladjjob}


if (process.argv[2]) {
  if (Object.keys(methods).find(el => el === process.argv[2])) {
    console.log('Starting:', process.argv[2])
    methods[process.argv[2]](process.argv[3], process.argv[4], process.argv[5], process.argv[6]).catch(err => console.error(err.toString()))
      .then(() => console.log('Finished'))
  }
}