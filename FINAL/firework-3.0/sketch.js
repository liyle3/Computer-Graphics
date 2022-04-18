let particles = [];
let camera;
let images = [];
let skybox;


function setup() {
//   images.push(loadImage('assets/skybox/box.jpg'));
  images.push(loadImage('assets/skybox/back.jpg'));
  images.push(loadImage('assets/skybox/bottom.jpg'));
  images.push(loadImage('assets/skybox/front.jpg'));
  images.push(loadImage('assets/skybox/left.jpg'));
  images.push(loadImage('assets/skybox/right.jpg'));
  images.push(loadImage('assets/skybox/top.jpg'));
  images.push(loadImage('assets/floor6_d.png'));
//   images.push(loadImage('assets/skybox/sleepyhollow_bk.jpg'));
//   images.push(loadImage('assets/skybox/sleepyhollow_dn.png'));
//   images.push(loadImage('assets/skybox/sleepyhollow_ft.jpg'));
//   images.push(loadImage('assets/skybox/sleepyhollow_lf.jpg'));
//   images.push(loadImage('assets/skybox/sleepyhollow_rt.jpg'));
//   images.push(loadImage('assets/skybox/sleepyhollow_up.jpg'));
  createCanvas(1280, 720, WEBGL);
  startStop();
  colorMode(HSB);
  angleMode(DEGREES);
  background(0);
  noStroke();
//   textureWrap(REPEAT);
  camera = new Camera(100, -100, 600);
  skybox = new skyBox(images, width);
  drawingContext.shadowBlur = 10;
}

function draw() {
  colorMode(HSB);
  background(0);

  camera.update();
//   texture(images[0])
//   box(100);
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
  skybox.draw();
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
  camera.fov -= event.delta * 0.003;
  camera.fov = max(60, camera.fov);
  camera.fov = min(80, camera.fov);
}
