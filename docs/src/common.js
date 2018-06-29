function isSet(variable) {
  return variable === null || variable === undefined;
}

function isSetOrDefault(variable, value) {
  return isSet(variable) ? value : variable;
}

export { isSet, isSetOrDefault };