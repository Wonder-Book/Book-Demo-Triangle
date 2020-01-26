type htmlElement = {
  .
  "width": int,
  "height": int,
};

type body;

type document = {. "body": body};

[@bs.val] external document: document = "";

[@bs.val] external requestAnimationFrame: (float => unit) => int = "";

[@bs.send] external querySelector: (document, string) => htmlElement = "";