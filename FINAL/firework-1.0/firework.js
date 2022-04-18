class Firework extends Particle {
  constructor() {
    super(
      random(9, 11),
      new p5.Vector(random(-100, 100), random(-100, 100), random(-100, 100)),
      random(360),
      new p5.Vector(random(-3, 3), 40, random(-3, 3))
    );
  }

  done() {
    return this.vel.y < 0;
  }

  explosion() {
    let particles = [];
    let amount = random(30, 40);
    for (let i = 0; i < amount; i++) {
      particles.push(
        new Particle(
          random(3, 4),
          new p5.Vector(this.pos.x, this.pos.y, this.pos.z),
          this.color,
          new p5.Vector(random(-5, 5), random(-5, 5), random(-5, 5))
        )
      );
    }
    return particles;
  }
}
