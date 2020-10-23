Core 1's sole duty is to transfer data from renderBuffer to SPI.

```
Core1() {
  Global::SPI_rendering = false;
  while (1) {
    while (!Global::renderBufferReady) {
      usleep(1);
    }
    Global::renderBufferReady = false;
    Global::SPI_rendering = true;
    SPI_WaitForQueueDone();// <-----
    SPI_xfer(renderBuffer);
    Global::SPI_rendering = false;
  }
}

Core2() {
  Global::renderBufferReady = false;
  while (1) {
    RenderGameFrame(); // to offscreenBuffer
// this is Display::paint():
    while (Global::SPI_rendering) {
      usleep(1);
    }
    //  swap renderBuffer and offscreenBuffer
    Global::renderBufferReady = true;
  }
  
}
```

