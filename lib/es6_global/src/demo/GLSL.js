


var vs1 = "\n  precision mediump float;\n  attribute vec3 a_position;\n  uniform mat4 u_pMatrix;\n  uniform mat4 u_vMatrix;\n  uniform mat4 u_mMatrix;\n\n  void main() {\n    /* gl_Position = u_pMatrix * u_vMatrix * u_mMatrix * vec4(a_position, 1.0); */\n    gl_Position = u_pMatrix * u_vMatrix * u_mMatrix * vec4(a_position, 1.0);\n  }\n    ";

var fs1 = "\n    precision mediump float;\n\n    uniform vec3 u_color;\n\n    void main(){\n        gl_FragColor = vec4(u_color,1.0);\n    }\n    ";

var vs2 = "\n  precision mediump float;\n  attribute vec3 a_position;\n  uniform mat4 u_pMatrix;\n  uniform mat4 u_vMatrix;\n  uniform mat4 u_mMatrix;\n\n  void main() {\n    gl_Position = u_pMatrix * u_vMatrix * u_mMatrix * vec4(a_position, 1.0);\n  }\n    ";

var fs2 = "\n    precision mediump float;\n\n    uniform vec3 u_color;\n\n    void main(){\n        gl_FragColor = vec4(u_color,0.2);\n    }\n    ";

export {
  vs1 ,
  fs1 ,
  vs2 ,
  fs2 ,
  
}
/* No side effect */
