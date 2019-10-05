

import * as React from "react";

function Test(Props) {
  var name = Props.name;
  return React.createElement("button", undefined, "Hello " + (name + "!"));
}

var make = Test;

export {
  make ,
  
}
/* react Not a pure module */
