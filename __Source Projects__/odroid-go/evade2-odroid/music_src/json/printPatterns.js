#!/usr/bin/env node

const fs = require('fs'),
      process = require('process');

const fileToOpen = process.argv[2],
      patternNumber = process.argv[3];


if (! fileToOpen) {
  console.log("Error! no file to open!");
  process.exit(1);
}

if (! fs.existsSync(fileToOpen)) {
  console.log(`Error! Could not open %{fileToOpen}`);
  process.exit(2);
}

var fileData = fs.readFileSync(fileToOpen).toString();

if (!fileData) {
  console.log("Error! no file data!");
  process.exit(3);
}

fileData = JSON.parse(fileData);

const patternsRaw = fileData.patternsRaw;

if (! patternNumber) {
  var keys = Object.keys(patternsRaw).sort();
  var tmp = [];
  keys.forEach(function(k) {
    tmp.push(parseInt(k, 10));
  });
  keys = tmp.sort(function(a,b) { return a - b;});
  delete tmp;

  keys.forEach(function(key) {
    const separator = `-------- -------- -------- PATTERN ${key} -------- -------- -------`;
    console.log(separator);
    console.log(JSON.stringify(patternsRaw[key], null, 4));
  });
}
else {
  const separator = `-------- -------- -------- PATTERN ${patternNumber} -------- -------- -------`;
  console.log(separator);
  console.log(JSON.stringify(patternsRaw[patternNumber], null, 4));
}
