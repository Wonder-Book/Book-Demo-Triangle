type webgl1Context;

type program;

type shader;

type buffer;

/* type texture;

   type textureSource; */

type attributeLocation = int;

type uniformLocation;

/* type enum; */

type precisionFormat = {. "precision": int};

/* type extension; */

/* type vaoExtension;

   type framebuffer;

   type renderbuffer;

   type hex;

   external imageElementToTextureSource :
     DomExtendType.imageElement => textureSource =
     "%identity";

   external parameterIntToNullableProgram : int => Js.Nullable.t(program) =
     "%identity";

   external parameterIntToBuffer : int => buffer = "%identity";

   external parameterIntToNullableTexture : int => Js.Nullable.t(texture) =
     "%identity";

   external intToHex : int => hex = "%identity"; */

type contextConfigJsObj = {
  .
  "alpha": bool,
  "depth": bool,
  "stencil": bool,
  "antialias": bool,
  "premultipliedAlpha": bool,
  "preserveDrawingBuffer": bool,
};