const G = -0.22;
const K = -0.01;
const lifespanCost = 8;

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
    this.lifespan -= lifespanCost;
  }

  done() {
    return this.lifespan < 50;
  }

  explosion() {
    return [];
  }

  show() {
    if (this.lifespan > 200) {
      pointLight(
        this.color,
        255,
        this.lifespan,
        this.pos.x,
        this.pos.y,
        this.pos.z
      );
    }
    push();
    translate(this.pos.x, -this.pos.y, this.pos.z);
    emissiveMaterial(this.color, 255, this.lifespan);
    sphere(this.mass / 2.0);
    pop();
  }
}
