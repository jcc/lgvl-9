#include <Arduino.h>
#include <lvgl.h>
#include "demos/lv_demos.h"
#define LGFX_USE_V1 // Define before #include <LovyanGFX.hpp>
#include <LovyanGFX.hpp>
#include "pin_config.h"
#include "lvgl_setup.h"

#define LCD_WIDTH 240
#define LCD_HEIGHT 280

/***************************************************************************************************
 * LGFX Setup
 ***************************************************************************************************/

class LGFX : public lgfx::LGFX_Device
{
    lgfx::Panel_ST7789      _panel_instance;
    lgfx::Bus_SPI           _bus_instance;
    lgfx::Light_PWM         _light_instance;
    lgfx::Touch_CST816S     _touch_instance;

public:
    LGFX(void)
    {
        {
          auto cfg = _bus_instance.config();
          cfg.spi_host = HSPI_HOST;
          cfg.spi_mode = 2;
          cfg.freq_write = 40000000;
          cfg.freq_read  = 16000000;
          cfg.spi_3wire  = true;
          cfg.use_lock   = true;
          cfg.dma_channel = SPI_DMA_CH_AUTO;
          cfg.pin_sclk = LCD_SCK;
          cfg.pin_mosi = LCD_MOSI;
          cfg.pin_miso = LCD_MISO;
          cfg.pin_dc   = LCD_DC;
          _bus_instance.config(cfg);
          _panel_instance.setBus(&_bus_instance);
        }

        {                           
            auto cfg = _panel_instance.config();    // 表示パネル設定用の構造体を取得します。

            cfg.pin_cs           =    LCD_CS;
            cfg.pin_rst          =    LCD_RST;
            cfg.pin_busy         =    -1;
            cfg.panel_width      =   LCD_WIDTH;
            cfg.panel_height     =   LCD_HEIGHT;
            cfg.offset_x         =     0;
            cfg.offset_y         =     0;
            cfg.offset_rotation  =     0;
            cfg.dummy_read_pixel =     8;
            cfg.dummy_read_bits  =     16;
            cfg.readable         =  true;
            cfg.invert           = true;
            cfg.rgb_order        = true;
            cfg.dlen_16bit       = false;
            cfg.bus_shared       =  true;

            _panel_instance.config(cfg);
        }

        {                                        // バックライト制御の設定を行います。（必要なければ削除）
            auto cfg = _light_instance.config(); // バックライト設定用の構造体を取得します。

            cfg.pin_bl = LCD_BL; // バックライトが接続されているピン番号
            cfg.invert = false;  // バックライトの輝度を反転させる場合 true
            cfg.freq = 44100;    // バックライトのPWM周波数
            cfg.pwm_channel = 7; // 使用するPWMのチャンネル番号

            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance); // バックライトをパネルにセットします。
        }

        { // タッチスクリーン制御の設定を行います。（必要なければ削除）
            auto cfg = _touch_instance.config();
            cfg.x_min = 0;              // タッチスクリーンから得られる最小のX値(生の値)
            cfg.x_max = LCD_WIDTH - 1;  // タッチスクリーンから得られる最大のX値(生の値)
            cfg.y_min = 0;              // タッチスクリーンから得られる最小のY値(生の値)
            cfg.y_max = LCD_HEIGHT - 1; // タッチスクリーンから得られる最大のY値(生の値)
            cfg.pin_int = LCD_INT_T;           // INTが接続されているピン番号
            cfg.bus_shared = true;      // 画面と共通のバスを使用している場合 trueを設定
            cfg.offset_rotation = 0;    // 表示とタッチの向きのが一致しない場合の調整 0~7の値で設定

            // SPI接続の場合
            cfg.spi_host = HSPI_HOST;// 使用するSPIを選択 (HSPI_HOST or VSPI_HOST)
            cfg.freq = 1000000;     // SPIクロックを設定
            cfg.pin_sclk = LCD_SCK;     // SCLKが接続されているピン番号
            cfg.pin_mosi = LCD_MOSI;     // MOSIが接続されているピン番号
            cfg.pin_miso = LCD_MISO;     // MISOが接続されているピン番号
            cfg.pin_cs   = LCD_CS;     //   CSが接続されているピン番号

            // I2C接続の場合
            cfg.i2c_port = 1;      // 使用するI2Cを選択 (0 or 1)
            cfg.i2c_addr = 0x15;   // I2Cデバイスアドレス番号
            cfg.pin_sda  = LCD_SDA_T;     // SDAが接続されているピン番号
            cfg.pin_scl  = LCD_SCL_T;     // SCLが接続されているピン番号
            cfg.freq = 400000;     // I2Cクロックを設定

            _touch_instance.config(cfg);
            _panel_instance.setTouch(&_touch_instance); // タッチスクリーンをパネルにセットします。
        }
        setPanel(&_panel_instance); // 使用するパネルをセットします。
    }
};

LGFX tft;

/***************************************************************************************************
 * LVGL Setup
 ***************************************************************************************************/

/* Display flushing */
static void display_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.writePixels((lgfx::rgb565_t *)px_map, w * h);
    tft.endWrite();

    lv_display_flush_ready(disp);
}

/*Read the touchpad*/
static void touchpad_read(lv_indev_t *indev_driver, lv_indev_data_t *data)
{
    uint16_t touchX, touchY;
    bool touched = tft.getTouch(&touchX, &touchY);
    if (!touched)
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
    else
    {
        /*Set the coordinates*/
        data->point.x = touchX;
        data->point.y = touchY;
        data->state = LV_INDEV_STATE_PRESSED;
        Serial.println("X = " + String(touchX) + " Y = " + String(touchY));
    }
}

void lvgl_init()
{
    /*Setup LCD*/
    tft.begin();
    tft.setBrightness(128);

    /*Calibrate touch screen*/
    // uint16_t calData[] = {0, 0, 0, 280, 240, 0, 240, 280};
    // tft.setTouchCalibrate(calData);

    lv_init();

    /*Initialize the display and buffer*/
    lv_display_t * disp = lv_display_create(LCD_WIDTH, LCD_HEIGHT);
    static uint16_t buf[LCD_WIDTH * LCD_HEIGHT / 10];
    lv_display_set_flush_cb(disp, display_flush);
    lv_display_set_buffers(disp, buf, NULL, sizeof(buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

    /*Initialize the touch device driver*/
    lv_indev_t * indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, touchpad_read);

    // lv_demo_benchmark();
    // lv_demo_scroll();

    Serial.printf("Lvgl v%d.%d.%d initialized\n", lv_version_major(), lv_version_minor(), lv_version_patch());
}

void lvgl_handler()
{
    const uint16_t period = 5;
    static uint32_t last_tick = 0;
    uint32_t tick = millis();
    if (tick - last_tick >= period)
    {
        last_tick = tick;
        lv_tick_inc(period);
        lv_task_handler();
    }
}