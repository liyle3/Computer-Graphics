const G = -0.4;
const K = -0.03;

function writeColor(image, x, y, red, green, blue, alpha) {
  let index = (x + y * width) * 4;
  image.pixels[index] = red;
  image.pixels[index + 1] = green;
  image.pixels[index + 2] = blue;
  image.pixels[index + 3] = alpha;
}

class Particle {
  constructor(mass, pos, color, vel) {
    this.mass = mass;
    this.pos = pos;
    this.color = color;
    this.vel = vel;
    this.lifespan = 255;
  }

  update() {
    let resistance = p5.Vector.normalize(this.vel);
    resistance.mult(K * this.vel.mag() * this.vel.mag());
    let acc = p5.Vector.div(resistance, this.mass);
    acc.add(new p5.Vector(0, G, 0));
    this.vel.add(acc);
    this.pos.add(this.vel);
    this.lifespan -= 8;
  }

  done() {
    return this.lifespan < 0;
  }

  explosion() {
    return [];
  }

  show() {
    colorMode(HSB);
    pointLight(this.color, 255, this.lifespan, this.pos.x, -this.pos.y, this.pos.z);

    push();
    translate(this.pos.x, -this.pos.y, this.pos.z);
    emissiveMaterial(this.color, 255, this.lifespan);
    sphere(this.mass / 2);
    pop();
  }
}
