let vs1 = {|
  precision mediump float;
  attribute vec3 a_position;
  uniform mat4 u_pMatrix;
  uniform mat4 u_vMatrix;
  uniform mat4 u_mMatrix;

  void main() {
    /* gl_Position = u_pMatrix * u_vMatrix * u_mMatrix * vec4(a_position, 1.0); */
    gl_Position = u_pMatrix * u_vMatrix * u_mMatrix * vec4(a_position, 1.0);
  }
    |};

let fs1 = {|
    precision mediump float;

    uniform vec3 u_color0;

    void main(){
        gl_FragColor = vec4(u_color0,1.0);
    }
    |};

let vs2 = {|
  precision mediump float;
  attribute vec3 a_position;
  uniform mat4 u_pMatrix;
  uniform mat4 u_vMatrix;
  uniform mat4 u_mMatrix;

  void main() {
    gl_Position = u_pMatrix * u_vMatrix * u_mMatrix * vec4(a_position, 1.0);
  }
    |};

let fs2 = {|
    precision mediump float;

    uniform vec3 u_color0;
    uniform vec3 u_color1;

    void main(){
        gl_FragColor = vec4(u_color0 * u_color1,1.0);
    }
    |};