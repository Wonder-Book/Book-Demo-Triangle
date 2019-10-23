type htmlElement = {
  .
  "width": int,
  "height": int,
};

type body;

type document = {. "body": body};

[@bs.val] external document: document = "";

/*

 [@bs.send] external createElement: (document, string) => Js.t({..}) = ""; */

/* [@bs.return null_to_opt] [@bs.send]
   external querySelectorAll: (document, string) => option(htmlElement) = ""; */

[@bs.val] external requestAnimationFrame: (float => unit) => int = "";

/* [@bs.send]
   external querySelectorAll: (document, string) => array(htmlElement) = ""; */

[@bs.send] external querySelector: (document, string) => htmlElement = "";