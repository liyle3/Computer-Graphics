let particles = [];
let camera;
let img;


// Load the image in the
// preload function
function preload() {
  img = loadImage('img/sun.jpg')
}

function setup() {
  createCanvas(1280, 720, WEBGL);
  startStop();
  colorMode(HSB);
  angleMode(DEGREES);
  stroke(255);
  strokeWeight(4);
  background(0);
  camera = new Camera(50, -250, 600);
}

function draw() {
  colorMode(RGB);
  blendMode(BLEND);
  background(0, 0, 0, 25);
  //ground
//   normalMaterial();
//   fill(100);
//   textureMode(NORMAL);
//   specularMaterial();
  texture(img);
  beginShape();
//   specularMaterial(120);
  vertex(-500, 0, -500);
  vertex(-500, 0, 500);
  vertex(500, 0, 500);
  vertex(500, 0, -500);
  endShape(CLOSE);
//   specularMaterial(120);
//   plane(200, 200);

  drawAxes(width / 2);
  camera.update();

  if (random(1000) < 15) {
    particles.push(new Firework());
  }

  for (let i = 0; i < particles.length; i++) {
    particles[i].update();
    particles[i].show();
    if (particles[i].done()) {
      particles.push(...particles[i].explosion());
      particles.splice(i, 1);
    }
  }
}

function startStop() {
  let div = createDiv();
  let button = createButton("start");
  let button2 = createButton("stop");

  button.parent(div);
  button2.parent(div);
  div.center("horizontal");
  // noLoop();

  button.mousePressed(loop);
  button2.mousePressed(noLoop);
}

function mouseWheel(event) {
  camera.fov -= event.delta * 0.01;
  camera.fov = max(60, camera.fov);
  camera.fov = min(100, camera.fov);
}

function drawAxes(length) {
  const LL = length * 0.95;
  const SL = length * 0.05;

  // x
  stroke(255, 0, 0);
  line(0, 0, length, 0);
  line(length, 0, LL, SL);
  line(length, 0, LL, -SL);

  // y
  stroke(0, 255, 0);
  line(0, 0, 0, length);
  line(0, length, SL, LL);
  line(0, length, -SL, LL);

  // z
  stroke(0, 0, 255);
  line(0, 0, 0, 0, 0, length);
  line(0, 0, length, SL, 0, LL);
  line(0, 0, length, -SL, 0, LL);
}
