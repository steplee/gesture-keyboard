//var keyboard_layout = require("../layouts/basic.json");
var keyboard_layout =  {
"width": 0.8,
"height": 0.9,
"keys": {
"q":[0.04999999999999999,0.09999999999999998],
"w":[0.04999999999999999,0.18],
"e":[0.04999999999999999,0.26],
"r":[0.04999999999999999,0.33999999999999997],
"t":[0.04999999999999999,0.42000000000000004],
"y":[0.04999999999999999,0.5],
"u":[0.04999999999999999,0.58],
"i":[0.04999999999999999,0.6599999999999999],
"o":[0.04999999999999999,0.7400000000000001],
"p":[0.04999999999999999,0.8200000000000001],
"a":[0.35,0.11999999999999998],
"s":[0.35,0.19999999999999998],
"d":[0.35,0.28],
"f":[0.35,0.36],
"g":[0.35,0.44000000000000006],
"h":[0.35,0.52],
"j":[0.35,0.6],
"k":[0.35,0.6799999999999999],
"l":[0.35,0.7600000000000001],
"z":[0.6499999999999999,0.16499999999999998],
"x":[0.6499999999999999,0.245],
"c":[0.6499999999999999,0.325],
"v":[0.6499999999999999,0.40499999999999997],
"b":[0.6499999999999999,0.48500000000000004],
"n":[0.6499999999999999,0.565],
"m":[0.6499999999999999,0.645]
}
}

function init() {
  console.log(" -- Frontend -- ");

  let sock = new WebSocket("ws://localhost:9002"),
      canvas = document.getElementById("the_canvas"),
      ctx = canvas.getContext("2d");

  render_keyboard();

  sock.onmessage = function(d) {
    console.log("received",d)
  }

  var firstTime = Date.now();
  var lastPos = undefined;
  var lastTime = firstTime;

  window.onmousemove = function(e) {
    let x = e.clientX,
        y = e.clientY,
        t = Date.now();

    if (Date.now() - lastTime > 333) {
      if (lastPos == undefined) {
        sock.send(JSON.stringify({type:"begin"}))
      } else {
        // Draw
        ctx.beginPath();
        ctx.moveTo(lastPos[0],lastPos[1]);
        ctx.lineTo(x,y);
        ctx.stroke();
      }

      // Send
      let xx = x/canvas.width,
          yy = y/canvas.height;
      var jobj = {
        type: "move",
        time: (t-firstTime)/1000.0,
        x:xx,y:yy
      }
      sock.send(JSON.stringify(jobj));

      lastPos = [x,y];
      lastTime = t;
    }
  }

  window.onmousedown = function(e) {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    render_keyboard();
    lastPos = undefined;
    lastTime = firstTime;
    sock.send(JSON.stringify({type:"end"}))
    sock.send(JSON.stringify({type:"begin"}))
  }

  function render_keyboard() {
    let _w = canvas.width,
        _h = canvas.height;
    for (var ch of Object.keys(keyboard_layout.keys)) {
      var yx = keyboard_layout.keys[ch];
      ctx.fillText(ch, yx[1]*_w, yx[0]*_h);
    }
  }


  window.onbeforeunload = function (e) {
    sock.close();
  }
}
