let particles = [];
let camera;
let images = [];

function setup() {
  images.push(loadImage('assets/floor6_d.png'));
  createCanvas(1280, 720, WEBGL);
  startStop();
  colorMode(HSB);
  angleMode(DEGREES);
  background(0);
  noStroke();
  camera = new Camera(50, -100, 600);
  drawingContext.shadowBlur = 10;
}

function draw() {
  colorMode(HSB);
  background(0);

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
  ambientLight(50);
  let h = width * 7 / 16;
  textureMode(NORMAL);
  textureWrap(MIRROR, MIRROR);
  push();
  translate(-width / 2, h, -width / 2);

  specularMaterial(10);
//   shininess(50);
//   box(width);
  texture(images[0]);
    vertex(0, -h, 0, 0, 0);
    vertex(width, -h, 0, 10, 0);
    vertex(width, -h, width, 10, 10);
    vertex(0, -h, width, 0, 10);
    endShape();
  pop();
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
