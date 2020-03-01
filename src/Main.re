let _init = () => {
  let canvas = DomExtend.querySelector(DomExtend.document, "#webgl");

  let gl =
    Gl.getWebGL1Context(
      canvas,
      {
        "alpha": true,
        "depth": true,
        "stencil": false,
        "antialias": true,
        "premultipliedAlpha": true,
        "preserveDrawingBuffer": false,
      }: Gl.contextConfigJsObj,
    );

  let program1 =
    gl |> Gl.createProgram |> Utils.initShader(GLSL.vs1, GLSL.fs1, gl);

  let program2 =
    gl |> Gl.createProgram |> Utils.initShader(GLSL.vs2, GLSL.fs2, gl);

  let (vertices1, indices1) = Utils.createTriangleVertexData();
  let (vertices2, indices2) = Utils.createTriangleVertexData();
  let (vertices3, indices3) = Utils.createTriangleVertexData();

  let (vertexBuffer1, indexBuffer1) =
    Utils.initVertexBuffers((vertices1, indices1), gl);

  let (vertexBuffer2, indexBuffer2) =
    Utils.initVertexBuffers((vertices2, indices2), gl);

  let (vertexBuffer3, indexBuffer3) =
    Utils.initVertexBuffers((vertices3, indices3), gl);

  let (position1, position2, position3) = (
    (0.75, 0., 0.),
    ((-0.), 0., 0.5),
    ((-0.5), 0., (-2.)),
  );

  let (color1, (color2_1, color2_2), color3) = (
    (1., 0., 0.),
    ((0., 0.8, 0.), (0., 0.5, 0.)),
    (0., 0., 1.),
  );

  let ((eyeX, eyeY, eyeZ), (centerX, centerY, centerZ), (upX, upY, upZ)) = (
    (0., 0.0, 5.),
    (0., 0., (-100.)),
    (0., 1., 0.),
  );
  let (near, far, fovy, aspect) = (
    1.,
    100.,
    30.,
    (canvas##width |> Js.Int.toFloat) /. (canvas##height |> Js.Int.toFloat),
  );

  (
    gl,
    (
      (program1, program2),
      (indices1, indices2, indices3),
      (vertexBuffer1, indexBuffer1),
      (vertexBuffer2, indexBuffer2),
      (vertexBuffer3, indexBuffer3),
      (position1, position2, position3),
      (color1, (color2_1, color2_2), color3),
      (
        ((eyeX, eyeY, eyeZ), (centerX, centerY, centerZ), (upX, upY, upZ)),
        (near, far, fovy, aspect),
      ),
    ),
  );
};

let _render =
    (
      (
        gl,
        (
          (program1, program2),
          (indices1, indices2, indices3),
          (vertexBuffer1, indexBuffer1),
          (vertexBuffer2, indexBuffer2),
          (vertexBuffer3, indexBuffer3),
          (position1, position2, position3),
          (color1, (color2_1, color2_2), color3),
          (
            (
              (eyeX, eyeY, eyeZ),
              (centerX, centerY, centerZ),
              (upX, upY, upZ),
            ),
            (near, far, fovy, aspect),
          ),
        ),
      ),
    ) => {
  Gl.enable(Gl.getDepthTest(gl), gl);

  Gl.enable(Gl.getCullFace(gl), gl);
  Gl.cullFace(Gl.getBack(gl), gl);

  let vMatrix =
    Matrix.createIdentityMatrix()
    |> Matrix.setLookAt(
         (eyeX, eyeY, eyeZ),
         (centerX, centerY, centerZ),
         (upX, upY, upZ),
       );
  let pMatrix =
    Matrix.createIdentityMatrix()
    |> Matrix.buildPerspective((near, far, fovy, aspect));

  let mMatrix1 =
    Matrix.createIdentityMatrix() |> Matrix.setTranslation(position1);
  let mMatrix2 =
    Matrix.createIdentityMatrix() |> Matrix.setTranslation(position2);
  let mMatrix3 =
    Matrix.createIdentityMatrix() |> Matrix.setTranslation(position3);

  Gl.useProgram(program1, gl);

  Utils.sendAttributeData(vertexBuffer1, program1, gl);

  Utils.sendCameraUniformData((vMatrix, pMatrix), program1, gl);

  Utils.sendModelUniformData1((mMatrix1, color1), program1, gl);

  Gl.bindBuffer(Gl.getElementArrayBuffer(gl), indexBuffer1, gl);

  Gl.drawElements(
    Gl.getTriangles(gl),
    indices1 |> Js.Typed_array.Uint16Array.length,
    Gl.getUnsignedShort(gl),
    0,
    gl,
  );

  Gl.useProgram(program2, gl);

  Utils.sendAttributeData(vertexBuffer2, program2, gl);

  Utils.sendCameraUniformData((vMatrix, pMatrix), program2, gl);

  Utils.sendModelUniformData2((mMatrix2, color2_1, color2_2), program2, gl);

  Gl.bindBuffer(Gl.getElementArrayBuffer(gl), indexBuffer2, gl);

  Gl.drawElements(
    Gl.getTriangles(gl),
    indices2 |> Js.Typed_array.Uint16Array.length,
    Gl.getUnsignedShort(gl),
    0,
    gl,
  );

  Gl.useProgram(program1, gl);

  Utils.sendAttributeData(vertexBuffer3, program1, gl);

  Utils.sendCameraUniformData((vMatrix, pMatrix), program1, gl);

  Utils.sendModelUniformData1((mMatrix3, color3), program1, gl);

  Gl.bindBuffer(Gl.getElementArrayBuffer(gl), indexBuffer3, gl);

  Gl.drawElements(
    Gl.getTriangles(gl),
    indices3 |> Js.Typed_array.Uint16Array.length,
    Gl.getUnsignedShort(gl),
    0,
    gl,
  );
};

let _clearColor = ((gl, sceneData) as data) => {
  Gl.clearColor(0., 0., 0., 1., gl);

  data;
};

let _clearCanvas = ((gl, sceneData) as data) => {
  Gl.clear(Gl.getColorBufferBit(gl) lor Gl.getDepthBufferBit(gl), gl);

  data;
};

let _loopBody = data => {
  data |> _clearColor |> _clearCanvas |> _render;
};

let rec _loop = data =>
  DomExtend.requestAnimationFrame((time: float) => {
    _loopBody(data);
    _loop(data) |> ignore;
  });

let main = () => {
  _init() |> _loop |> ignore;
};