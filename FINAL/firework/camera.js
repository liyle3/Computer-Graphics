const camSpeed = 10;

class Camera {
  constructor(x, y, z, cX, cY, cZ, uX, uY, uZ) {
    this.cam = createCamera();
    this.cam.camera(x, y, z, cX, cY, cZ, uX, uY, uZ);
    this.fov = 60;
  }

  update() {
    perspective(this.fov, width / height, 0.1, 5000);
    // Drag mouse to change the view angle
    orbitControl();
    if (keyIsDown(65)) {
      // Use key'A'to make a left move
      this.cam.move(-camSpeed, 0, 0);
    }
    if (keyIsDown(68)) {
      // Use key'D'to make a right move
      this.cam.move(camSpeed, 0, 0);
    }
    if (keyIsDown(87)) {
      // Use key'W'to make a forward move
      this.cam.move(0, 0, -camSpeed);
    }
    if (keyIsDown(83)) {
      // Use key'S'to make a backward move
      this.cam.move(0, 0, camSpeed);
    }
    if (keyIsDown(67)) {
      // Use key'C'to make a upward move
      this.cam.move(0, camSpeed, 0);
    }
    if (keyIsDown(32)) {
      // Use key'Space'to make a downward move
      this.cam.move(0, -camSpeed, 0);
    }
    // console.log(
    //   this.cam.eyeX,
    //   this.cam.eyeY,
    //   this.cam.eyeZ,
    //   this.cam.centerX,
    //   this.cam.centerY,
    //   this.cam.centerZ,
    //   this.cam.upX,
    //   this.cam.upY,
    //   this.cam.upZ
    // );
  }
}
