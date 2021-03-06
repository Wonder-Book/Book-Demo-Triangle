let error = msg => Js.Exn.raiseError(msg);

let createTriangleVertexData = () => {
  open Js.Typed_array;

  let vertices =
    Float32Array.make([|
      0.,
      0.5,
      0.0,
      (-0.5),
      (-0.5),
      0.0,
      0.5,
      (-0.5),
      0.0,
    |]);

  let indices = Uint16Array.make([|0, 1, 2|]);

  (vertices, indices);
};

let _compileShader = (gl, glslSource: string, shader) => {
  WebGL1.shaderSource(shader, glslSource, gl);
  WebGL1.compileShader(shader, gl);

  WebGL1.getShaderParameter(shader, WebGL1.getCompileStatus(gl), gl)
  === false
    ? {
      let message = WebGL1.getShaderInfoLog(shader, gl);

      error(
        {j|shader info log: $message
        glsl source: $glslSource
        |j},
      );
    }
    : ();

  shader;
};

let _linkProgram = (program, gl) => {
  WebGL1.linkProgram(program, gl);

  WebGL1.getProgramParameter(program, WebGL1.getLinkStatus(gl), gl) === false
    ? {
      let message = WebGL1.getProgramInfoLog(program, gl);

      error({j|link program error: $message|j});
    }
    : ();
};

let initShader = (vsSource: string, fsSource: string, gl, program) => {
  let vs =
    _compileShader(
      gl,
      vsSource,
      WebGL1.createShader(WebGL1.getVertexShader(gl), gl),
    );
  let fs =
    _compileShader(
      gl,
      fsSource,
      WebGL1.createShader(WebGL1.getFragmentShader(gl), gl),
    );

  WebGL1.attachShader(program, vs, gl);
  WebGL1.attachShader(program, fs, gl);

  /*!
    if warn:"Attribute 0 is disabled. This has significant performance penalty" when run,
    then do this before linkProgram:
    gl.bindAttribLocation( this.glProgram, 0, "a_position");



    can reference here:
    http://stackoverflow.com/questions/20305231/webgl-warning-attribute-0-is-disabled-this-has-significant-performance-penalt?answertab=votes#tab-top


    OpenGL requires attribute zero to be enabled otherwise it will not render anything.
    On the other hand OpenGL ES 2.0 on which WebGL is based does not. So, to emulate OpenGL ES 2.0 on top of OpenGL if you don't enable attribute 0 the browser has to make a buffer for you large enough for the number of vertices you've requested to be drawn, fill it with the correct value (see gl.vertexAttrib),
    attach it to attribute zero, and enable it.

    It does all this behind the scenes but it's important for you to know that it takes time to create and fill that buffer. There are optimizations the browser can make but in the general case,
    if you were to assume you were running on OpenGL ES 2.0 and used attribute zero as a constant like you are supposed to be able to do, without the warning you'd have no idea of the work the browser is doing on your behalf to emulate that feature of OpenGL ES 2.0 that is different from OpenGL.

    require your particular case the warning doesn't have much meaning. It looks like you are only drawing a single point. But it would not be easy for the browser to figure that out so it just warns you anytime you draw and attribute 0 is not enabled.
    */
  /*!
    Always have vertex attrib 0 array enabled. If you draw with vertex attrib 0 array disabled, you will force the browser to do complicated emulation when running on desktop OpenGL (e.g. on Mac OSX). This is because in desktop OpenGL, nothing gets drawn if vertex attrib 0 is not array-enabled. You can use bindAttribLocation() to force a vertex attribute to use location 0, and use enableVertexAttribArray() to make it array-enabled.
    */
  WebGL1.bindAttribLocation(program, 0, "a_position", gl);

  _linkProgram(program, gl);

  /*!
    should detach and delete shaders after linking the program

    explain:
    The shader object, due to being attached to the program object, will continue to exist even if you delete the shader object. It will only be deleted by the system when it is no longer attached to any program object (and when the user has asked to delete it, of course).

    "Deleting" the shader, as with all OpenGL objects, merely sets a flag that says you don't need it any more. OpenGL will keep it around for as long as it needs it itself, and will do the actual delete any time later (most likely, but not necessarily, after the program is deleted).
    */
  WebGL1.deleteShader(vs, gl);
  WebGL1.deleteShader(fs, gl);

  program;
};

let initVertexBuffers = ((vertices, indices), gl) => {
  let vertexBuffer = WebGL1.createBuffer(gl);

  WebGL1.bindBuffer(WebGL1.getArrayBuffer(gl), vertexBuffer, gl);

  WebGL1.bufferFloat32Data(
    WebGL1.getArrayBuffer(gl),
    vertices,
    WebGL1.getStaticDraw(gl),
    gl,
  );

  let indexBuffer = WebGL1.createBuffer(gl);

  WebGL1.bindBuffer(WebGL1.getElementArrayBuffer(gl), indexBuffer, gl);

  WebGL1.bufferUint16Data(
    WebGL1.getElementArrayBuffer(gl),
    indices,
    WebGL1.getStaticDraw(gl),
    gl,
  );

  (vertexBuffer, indexBuffer);
};

let sendAttributeData = (vertexBuffer, program, gl) => {
  let positionLocation = WebGL1.getAttribLocation(program, "a_position", gl);

  positionLocation === (-1)
    ? error({j|Failed to get the storage location of a_position|j}) : ();

  WebGL1.bindBuffer(WebGL1.getArrayBuffer(gl), vertexBuffer, gl);

  WebGL1.vertexAttribPointer(
    positionLocation,
    3,
    WebGL1.getFloat(gl),
    false,
    0,
    0,
    gl,
  );
  WebGL1.enableVertexAttribArray(positionLocation, gl);
};

let _unsafeGetUniformLocation = (program, name, gl) =>
  switch (WebGL1.getUniformLocation(program, name, gl)) {
  | pos when !Js.Null.test(pos) => Js.Null.getUnsafe(pos)
  | _ => error({j|$name uniform not exist|j})
  };

let sendCameraUniformData = ((vMatrix, pMatrix), program, gl) => {
  let vMatrixLocation = _unsafeGetUniformLocation(program, "u_vMatrix", gl);
  let pMatrixLocation = _unsafeGetUniformLocation(program, "u_pMatrix", gl);

  WebGL1.uniformMatrix4fv(vMatrixLocation, false, vMatrix, gl);
  WebGL1.uniformMatrix4fv(pMatrixLocation, false, pMatrix, gl);
};

let _sendColorData = ((r, g, b), gl, colorLocation) =>
  WebGL1.uniform3f(colorLocation, r, g, b, gl);

let sendModelUniformData1 = ((mMatrix, color), program, gl) => {
  let mMatrixLocation = _unsafeGetUniformLocation(program, "u_mMatrix", gl);
  let colorLocation = _unsafeGetUniformLocation(program, "u_color0", gl);

  WebGL1.uniformMatrix4fv(mMatrixLocation, false, mMatrix, gl);
  _sendColorData(color, gl, colorLocation);
};

let sendModelUniformData2 = ((mMatrix, color1, color2), program, gl) => {
  let mMatrixLocation = _unsafeGetUniformLocation(program, "u_mMatrix", gl);
  let color1Location = _unsafeGetUniformLocation(program, "u_color0", gl);
  let color2Location = _unsafeGetUniformLocation(program, "u_color1", gl);

  WebGL1.uniformMatrix4fv(mMatrixLocation, false, mMatrix, gl);
  _sendColorData(color1, gl, color1Location);
  _sendColorData(color2, gl, color2Location);
};