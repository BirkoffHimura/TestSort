"# TestSort" 

A simple test function exposed to NodeJS from c++, inside the project there's a file called "testApp.txt" that gets executed with node. it loads the add-on and uses the function called sortSimple to sort an array or typed array with JavaScript, we also use a JavaScript function to compare the two values:

var f = function(x,y) { return x<=y? true:false; }

here's the actual testApp.txt content:

var app = require('./SortTest');

var a = new Int32Array([12,4,6,8,9,1,3]);

var f = function(x,y) { return x<=y? true:false; }

var sorted = app.sortSimple(a,f);
console.log(sorted);
