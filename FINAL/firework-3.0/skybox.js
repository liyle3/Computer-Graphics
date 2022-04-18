class skyBox
{
  constructor(images, size)
  {
    this.images = images;
    this.size = size;
  }

  draw()
  {
    let h = width * 7 / 16;
    textureMode(NORMAL);
    textureWrap(MIRROR, MIRROR);
    push();
    translate(-width / 2, h, -width / 2);
    specularMaterial(10);
    // back
    beginShape();
    texture(this.images[0]);
    vertex(0, -width, width, 0, 0);
    vertex(width, -width, width, 1, 0);
    vertex(width, 0, width, 1, 1);
    vertex(0, 0, width, 0, 1);
    endShape();
    // bottom
    beginShape();
    texture(this.images[1]);
    vertex(0, 0, 0, 0, 0);
    vertex(width, 0, 0, 1, 0);
    vertex(width, 0, width, 1, 1);
    vertex(0, 0, width, 0, 1);
    endShape();
    // front
    beginShape();
    texture(this.images[2]);
    vertex(0, -width, 0, 1, 0);
    vertex(width, -width, 0, 0, 0);
    vertex(width, 0, 0, 0, 1);
    vertex(0, 0, 0, 1, 1);
    endShape();
    // left
    beginShape();
    texture(this.images[3]);
    vertex(0, -width, 0, 0, 0);
    vertex(0, -width, width, 1, 0);
    vertex(0, 0, width, 1, 1);
    vertex(0, 0, 0, 0, 1);
    endShape();
    // right
    beginShape();
    texture(this.images[4]);
    vertex(width, -width, 0, 1, 0);
    vertex(width, -width, width, 0, 0);
    vertex(width, 0, width, 0, 1);
    vertex(width, 0, 0, 1, 1);
    endShape();
    // top
    beginShape();
    texture(this.images[5]);
    vertex(0, -width, 0, 0, 0);
    vertex(width, -width, 0, 1, 0);
    vertex(width, -width, width, 1, 1);
    vertex(0, -width, width, 0, 1);
    endShape();
    // plane
    beginShape();
    texture(this.images[6]);
    vertex(0, -h, 0, 0, 0);
    vertex(width, -h, 0, 10, 0);
    vertex(width, -h, width, 10, 10);
    vertex(0, -h, width, 0, 10);
    endShape();
    pop();
  }
}