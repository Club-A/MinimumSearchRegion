var express = require('express');
var router = express.Router();

/* GET home page. */
var faultDetect = require('./faultDetect/controller');
router.post('/faultDetect', faultDetect.testCommunicate);
/*
var boolDiagnose = require('./boolDiagnose/controller');
router.post('/boolDiagnose', boolDiagnose.testCommunicate);

var firstDiagnose = require('./firstDiagnose/controller');
router.post('/firstDiagnose', firstDiagnose.testCommunicate);
*/

module.exports = router;