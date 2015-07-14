var addon = require('bindings')('s2n');
delete addon.path;
module.exports = addon;
