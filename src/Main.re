let _init = () => {
  let canvas = DomExtend.querySelector(DomExtend.document, "#webgl");

  let gl =
    Gl.getWebgl1Context(
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

  let vMatrix =
    Matrix.createIdentityMatrix()
    |> Matrix.setLookAt((0., 0.0, 5.), (0., 0., (-100.)), (0., 1., 0.));

  let pMatrix =
    Matrix.createIdentityMatrix()
    |> Matrix.buildPerspective((
         30.,
         (canvas##width |> Js.Int.toFloat)
         /. (canvas##height |> Js.Int.toFloat),
         1.,
         100.,
       ));

  Gl.clearColor(0., 0., 0., 1., gl);

  (
    gl,
    (program1, program2),
    (indices1, indices2, indices3),
    (vertexBuffer1, indexBuffer1),
    (vertexBuffer2, indexBuffer2),
    (vertexBuffer3, indexBuffer3),
    (vMatrix, pMatrix),
  );
};

let _render =
    (
      (
        gl,
        (program1, program2),
        (indices1, indices2, indices3),
        (vertexBuffer1, indexBuffer1),
        (vertexBuffer2, indexBuffer2),
        (vertexBuffer3, indexBuffer3),
        (vMatrix, pMatrix),
      ),
    ) => {
  Gl.enable(Gl.getDepthTest(gl), gl);

  Gl.enable(Gl.getCullFace(gl), gl);
  Gl.cullFace(Gl.getBack(gl), gl);

  Gl.useProgram(program1, gl);

  Utils.sendAttributeData(vertexBuffer1, program1, gl);

  Utils.sendCameraUniformData((vMatrix, pMatrix), program1, gl);

  Utils.sendModelUniformData1(
    (
      Matrix.createIdentityMatrix() |> Matrix.setTranslation((0.75, 0., 0.)),
      (1., 0., 0.),
    ),
    program1,
    gl,
  );

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

  Utils.sendModelUniformData2(
    (
      Matrix.createIdentityMatrix() |> Matrix.setTranslation(((-0.), 0., 0.5)),
      (0., 0.8, 0.),
      (0., 0.5, 0.),
    ),
    program2,
    gl,
  );

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

  Utils.sendModelUniformData1(
    (
      Matrix.createIdentityMatrix()
      |> Matrix.setTranslation(((-0.5), 0., (-2.))),
      (0., 0., 1.),
    ),
    program1,
    gl,
  );

  Gl.bindBuffer(Gl.getElementArrayBuffer(gl), indexBuffer3, gl);

  Gl.drawElements(
    Gl.getTriangles(gl),
    indices3 |> Js.Typed_array.Uint16Array.length,
    Gl.getUnsignedShort(gl),
    0,
    gl,
  );
};

let _clearCanvas =
    (
      (
        gl,
        (program1, program2),
        (indices1, indices2, indices3),
        (vertexBuffer1, indexBuffer1),
        (vertexBuffer2, indexBuffer2),
        (vertexBuffer3, indexBuffer3),
        (vMatrix, pMatrix),
      ) as data,
    ) => {
  Gl.clear(Gl.getColorBufferBit(gl) lor Gl.getDepthBufferBit(gl), gl);

  data;
};

let _loopBody = data => {
  data |> _clearCanvas |> _render;
};

let rec _loop = data =>
  DomExtend.requestAnimationFrame((time: float) => {
    _loopBody(data);
    _loop(data) |> ignore;
  });

let main = () => {
  _init() |> _loop |> ignore;
};