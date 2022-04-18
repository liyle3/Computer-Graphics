let particles = [];
let fireworkCount = 0;
let camera;
let images = [];
let skybox;
let boomSound;
let backgroundSound;

function preload() {
  backgroundSound = loadSound("assets/whistle.mp3");
  boomSound = loadSound("assets/boom.mp3");
  images.push(loadImage("assets/skybox/posz.jpg"));
  images.push(loadImage("assets/skybox/negz.jpg"));
  images.push(loadImage("assets/skybox/negx.jpg"));
  images.push(loadImage("assets/skybox/posx.jpg"));
  images.push(loadImage("assets/skybox/posy.jpg"));
  images.push(loadImage("assets/skybox/negy.jpg"));
}

function setup() {
  createCanvas(1280, 720, WEBGL);
  createButtons();

  colorMode(HSB);
  background(0);
  angleMode(DEGREES);
  noStroke();
  camera = new Camera(15, -325, 690, -23, -508, 194, 0, 1, 0);
  skybox = new skyBox(images);
}

function draw() {
  background(0);
  lightFalloff(0.8, 0.00, 0);

  camera.update();
  updateParticles();
//   lights();
//   ambientLight(50);
  skybox.draw();
  let w = 2048;
  let h = (w * 6) / 16;
  //plane
    // push();
    // beginShape();
    // specularMaterial(20);
    // vertex(0, -h, 0, 0, 0);
    // vertex(w, -h, 0, 10, 0);
    // vertex(w, -h, w, 10, 10);
    // vertex(0, -h, w, 0, 10);
    // endShape();
    // pop();
}

function createButtons() {
  let div = createDiv();
  let button = createButton("start");
  let button2 = createButton("stop");

  button.parent(div);
  button2.parent(div);
  div.center("horizontal");

  noLoop();

  button.mousePressed(function () {
    loop();
    backgroundSound.loop();
  });
  button2.mousePressed(function () {
    noLoop();
    backgroundSound.stop();
  });
}

function updateParticles() {
  if (random(1000) < 30 && fireworkCount < maxFireworkCount) {
    particles.push(new Firework());
    fireworkCount++;
  }

  for (let i = 0; i < particles.length; i++) {
    particles[i].update();
    particles[i].show();
    if (particles[i].done()) {
      let newParticles = particles[i].explosion();
      if (newParticles.length != 0) {
        particles.push(...newParticles);
        boomSound.play();
        fireworkCount--;
      }
      particles.splice(i, 1);
    }
  }
}

function mouseWheel(event) {
  camera.fov -= event.delta * 0.003;
  camera.fov = max(60, camera.fov);
  camera.fov = min(80, camera.fov);
}
