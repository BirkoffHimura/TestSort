var app = require('./SortTest');

var a = new Int32Array([12,4,6,8,9,1,3]);

var f = function(x,y) { return x<=y? true:false; }

var sorted = app.sortSimple(a,f);
console.log(sorted);
