


function dot(param, param$1) {
  return param[0] * param$1[0] + param[1] * param$1[1] + param[2] * param$1[2];
}

function sub(kind, param, param$1) {
  return /* tuple */[
          param[0] - param$1[0],
          param[1] - param$1[1],
          param[2] - param$1[2]
        ];
}

function scale(kind, scalar, param) {
  return /* tuple */[
          param[0] * scalar,
          param[1] * scalar,
          param[2] * scalar
        ];
}

function cross(param, param$1) {
  var z2 = param$1[2];
  var y2 = param$1[1];
  var x2 = param$1[0];
  var z1 = param[2];
  var y1 = param[1];
  var x1 = param[0];
  return /* tuple */[
          y1 * z2 - y2 * z1,
          z1 * x2 - z2 * x1,
          x1 * y2 - x2 * y1
        ];
}

function normalize(param) {
  var z = param[2];
  var y = param[1];
  var x = param[0];
  var d = Math.sqrt(x * x + y * y + z * z);
  var match = d === 0;
  if (match) {
    return /* tuple */[
            0,
            0,
            0
          ];
  } else {
    return /* tuple */[
            x / d,
            y / d,
            z / d
          ];
  }
}

function lengthSq(param) {
  var z = param[2];
  var y = param[1];
  var x = param[0];
  return x * x + y * y + z * z;
}

function length(vec) {
  return Math.sqrt(lengthSq(vec));
}

export {
  dot ,
  sub ,
  scale ,
  cross ,
  normalize ,
  lengthSq ,
  length ,
  
}
/* No side effect */
