

import * as GLSL$TinyWonderEditor from "./GLSL.js";
import * as Utils$TinyWonderEditor from "./Utils.js";
import * as Matrix$TinyWonderEditor from "./Matrix.js";

function main(param) {
  var canvas = document.querySelector("#webgl");
  var gl = canvas.getContext("webgl", {
        alpha: true,
        depth: true,
        stencil: false,
        antialias: true,
        premultipliedAlpha: true,
        preserveDrawingBuffer: false
      });
  var program1 = Utils$TinyWonderEditor.initShader(GLSL$TinyWonderEditor.vs1, GLSL$TinyWonderEditor.fs1, gl, gl.createProgram());
  var program2 = Utils$TinyWonderEditor.initShader(GLSL$TinyWonderEditor.vs2, GLSL$TinyWonderEditor.fs2, gl, gl.createProgram());
  var match = Utils$TinyWonderEditor.createTriangleGeometryData(/* () */0);
  var indices1 = match[1];
  var match$1 = Utils$TinyWonderEditor.createTriangleGeometryData(/* () */0);
  var indices2 = match$1[1];
  var match$2 = Utils$TinyWonderEditor.createTriangleGeometryData(/* () */0);
  var indices3 = match$2[1];
  var match$3 = Utils$TinyWonderEditor.initVertexBuffers(/* tuple */[
        match[0],
        indices1
      ], gl);
  var indexBuffer1 = match$3[1];
  var vertexBuffer1 = match$3[0];
  var match$4 = Utils$TinyWonderEditor.initVertexBuffers(/* tuple */[
        match$1[0],
        indices2
      ], gl);
  var indexBuffer2 = match$4[1];
  var vertexBuffer2 = match$4[0];
  var match$5 = Utils$TinyWonderEditor.initVertexBuffers(/* tuple */[
        match$2[0],
        indices3
      ], gl);
  var indexBuffer3 = match$5[1];
  var vertexBuffer3 = match$5[0];
  var vMatrix = Matrix$TinyWonderEditor.setLookAt(/* tuple */[
        0,
        0.0,
        5
      ], /* tuple */[
        0,
        0,
        -100
      ], /* tuple */[
        0,
        1,
        0
      ]);
  var pMatrix = Matrix$TinyWonderEditor.buildPerspective(/* tuple */[
        30,
        canvas.width / canvas.height,
        1,
        100
      ], Matrix$TinyWonderEditor.createIdentityMatrix4(/* () */0));
  gl.clearColor(0, 0, 0, 1);
  var _loopBody = function (time) {
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    gl.enable(gl.DEPTH_TEST);
    gl.enable(gl.CULL_FACE);
    gl.cullFace(gl.BACK);
    gl.useProgram(program1);
    Utils$TinyWonderEditor.sendAttributeData(vertexBuffer1, program1, gl);
    Utils$TinyWonderEditor.sendCameraUniformData(/* tuple */[
          vMatrix,
          pMatrix
        ], program1, gl);
    Utils$TinyWonderEditor.sendModelUniformData(/* tuple */[
          Matrix$TinyWonderEditor.setTranslation(/* tuple */[
                0.75,
                0,
                0
              ], Matrix$TinyWonderEditor.createIdentityMatrix4(/* () */0)),
          /* tuple */[
            1,
            0,
            0
          ]
        ], program1, gl);
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer1);
    gl.drawElements(gl.TRIANGLES, indices1.length, gl.UNSIGNED_SHORT, 0);
    gl.useProgram(program2);
    Utils$TinyWonderEditor.sendAttributeData(vertexBuffer2, program2, gl);
    Utils$TinyWonderEditor.sendCameraUniformData(/* tuple */[
          vMatrix,
          pMatrix
        ], program2, gl);
    Utils$TinyWonderEditor.sendModelUniformData(/* tuple */[
          Matrix$TinyWonderEditor.setTranslation(/* tuple */[
                -0,
                0,
                0.5
              ], Matrix$TinyWonderEditor.createIdentityMatrix4(/* () */0)),
          /* tuple */[
            0,
            1,
            0
          ]
        ], program2, gl);
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer2);
    gl.drawElements(gl.TRIANGLES, indices2.length, gl.UNSIGNED_SHORT, 0);
    gl.useProgram(program1);
    Utils$TinyWonderEditor.sendAttributeData(vertexBuffer3, program1, gl);
    Utils$TinyWonderEditor.sendCameraUniformData(/* tuple */[
          vMatrix,
          pMatrix
        ], program1, gl);
    Utils$TinyWonderEditor.sendModelUniformData(/* tuple */[
          Matrix$TinyWonderEditor.setTranslation(/* tuple */[
                -0.5,
                0,
                -2
              ], Matrix$TinyWonderEditor.createIdentityMatrix4(/* () */0)),
          /* tuple */[
            0,
            0,
            1
          ]
        ], program1, gl);
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer3);
    gl.drawElements(gl.TRIANGLES, indices3.length, gl.UNSIGNED_SHORT, 0);
    return /* () */0;
  };
  var _loop = function (time) {
    return requestAnimationFrame((function (time) {
                  _loopBody(time);
                  _loop(time);
                  return /* () */0;
                }));
  };
  return _loop(0);
}

export {
  main ,
  
}
/* No side effect */
