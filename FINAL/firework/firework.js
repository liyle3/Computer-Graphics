const explosionSpeedMin = 0;
const explosionSpeedMax = 8;
const fireworkSpeed = 40;
const explosionAmount = 100;
const fireworkWeight = 10;
const particleWeight = 4;
const maxFireworkCount = 10;

class Firework extends Particle {
  constructor() {
    super(
      fireworkWeight,
      new p5.Vector(random(-500, 500), 0, random(-100, 100)),
      random(180),
      new p5.Vector(0, fireworkSpeed, 0)
    );
    this.lifespan = 32767;
  }

  done() {
    return this.vel.y < 0;
  }

  explosion() {
    let particles = [];
    for (let i = 0; i < explosionAmount; i++) {
      let newParticle = new Particle(
        particleWeight,
        new p5.Vector(this.pos.x, this.pos.y, this.pos.z),
        this.color,
        p5.Vector.random3D()
      );
      newParticle.vel.mult(random(explosionSpeedMin, explosionSpeedMax));
      particles.push(newParticle);
    }
    return particles;
  }
}
