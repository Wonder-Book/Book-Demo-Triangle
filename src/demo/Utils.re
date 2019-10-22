let error = msg => Js.Exn.raiseError(msg) |> ignore;

let createTriangleGeometryData = () => {
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
  Gl.shaderSource(shader, glslSource, gl);
  Gl.compileShader(shader, gl);

  /* TODO optimize */
  Gl.getShaderParameter(shader, Gl.getCompileStatus(gl), gl) === false ?
    {
      let message = Gl.getShaderInfoLog(shader, gl);

      error(
        {j|shader info log: $message
        glsl source: $glslSource
        |j},
      );
      /* Js.log({j|shader info log: $message|j});
         Js.log({j|glsl source: $glslSource|j}); */
    } :
    ();

  shader;
};

let _linkProgram = (program, gl) => {
  Gl.linkProgram(program, gl);

  /* TODO optimize */
  Gl.getProgramParameter(program, Gl.getLinkStatus(gl), gl) === false ?
    {
      let message = Gl.getProgramInfoLog(program, gl);

      /* Js.log({j|link program error: $message|j}); */
      error({j|link program error: $message|j});
    } :
    ();
};

let initShader = (vsSource: string, fsSource: string, gl, program) => {
  let vs =
    _compileShader(
      gl,
      vsSource,
      Gl.createShader(Gl.getVertexShader(gl), gl),
    );
  let fs =
    _compileShader(
      gl,
      fsSource,
      Gl.createShader(Gl.getFragmentShader(gl), gl),
    );

  Gl.attachShader(program, vs, gl);
  Gl.attachShader(program, fs, gl);

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
  Gl.bindAttribLocation(program, 0, "a_position", gl);

  _linkProgram(program, gl);

  /*!
    should detach and delete shaders after linking the program

    explain:
    The shader object, due to being attached to the program object, will continue to exist even if you delete the shader object. It will only be deleted by the system when it is no longer attached to any program object (and when the user has asked to delete it, of course).

    "Deleting" the shader, as with all OpenGL objects, merely sets a flag that says you don't need it any more. OpenGL will keep it around for as long as it needs it itself, and will do the actual delete any time later (most likely, but not necessarily, after the program is deleted).
    */
  Gl.deleteShader(vs, gl);
  Gl.deleteShader(fs, gl);

  program;
};

let initVertexBuffers = ((vertices, indices), gl) => {
  let vertexBuffer = Gl.createBuffer(gl);

  Gl.bindBuffer(Gl.getArrayBuffer(gl), vertexBuffer, gl);

  Gl.bufferFloat32Data(
    Gl.getArrayBuffer(gl),
    vertices,
    Gl.getStaticDraw(gl),
    gl,
  );

  let indexBuffer = Gl.createBuffer(gl);

  Gl.bindBuffer(Gl.getElementArrayBuffer(gl), indexBuffer, gl);

  Gl.bufferUint16Data(
    Gl.getElementArrayBuffer(gl),
    indices,
    Gl.getStaticDraw(gl),
    gl,
  );

  (vertexBuffer, indexBuffer);
};

let sendAttributeData = (vertexBuffer, program, gl) => {
  let positionLocation = Gl.getAttribLocation(program, "a_position", gl);

  positionLocation === (-1) ?
    error({j|Failed to get the storage location of a_position|j}) : ();

  Gl.bindBuffer(Gl.getArrayBuffer(gl), vertexBuffer, gl);

  Gl.vertexAttribPointer(
    positionLocation,
    3,
    Gl.getFloat(gl),
    false,
    0,
    0,
    gl,
  );
  Gl.enableVertexAttribArray(positionLocation, gl);
};

let sendCameraUniformData = ((vMatrix, pMatrix), program, gl) => {
  let vMatrixLocation = Gl.getUniformLocation(program, "u_vMatrix", gl);
  let pMatrixLocation = Gl.getUniformLocation(program, "u_pMatrix", gl);

  Gl.uniformMatrix4fv(vMatrixLocation, false, vMatrix, gl);
  Gl.uniformMatrix4fv(pMatrixLocation, false, pMatrix, gl);
};

let _sendColorData = ((r, g, b), gl, colorLocation) =>
  Gl.uniform3f(colorLocation, r, g, b, gl);

let sendModelUniformData1 = ((mMatrix, color), program, gl) => {
  let mMatrixLocation = Gl.getUniformLocation(program, "u_mMatrix", gl);
  let colorLocation = Gl.getUniformLocation(program, "u_color0", gl);

  Gl.uniformMatrix4fv(mMatrixLocation, false, mMatrix, gl);
  _sendColorData(color, gl, colorLocation);
};

let sendModelUniformData2 = ((mMatrix, color1, color2), program, gl) => {
  let mMatrixLocation = Gl.getUniformLocation(program, "u_mMatrix", gl);
  let color1Location = Gl.getUniformLocation(program, "u_color0", gl);
  let color2Location = Gl.getUniformLocation(program, "u_color1", gl);

  Gl.uniformMatrix4fv(mMatrixLocation, false, mMatrix, gl);
  _sendColorData(color1, gl, color1Location);
  _sendColorData(color2, gl, color2Location);
};