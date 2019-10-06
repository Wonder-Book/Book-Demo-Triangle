open Js.Typed_array;

let createIdentityMatrix4 = () =>
  Js.Typed_array.Float32Array.make([|
    1.,
    0.,
    0.,
    0.,
    0.,
    1.,
    0.,
    0.,
    0.,
    0.,
    1.,
    0.,
    0.,
    0.,
    0.,
    1.,
  |]);

let _getEpsilon = () => 0.000001;

let setLookAt =
    (
      (eyeX, eyeY, eyeZ) as eye,
      (centerX, centerY, centerZ) as center,
      (upX, upY, upZ) as up,
    ) =>
  Js.Math.abs_float(eyeX -. centerX) < _getEpsilon()
  && Js.Math.abs_float(eyeY -. centerY) < _getEpsilon()
  && Js.Math.abs_float(eyeZ -. centerZ) < _getEpsilon() ?
    createIdentityMatrix4() :
    {
      let (z1, z2, z3) as z =
        Vector3.sub(Vector3Type.Float, eye, center) |> Vector3.normalize;

      let (x1, x2, x3) as x = Vector3.cross(up, z) |> Vector3.normalize;

      let (y1, y2, y3) as y = Vector3.cross(z, x) |> Vector3.normalize;

      Float32Array.make([|
        x1,
        y1,
        z1,
        0.,
        x2,
        y2,
        z2,
        0.,
        x3,
        y3,
        z3,
        0.,
        -. Vector3.dot(x, eye),
        -. Vector3.dot(y, eye),
        -. Vector3.dot(z, eye),
        1.,
      |]);
    };

let buildPerspective =
    ((fovy: float, aspect: float, near: float, far: float), resultFloat32Arr) => {
  /* WonderLog.Contract.requireCheck(
       () =>
         WonderLog.(
           Contract.(
             Operators.(
               test(
                 Log.buildAssertMessage(
                   ~expect={j|frustum not be null|j},
                   ~actual={j|be|j},
                 ),
                 () => {
                   let fovy = Js.Math._PI *. fovy /. 180. /. 2.;
                   Js.Math.sin(fovy) <>=. 0.;
                 },
               )
             )
           )
         ),
       IsDebugMainService.getIsDebug(StateDataMain.stateData),
     ); */

  Js.Math.sin(Js.Math._PI *. fovy /. 180. /. 2.) === 0. ?
    Utils.error("frustum should not be null") : ();

  let fovy = Js.Math._PI *. fovy /. 180. /. 2.;
  let s = Js.Math.sin(fovy);
  let rd = 1. /. (far -. near);
  let ct = Js.Math.cos(fovy) /. s;
  Float32Array.unsafe_set(resultFloat32Arr, 0, ct /. aspect);
  Float32Array.unsafe_set(resultFloat32Arr, 1, 0.);
  Float32Array.unsafe_set(resultFloat32Arr, 2, 0.);
  Float32Array.unsafe_set(resultFloat32Arr, 3, 0.);
  Float32Array.unsafe_set(resultFloat32Arr, 4, 0.);
  Float32Array.unsafe_set(resultFloat32Arr, 5, ct);
  Float32Array.unsafe_set(resultFloat32Arr, 6, 0.);
  Float32Array.unsafe_set(resultFloat32Arr, 7, 0.);
  Float32Array.unsafe_set(resultFloat32Arr, 8, 0.);
  Float32Array.unsafe_set(resultFloat32Arr, 9, 0.);
  Float32Array.unsafe_set(resultFloat32Arr, 10, -. (far +. near) *. rd);
  Float32Array.unsafe_set(resultFloat32Arr, 11, -1.);
  Float32Array.unsafe_set(resultFloat32Arr, 12, 0.);
  Float32Array.unsafe_set(resultFloat32Arr, 13, 0.);
  Float32Array.unsafe_set(resultFloat32Arr, 14, (-2.) *. far *. near *. rd);
  Float32Array.unsafe_set(resultFloat32Arr, 15, 0.);
  resultFloat32Arr;
};

let setTranslation = ((x, y, z), resultFloat32Arr) => {
  Float32Array.unsafe_set(resultFloat32Arr, 12, x);
  Float32Array.unsafe_set(resultFloat32Arr, 13, y);
  Float32Array.unsafe_set(resultFloat32Arr, 14, z);

  resultFloat32Arr;
};