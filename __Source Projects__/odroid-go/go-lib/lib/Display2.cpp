#include "Display2.h"


#pragma GCC optimize ("O3")


#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "esp_event.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "driver/ledc.h"
#include "driver/rtc_io.h"

#include <string.h>


const int DUTY_MAX = 0x1fff;

const gpio_num_t SPI_PIN_NUM_MISO = GPIO_NUM_19;
const gpio_num_t SPI_PIN_NUM_MOSI = GPIO_NUM_23;
const gpio_num_t SPI_PIN_NUM_CLK  = GPIO_NUM_18;

const gpio_num_t LCD_PIN_NUM_CS   = GPIO_NUM_5;
const gpio_num_t LCD_PIN_NUM_DC   = GPIO_NUM_21;
const gpio_num_t LCD_PIN_NUM_BCKL = GPIO_NUM_14;
const int LCD_BACKLIGHT_ON_VALUE = 1;
const int LCD_SPI_CLOCK_RATE = 40000000;


#define SPI_TRANSACTION_COUNT (4)
static spi_transaction_t trans[SPI_TRANSACTION_COUNT];
static spi_device_handle_t spi;


#define LINE_BUFFERS (2)
#define PARALLEL_LINES (5)
uint16_t* line[LINE_BUFFERS];
QueueHandle_t spi_queue;
QueueHandle_t line_buffer_queue;

SemaphoreHandle_t line_semaphore;
SemaphoreHandle_t spi_empty;
SemaphoreHandle_t spi_count_semaphore;


bool isBackLightIntialized = false;


 // GB


#define SMS_WIDTH (256)
#define SMS_HEIGHT (192)

//WTF?
#define PIXEL_MASK      (0x1F)


SemaphoreHandle_t sms_mutex = NULL;




/*
 The ILI9341 needs a bunch of command/argument values to be initialized. They are stored in this struct.
*/
typedef struct {
  uint8_t cmd;
  uint8_t data[128];
  uint8_t databytes; //No of data in data; bit 7 = delay after set; 0xFF = end of cmds.
} ili_init_cmd_t;

#define TFT_CMD_SWRESET 0x01
#define TFT_CMD_SLEEP 0x10
#define TFT_CMD_DISPLAY_OFF 0x28

#define MADCTL_MY   0x80
#define MADCTL_MX   0x40
#define MADCTL_MV   0x20
#define MADCTL_ML   0x10
#define MADCTL_MH   0x04
#define TFT_RGB_BGR 0x08
#define TFT_RGB 0x00




// 2.4" LCD
DRAM_ATTR static const ili_init_cmd_t ili_init_cmds[] = {
  // VCI=2.8V
  //************* Start Initial Sequence **********//
  {TFT_CMD_SWRESET, {0}, 0x80},
  {0xCF, {0x00, 0xc3, 0x30}, 3},
  {0xED, {0x64, 0x03, 0x12, 0x81}, 4},
  {0xE8, {0x85, 0x00, 0x78}, 3},
  {0xCB, {0x39, 0x2c, 0x00, 0x34, 0x02}, 5},
  {0xF7, {0x20}, 1},
  {0xEA, {0x00, 0x00}, 2},
  {0xC0, {0x1B}, 1},  //Power control   //VRH[5:0]
  {0xC1, {0x12}, 1},  //Power control   //SAP[2:0];BT[3:0]
  {0xC5, {0x32, 0x3C}, 2},  //VCM control
  {0xC7, {0x91}, 1},  //VCM control2
  //{0x36, {(MADCTL_MV | MADCTL_MX | TFT_RGB_BGR)}, 1},  // Memory Access Control
  // MV = flip vertical, MY = flip horizontal
  {0x36, {(MADCTL_MV | MADCTL_MY | TFT_RGB_BGR)}, 1},  // Memory Access Control
  {0x3A, {0x55}, 1},
  {0xB1, {0x00, 0x1B}, 2},  // Frame Rate Control (1B=70, 1F=61, 10=119)
  {0xB6, {0x0A, 0xA2}, 2},  // Display Function Control
  // {0xF6, {0x01, 0x30}, 2}, // production
  //            0b00110000 
  {0xF6, {0x01, 0b00100000}, 2}, // test
  {0xF2, {0x00}, 1},  // 3Gamma Function Disable
  {0x26, {0x01}, 1},   //Gamma curve selected

  //Set Gamma
  {0xE0, {0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00}, 15},
  {0XE1, {0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F}, 15},

/*
  // LUT
  {0x2d, {0x01, 0x03, 0x05, 0x07, 0x09, 0x0b, 0x0d, 0x0f, 0x11, 0x13, 0x15, 0x17, 0x19, 0x1b, 0x1d, 0x1f,
      0x21, 0x23, 0x25, 0x27, 0x29, 0x2b, 0x2d, 0x2f, 0x31, 0x33, 0x35, 0x37, 0x39, 0x3b, 0x3d, 0x3f,
      0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,
      0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c,
      0x1d, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x26, 0x27, 0x28, 0x29, 0x2a,
      0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
      0x00, 0x00, 0x02, 0x04, 0x06, 0x08, 0x0a, 0x0c, 0x0e, 0x10, 0x12, 0x12, 0x14, 0x16, 0x18, 0x1a,
      0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38}, 128},
*/

  {0x11, {0}, 0x80},  //Exit Sleep
  {0x29, {0}, 0x80},  //Display on

  {0, {0}, 0xff}
};



uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
    return ((b & 0xF8) << 8) | ((r & 0xFC) << 3) | ((g & 0xF8) >> 3);
}


void lock_display() {
  if (!sms_mutex) {
    sms_mutex = xSemaphoreCreateMutex();
    if (!sms_mutex) abort();
  }

  if (xSemaphoreTake(sms_mutex, 1000 / portTICK_RATE_MS) != pdTRUE) {
    abort();
  }
}

void unlock_display()
{
  if (!sms_mutex) abort();

  xSemaphoreGive(sms_mutex);
}



static uint16_t* line_buffer_get() {
  uint16_t* buffer;
  if (xQueueReceive(line_buffer_queue, &buffer, 1000 / portTICK_RATE_MS) != pdTRUE) {
    abort();
  }

  return buffer;
}

void line_buffer_put(uint16_t* buffer) {
  if (xQueueSend(line_buffer_queue, &buffer, 1000 / portTICK_RATE_MS) != pdTRUE) {
    abort();
  }
}

static void spi_task(void *arg) {
  printf("Display2 %s: Entered.\n", __func__);

  uint16_t* param;
  
  while(true) {
    // Ensure only LCD transactions are pulled
    if(xSemaphoreTake(spi_count_semaphore, portMAX_DELAY) == pdTRUE ) {
      spi_transaction_t* t;

      esp_err_t ret = spi_device_get_trans_result(spi, &t, portMAX_DELAY);
      assert(ret==ESP_OK);

      int dc = (int)t->user & 0x80;
      
      if (dc) {
        xSemaphoreGive(line_semaphore);
        line_buffer_put((uint16_t *)t->tx_buffer);
      }

      if (xQueueSend(spi_queue, &t, portMAX_DELAY) != pdPASS) {
        abort();
      }

      if (uxQueueMessagesWaiting(spi_queue) >= SPI_TRANSACTION_COUNT) {
        xSemaphoreGive(spi_empty);
      }
    }
    else {
      printf("%s: xSemaphoreTake failed.\n", __func__);
    }
  }

  printf("%s: Exiting.\n", __func__);
  vTaskDelete(NULL);

  while (1) {}
}

static void initialize_spi() {
  spi_queue = xQueueCreate(SPI_TRANSACTION_COUNT, sizeof(void*));
  if(!spi_queue) abort();


  line_buffer_queue = xQueueCreate(LINE_BUFFERS, sizeof(void*));
  if(!line_buffer_queue) abort();

  line_semaphore = xSemaphoreCreateCounting(LINE_BUFFERS, LINE_BUFFERS);
  if (!line_semaphore) abort();

  spi_empty = xSemaphoreCreateBinary();
  if(!spi_empty) abort();

  xSemaphoreGive(spi_empty);

  spi_count_semaphore = xSemaphoreCreateCounting(SPI_TRANSACTION_COUNT, 0);
  if (!spi_count_semaphore) abort();

  xTaskCreatePinnedToCore(
    &spi_task, 
    "spi_task", 
    1024 + 768, 
    NULL, 
    5, 
    NULL, 
    1
  );
}



static spi_transaction_t* spi_get_transaction()
{
  spi_transaction_t* t;
  xQueueReceive(spi_queue, &t, portMAX_DELAY);

  memset(t, 0, sizeof(*t));

  return t;
}

static void spi_put_transaction(spi_transaction_t* t)
{
  t->rx_buffer = NULL;
  t->rxlength = t->length;

  if (t->flags & SPI_TRANS_USE_TXDATA)
  {
    t->flags |= SPI_TRANS_USE_RXDATA;
  }

  if (uxSemaphoreGetCount(spi_empty) > 0)
  {
    if(xSemaphoreTake(spi_empty, portMAX_DELAY) != pdTRUE)
    {
      abort();
    }
  }

  esp_err_t ret = spi_device_queue_trans(spi, t, portMAX_DELAY);
  assert(ret==ESP_OK);

  xSemaphoreGive(spi_count_semaphore);
}


//Send a command to the ILI9341. Uses spi_device_transmit, which waits until the transfer is complete.
static void display_cmd(const uint8_t cmd)
{
  spi_transaction_t* t = spi_get_transaction();

  t->length = 8;           //Command is 8 bits
  t->tx_data[0] = cmd;         //The data is the cmd itself
  t->user = (void*)0;        //D/C needs to be set to 0
  t->flags = SPI_TRANS_USE_TXDATA;

  spi_put_transaction(t);
}

//Send data to the ILI9341. Uses spi_device_transmit, which waits until the transfer is complete.
static void display_data(const uint8_t *data, int len) {
  if (len) {
    spi_transaction_t* t = spi_get_transaction();

    if (len < 5) {
      for (int i = 0; i < len; ++i) {
        t->tx_data[i] = data[i];
      }
      t->length = len * 8;         //Len is in bytes, transaction length is in bits.
      t->user = (void*)1;        //D/C needs to be set to 1
      t->flags = SPI_TRANS_USE_TXDATA;
    }
    else {
      t->length = len * 8;         //Len is in bytes, transaction length is in bits.
      t->tx_buffer = data;         //Data
      t->user = (void*)1;        //D/C needs to be set to 1
      t->flags = 0; //SPI_TRANS_USE_TXDATA;
    }

    spi_put_transaction(t);
  }
}

//This function is called (in irq context!) just before a transmission starts. It will
//set the D/C line to the value indicated in the user field.
static void spi_pre_transfer_callback(spi_transaction_t *t) {
  int dc=(int)t->user & 0x01;
  gpio_set_level(LCD_PIN_NUM_DC, dc);
}

//Initialize the display
void send_display_boot_program() {
  int cmd = 0;

  //Initialize non-SPI GPIOs
  gpio_set_direction(LCD_PIN_NUM_DC, GPIO_MODE_OUTPUT);
  gpio_set_direction(LCD_PIN_NUM_BCKL, GPIO_MODE_OUTPUT);

  //Send all the commands
  while (ili_init_cmds[cmd].databytes != 0xff) {
    display_cmd(ili_init_cmds[cmd].cmd);

    int len = ili_init_cmds[cmd].databytes & 0x7f;
    if (len) display_data(ili_init_cmds[cmd].data, len);

    if (ili_init_cmds[cmd].databytes & 0x80) {
      vTaskDelay(100 / portTICK_RATE_MS);
    }

    cmd++;
  }
}


void send_reset_drawing(uint8_t left, uint8_t top, uint16_t width, uint8_t height) {
  // printf("%s: left:%i, top:%i, width:%i, height:%i\n", __func__, left, top, width, height);
  // fflush(stdout);

  display_cmd(0x2A); // Column Address Set

  // Casts deal with: error: narrowing conversion of '(((((int)left) + ((int)width)) + -1) >> 8)' from 'int' to 'uint8_t {aka unsigned char}' inside { } [-Werror=narrowing]
  const uint8_t data1[] = { 
    (uint8_t)(left >> 8), 
    (uint8_t)(left & 0xff), 
    (uint8_t)((left + width - 1) >> 8), 
    (uint8_t)((left + width - 1) & 0xff) 
  };

  display_data(data1, 4);

  display_cmd(0x2B);      //Page address set

  const uint8_t data2[] = { 
    (uint8_t)(top >> 8), 
    (uint8_t)(top & 0xff), 
    (uint8_t)((top + height - 1) >> 8), 
    (uint8_t)((top + height - 1) & 0xff) 
  };
  display_data(data2, 4);

  display_cmd(0x2C);       //memory write
}

// static void wait_for_line_buffer()
// {
//   // if(xSemaphoreTake(line_semaphore, 1000 / portTICK_RATE_MS) != pdTRUE )
//   // {
//   //   abort();
//   // }
// }

void send_continue_wait() {
  if(xSemaphoreTake(spi_empty, 1000 / portTICK_RATE_MS) != pdTRUE){
    abort();
  }
}

void send_continue_line(uint16_t *line, int width, int lineCount) {
  spi_transaction_t* t;
  t = spi_get_transaction();


  t->tx_data[0] = 0x3C;   //memory write continue
  t->length = 8;
  t->user = (void*)0;
  t->flags = SPI_TRANS_USE_TXDATA;

  spi_put_transaction(t);


  t = spi_get_transaction();

  t->length = width * 2 * lineCount * 8;
  t->tx_buffer = line;
  t->user = (void*)0x81;
  t->flags = 0;

  spi_put_transaction(t);
}

static void initialize_backlight() {
  // Note: In esp-idf v3.0, settings flash speed to 80Mhz causes the LCD controller
  // to malfunction after a soft-reset.

  // (duty range is 0 ~ ((2**bit_num)-1)


  //configure timer0
  ledc_timer_config_t ledc_timer;
  memset(&ledc_timer, 0, sizeof(ledc_timer));

  ledc_timer.bit_num = LEDC_TIMER_13_BIT; //set timer counter bit number
  ledc_timer.freq_hz = 5000;        //set frequency of pwm
  ledc_timer.speed_mode = LEDC_LOW_SPEED_MODE;   //timer mode,
  ledc_timer.timer_num = LEDC_TIMER_0;  //timer index


  ledc_timer_config(&ledc_timer);


  //set the configuration
  ledc_channel_config_t ledc_channel;
  memset(&ledc_channel, 0, sizeof(ledc_channel));

  //set LEDC channel 0
  ledc_channel.channel = LEDC_CHANNEL_0;
  //set the duty for initialization.(duty range is 0 ~ ((2**bit_num)-1)
  ledc_channel.duty = (LCD_BACKLIGHT_ON_VALUE) ? 0 : DUTY_MAX;
  //GPIO number
  ledc_channel.gpio_num = LCD_PIN_NUM_BCKL;
  //GPIO INTR TYPE, as an example, we enable fade_end interrupt here.
  ledc_channel.intr_type = LEDC_INTR_FADE_END;
  //set LEDC mode, from ledc_mode_t
  ledc_channel.speed_mode = LEDC_LOW_SPEED_MODE;
  //set LEDC timer source, if different channel use one timer,
  //the frequency and bit_num of these channels should be the same
  ledc_channel.timer_sel = LEDC_TIMER_0;


  ledc_channel_config(&ledc_channel);


  //initialize fade service.
  ledc_fade_func_install(0);

  // duty range is 0 ~ ((2**bit_num)-1)
  ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, (LCD_BACKLIGHT_ON_VALUE) ? DUTY_MAX : 0, 500);
  ledc_fade_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, LEDC_FADE_NO_WAIT);

  isBackLightIntialized = true;
}

void setBacklightValue(int value) {
  int duty = DUTY_MAX * (value * 0.01f);

  ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty, 500);
  ledc_fade_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, LEDC_FADE_NO_WAIT);
}


void Display2::Init() {
  // Init
  initialize_spi();

  // Malloc the buffers used to paint the display via SPI transactions
  //Todo: Unroll this (there are only TWO buffers!)
  const size_t bufferSize = DISPLAY_WIDTH * PARALLEL_LINES * sizeof(uint16_t);
  for (int x = 0; x < LINE_BUFFERS; x++) {
    printf("%s: malloc line[%i] with size %i bytes\n", __func__, x, bufferSize);;
    line[x] = (uint16_t *)heap_caps_malloc(bufferSize, MALLOC_CAP_DMA | MALLOC_CAP_8BIT);
    if (!line[x]) abort();

    line_buffer_put(line[x]);
  }

  // Initialize transactions
  for (int x = 0; x < SPI_TRANSACTION_COUNT; x++) {
    void* param = &trans[x];
    xQueueSend(spi_queue, &param, portMAX_DELAY);
  }


  // Initialize SPI
  esp_err_t ret;
  spi_bus_config_t buscfg;
  memset(&buscfg, 0, sizeof(buscfg));

  buscfg.miso_io_num = SPI_PIN_NUM_MISO;
  buscfg.mosi_io_num = SPI_PIN_NUM_MOSI;
  buscfg.sclk_io_num = SPI_PIN_NUM_CLK;
  buscfg.quadwp_io_num = -1;
  buscfg.quadhd_io_num = -1;

  spi_device_interface_config_t devcfg;
  memset(&devcfg, 0, sizeof(devcfg));

  devcfg.clock_speed_hz = LCD_SPI_CLOCK_RATE;
  devcfg.mode = 0;                //SPI mode 0
  devcfg.spics_io_num = LCD_PIN_NUM_CS;         //CS pin
  devcfg.queue_size = 7;              //We want to be able to queue 7 transactions at a time
  devcfg.pre_cb = spi_pre_transfer_callback;  //Specify pre-transfer callback to handle D/C line
  devcfg.flags = SPI_DEVICE_NO_DUMMY; //SPI_DEVICE_HALFDUPLEX;

  //Initialize the SPI bus
  ret = spi_bus_initialize(HSPI_HOST, &buscfg, 1);
  assert(ret==ESP_OK);

  //Attach the LCD to the SPI bus
  ret = spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
  assert(ret==ESP_OK);

  //Initialize the LCD
  printf("LCD: calling send_display_boot_program().\n");
  send_display_boot_program();

  printf("LCD: calling initialize_backlight.\n");
  initialize_backlight();

  printf("LCD Initialized (%d Hz).\n", LCD_SPI_CLOCK_RATE);
}





// Return use of backlight pin
void prepare() {
  esp_err_t err = rtc_gpio_deinit(LCD_PIN_NUM_BCKL);
  if (err != ESP_OK)  {
    abort();
  }
}


void Display2::WriteFrame(uint8_t* frameBuffer, uint16_t* palette) {
  uint16_t x;
  short y;

  lock_display();


  const uint16_t displayWidth = DISPLAY_WIDTH;
  send_reset_drawing(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);

  uint16_t step = (DISPLAY_WIDTH * sizeof(uint16_t) * PARALLEL_LINES) >> 1;
  uint32_t position = 0, 
           start = 0, 
           end = 0;

  uint8_t* bufferPointer = frameBuffer;

  // printf("Step = %i\n", step);
  // Print palette
  // for (int i = 0; i < 16; i++) {
  //   printf("palette values: palette[%i] = %i\n", i, palette[i]);
  // }

  // for (int i = 0; i < 256; i++) {
  //   printf("palette values: palette[%i] = %i\n", i, palette[i]);
  // }


  uint16_t color = color565(0,0,255);

  for (y = 0; y < DISPLAY_HEIGHT; y += PARALLEL_LINES) {
    int linesWritten = 0;
    uint16_t* line_buffer = line_buffer_get();

    // printf("sizoeof line_buffer = %i\n", sizeof(line_buffer));
    end += step;
    // printf("bufferPointer[%i] = %i\n", position, bufferPointer[position]);

    for (uint16_t i = 0; i < step; i++) {
      // uint8_t val = bufferPointer[0];
      // line_buffer[i] = val;
      // printf("\tposition = %i | val 0x%4X\n", position, val);
      // *spiBuff++ = dispBufferIn[position++] > 0 ? 65535 : 0;
      //00000.111011.00000
      line_buffer[i] = palette[bufferPointer[position++]];
      // line_buffer[i] = color;
      // line_buffer[i] = palette[bufferPointer[position++]];
      
    }

    start = end;

    // memset(line_buffer, esp_random() & 65535, DISPLAY_WIDTH * sizeof(uint16_t) * PARALLEL_LINES);

    // display
    send_continue_line(line_buffer, displayWidth, PARALLEL_LINES);
  }



  send_continue_wait();
  unlock_display();
}


void clear(uint16_t color) {
  //xTaskToNotify = xTaskGetCurrentTaskHandle();

  send_reset_drawing(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);

  // clear the buffer
  for (int i = 0; i < LINE_BUFFERS; ++i){
    for (int j = 0; j < DISPLAY_WIDTH * PARALLEL_LINES; ++j) {
      line[i][j] = color;
    }
  }

  // clear the screen
  send_reset_drawing(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);

  for (int y = 0; y < DISPLAY_HEIGHT; y += PARALLEL_LINES)
  {
    uint16_t* line_buffer = line_buffer_get();
    send_continue_line(line_buffer, DISPLAY_WIDTH, PARALLEL_LINES);
  }

  send_continue_wait();
}



int Display2::isBacklightInitialized() {
  return isBackLightIntialized;
}