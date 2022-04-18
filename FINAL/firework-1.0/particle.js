const G = -0.4;
const K = -0.03; // -0.01

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
    strokeWeight(this.mass);
    stroke(this.color, 255, 255, this.lifespan);
    point(this.pos.x, -this.pos.y, this.pos.z);
  }
}
