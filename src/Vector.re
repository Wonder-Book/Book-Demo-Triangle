let dot = ((x, y, z), (vx, vy, vz)) => x *. vx +. y *. vy +. z *. vz;

let sub = ((x1, y1, z1), (x2, y2, z2)) => (x1 -. x2, y1 -. y2, z1 -. z2);

let scale = (scalar, (x, y, z)) => (x *. scalar, y *. scalar, z *. scalar);

let cross = ((x1, y1, z1), (x2, y2, z2)) => (
  y1 *. z2 -. y2 *. z1,
  z1 *. x2 -. z2 *. x1,
  x1 *. y2 -. x2 *. y1,
);

let normalize = ((x, y, z)) => {
  let d = Js.Math.sqrt(x *. x +. y *. y +. z *. z);
  d === 0. ? (0., 0., 0.) : (x /. d, y /. d, z /. d);
};