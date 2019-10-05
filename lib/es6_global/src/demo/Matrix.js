

import * as Utils$TinyWonderEditor from "./Utils.js";
import * as Vector3$TinyWonderEditor from "./Vector3.js";

function createIdentityMatrix4(param) {
  return new Float32Array(/* array */[
              1,
              0,
              0,
              0,
              0,
              1,
              0,
              0,
              0,
              0,
              1,
              0,
              0,
              0,
              0,
              1
            ]);
}

function _getEpsilon(param) {
  return 0.000001;
}

function setLookAt(eye, center, up) {
  var match = Math.abs(eye[0] - center[0]) < 0.000001 && Math.abs(eye[1] - center[1]) < 0.000001 && Math.abs(eye[2] - center[2]) < 0.000001;
  if (match) {
    return createIdentityMatrix4(/* () */0);
  } else {
    var z = Vector3$TinyWonderEditor.normalize(Vector3$TinyWonderEditor.sub(/* Float */0, eye, center));
    var x = Vector3$TinyWonderEditor.normalize(Vector3$TinyWonderEditor.cross(up, z));
    var y = Vector3$TinyWonderEditor.normalize(Vector3$TinyWonderEditor.cross(z, x));
    return new Float32Array(/* array */[
                x[0],
                y[0],
                z[0],
                0,
                x[1],
                y[1],
                z[1],
                0,
                x[2],
                y[2],
                z[2],
                0,
                -Vector3$TinyWonderEditor.dot(x, eye),
                -Vector3$TinyWonderEditor.dot(y, eye),
                -Vector3$TinyWonderEditor.dot(z, eye),
                1
              ]);
  }
}

function buildPerspective(param, resultFloat32Arr) {
  var far = param[3];
  var near = param[2];
  var fovy = param[0];
  var match = Math.sin(Math.PI * fovy / 180 / 2) === 0;
  if (match) {
    Utils$TinyWonderEditor.error("frustum should not be null");
  }
  var fovy$1 = Math.PI * fovy / 180 / 2;
  var s = Math.sin(fovy$1);
  var rd = 1 / (far - near);
  var ct = Math.cos(fovy$1) / s;
  resultFloat32Arr[0] = ct / param[1];
  resultFloat32Arr[1] = 0;
  resultFloat32Arr[2] = 0;
  resultFloat32Arr[3] = 0;
  resultFloat32Arr[4] = 0;
  resultFloat32Arr[5] = ct;
  resultFloat32Arr[6] = 0;
  resultFloat32Arr[7] = 0;
  resultFloat32Arr[8] = 0;
  resultFloat32Arr[9] = 0;
  resultFloat32Arr[10] = -(far + near) * rd;
  resultFloat32Arr[11] = -1;
  resultFloat32Arr[12] = 0;
  resultFloat32Arr[13] = 0;
  resultFloat32Arr[14] = -2 * far * near * rd;
  resultFloat32Arr[15] = 0;
  return resultFloat32Arr;
}

function setTranslation(param, resultFloat32Arr) {
  resultFloat32Arr[12] = param[0];
  resultFloat32Arr[13] = param[1];
  resultFloat32Arr[14] = param[2];
  return resultFloat32Arr;
}

export {
  createIdentityMatrix4 ,
  _getEpsilon ,
  setLookAt ,
  buildPerspective ,
  setTranslation ,
  
}
/* No side effect */
