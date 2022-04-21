const addon = require('../build/Release/hello-world-native');

module.exports = {
  hello: addon.hello,
  helloAsync: addon.helloAsync,
};
