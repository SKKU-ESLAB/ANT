/* Copyright (c) 2017-2021 SKKU ESLAB, and contributors. All rights reserved.
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

var initialCode = 'var ant = require("ant"); \n\
var console = require("console"); \n\
\n\
var onInitialize = function () {\n\
  console.log("onInitialize");\n\
};\n\
\n\
var onStart = function () {\n\
  console.log("onStart");\n\
};\n\
\n\
var onStop = function () {\n\
  console.log("onStop");\n\
};\n\
\n\
ant.runtime.setCurrentApp(onInitialize, onStart, onStop);\n';

function config() {
  this.initialCode = initialCode;
}
