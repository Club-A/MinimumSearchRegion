const db = require('../db')
const Sequelize = require('sequelize')

const User = db.define('users', {
    id: {
        type: Sequelize.INTEGER(11),
        primaryKey: true,            // 主键
        autoIncrement: true,         // 自动递增
    },
    username: {
        type: Sequelize.STRING,
    },
    password: {
        type: Sequelize.STRING,
    },
    email: {
        type: Sequelize.STRING,
    }
})


const HistoryPassword = db.define('historypasswords', {
    email: {
        type: Sequelize.STRING,
    },
    password: {
        type: Sequelize.STRING,
    }
});

//生成用户信息表
const UserinfoModel = db.define('userinfos', {
    id: {
        type: Sequelize.INTEGER(11),
        primaryKey: true,            // 主键
        autoIncrement: true,         // 自动递增
    },
    image_url: Sequelize.STRING,
    user_name: Sequelize.STRING,
    city: Sequelize.STRING,
    sex: Sequelize.STRING,
    sign: Sequelize.STRING,
    user_id: Sequelize.INTEGER,
    work: Sequelize.STRING,
    edcu: Sequelize.STRING
});

//生成缺陷类型表
const HistorydiagnosesModel = db.define('historydiagnoses', {
    id: {
        type: Sequelize.INTEGER(11),
        primaryKey: true,            // 主键
        autoIncrement: true,         // 自动递增
    },
    userid: Sequelize.INTEGER,
    yuanbiaodashi: Sequelize.STRING,
    daicebiaodashi: Sequelize.STRING,
    defecttype: Sequelize.STRING,
    diagnosetype: Sequelize.STRING,
    createtime: Sequelize.DATE,
    updatetime: Sequelize.DATE,
});


//生成变体类型表
const HistoryvariantModel = db.define('historyvariants', {
    id: {
        type: Sequelize.INTEGER(11),
        primaryKey: true,            // 主键
        autoIncrement: true,         // 自动递增
    },
    userid: Sequelize.INTEGER,
    biaodashi: Sequelize.STRING,
    defecttype: Sequelize.STRING,
    diagnosetype: Sequelize.STRING,
    testcase: Sequelize.STRING,
    createtime: Sequelize.DATE,
    updatetime: Sequelize.DATE,
});

//测试用例历史记录表
const HistorytestcaseModel = db.define('historytestcases', {
    id: {
        type: Sequelize.INTEGER(11),
        primaryKey: true,            // 主键
        autoIncrement: true,         // 自动递增
    },
    userid: Sequelize.INTEGER,
    biaodashi: Sequelize.STRING,
    testcase: Sequelize.STRING,
    createtime: Sequelize.DATE,
    updatetime: Sequelize.DATE,
});

UserinfoModel.sync();
HistoryvariantModel.sync();
HistorydiagnosesModel.sync();
HistorytestcaseModel.sync();


User.sync({force: false});

module.exports = {User, HistoryPassword,UserinfoModel,HistoryvariantModel,HistorydiagnosesModel,HistorytestcaseModel}
