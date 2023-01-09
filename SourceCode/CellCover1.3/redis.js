const redis = require('redis')

const client = redis.createClient(6379,'127.0.0.1',{});
//错误监听
client.on("error", function (err) {
  console.log("Error " + err);
});

module.exports = client