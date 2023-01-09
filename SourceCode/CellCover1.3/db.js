const Sequelize = require('sequelize')
/*连接数据库并创建连接池*/
module.exports = new Sequelize('gefugai', 'root', '960406', {
  host: 'localhost',
  port: '3306',
  dialect: 'mysql',
  pool: {
    max: 20,
    min: 0,
    idle: 10000
  }
})
