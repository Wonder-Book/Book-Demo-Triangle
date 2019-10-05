

import * as Js_exn from "../../../../node_modules/bs-platform/lib/es6/js_exn.js";

function error(msg) {
  Js_exn.raiseError(msg);
  return /* () */0;
}

function createTriangleGeometryData(param) {
  var vertices = new Float32Array(/* array */[
        0.0,
        1.0,
        -4.0,
        -0.5,
        -1.0,
        -4.0,
        0.5,
        -1.0,
        -4.0
      ]);
  var indices = new Uint16Array(/* array */[
        0,
        1,
        2
      ]);
  return /* tuple */[
          vertices,
          indices
        ];
}

function _compileShader(gl, glslSource, shader) {
  gl.shaderSource(shader, glslSource);
  gl.compileShader(shader);
  var match = gl.getShaderParameter(shader, gl.COMPILE_STATUS) === false;
  if (match) {
    var message = gl.getShaderInfoLog(shader);
    var msg = "shader info log: " + (String(message) + ("\n        glsl source: " + (String(glslSource) + "\n        ")));
    Js_exn.raiseError(msg);
  }
  return shader;
}

function _linkProgram(program, gl) {
  gl.linkProgram(program);
  var match = gl.getProgramParameter(program, gl.LINK_STATUS) === false;
  if (match) {
    var message = gl.getProgramInfoLog(program);
    var msg = "link program error: " + (String(message) + "");
    Js_exn.raiseError(msg);
    return /* () */0;
  } else {
    return /* () */0;
  }
}

function initShader(vsSource, fsSource, gl, program) {
  var vs = _compileShader(gl, vsSource, gl.createShader(gl.VERTEX_SHADER));
  var fs = _compileShader(gl, fsSource, gl.createShader(gl.FRAGMENT_SHADER));
  gl.attachShader(program, vs);
  gl.attachShader(program, fs);
  gl.bindAttribLocation(program, 0, "a_position");
  _linkProgram(program, gl);
  gl.deleteShader(vs);
  gl.deleteShader(fs);
  return program;
}

function initVertexBuffers(param, gl) {
  var vertexBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, param[0], gl.STATIC_DRAW);
  var indexBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer);
  gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, param[1], gl.STATIC_DRAW);
  return /* tuple */[
          vertexBuffer,
          indexBuffer
        ];
}

function sendAttributeData(vertexBuffer, program, gl) {
  var positionLocation = gl.getAttribLocation(program, "a_position");
  var match = positionLocation === -1;
  if (match) {
    Js_exn.raiseError("Failed to get the storage location of a_position");
  }
  gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
  gl.vertexAttribPointer(positionLocation, 3, gl.FLOAT, false, 0, 0);
  gl.enableVertexAttribArray(positionLocation);
  return /* () */0;
}

function sendCameraUniformData(param, program, gl) {
  var vMatrixLocation = gl.getUniformLocation(program, "u_vMatrix");
  var pMatrixLocation = gl.getUniformLocation(program, "u_pMatrix");
  gl.uniformMatrix4fv(vMatrixLocation, false, param[0]);
  gl.uniformMatrix4fv(pMatrixLocation, false, param[1]);
  return /* () */0;
}

function sendModelUniformData(param, program, gl) {
  var color = param[1];
  var mMatrixLocation = gl.getUniformLocation(program, "u_mMatrix");
  var colorLocation = gl.getUniformLocation(program, "u_color");
  gl.uniformMatrix4fv(mMatrixLocation, false, param[0]);
  gl.uniform3f(colorLocation, color[0], color[1], color[2]);
  return /* () */0;
}

export {
  error ,
  createTriangleGeometryData ,
  _compileShader ,
  _linkProgram ,
  initShader ,
  initVertexBuffers ,
  sendAttributeData ,
  sendCameraUniformData ,
  sendModelUniformData ,
  
}
/* No side effect */
