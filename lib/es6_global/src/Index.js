

import * as React from "react";
import * as ReactDOMRe from "../../../node_modules/reason-react/lib/es6_global/src/ReactDOMRe.js";
import * as Test$TinyWonderEditor from "./test.js";

ReactDOMRe.renderToElementWithId(React.createElement(Test$TinyWonderEditor.make, {
          name: "John"
        }), "greeting");

export {
  
}
/*  Not a pure module */
