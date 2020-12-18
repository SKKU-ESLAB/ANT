/* Copyright (c) 2017-2020 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

var console = require('console');

/** ANT main object start **/
function ANT() {}

console.log('ANT API checking...');

try {
  ANT.prototype.runtime = require('antruntime');
} catch (e) {
  throw new Error('ANT Runtime API is always required!');
}
try {
  ANT.prototype.resource = require('antresource');
} catch (e) {
  console.log('> Not found Resource API');
}
try {
  ANT.prototype.remoteui = require('antremoteui');
} catch (e) {
  console.log('> Not found Remote UI API');
}
try {
  ANT.prototype.stream = require('antstream');
} catch (e) {
  console.log('> Not found Stream API');
}
try {
  ANT.prototype.ml = require('antml');
} catch (e) {
  console.log('> Not found ML API');
}
try {
  ANT.prototype.companion = require('antcompanion');
} catch (e) {
  console.log('> Not found Companion API');
}
try {
  ANT.prototype.gateway = require('antgateway');
} catch (e) {
  console.log('> Not found Gateway API');
}

/** ANT main object end **/

module.exports = new ANT();
module.exports.ANT = ANT;
