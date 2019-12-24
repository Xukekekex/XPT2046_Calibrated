// -----------------------------------------------------------------------------
//
// the process of calibrating a touchscreen, at a high-level, consists of:
//
// (ONE-TIME)
//   1. pick three random points a,b,c (not co-linear) in -screen- coordinates.
//   2. for each point p, identify the corresponding -touchscreen- coorindate p_t.
// (ALWAYS/RUNTIME)
//   3. with all six points (a, a_t, b, b_t, c, c_t), along with the screen
//      height and width for the current orientation (per setRotation()), pass 
//      a calibration object to the touchscreen via calibrate().
//
// any subsequent calls to get touch position after calling calibrate() will 
// return (x,y)-points relative to the -screen- coordinate system.
//
// ** NOTE that steps 1-2 above only need to be performed one time beforehand
// for each physical touchscreen used. the six computed parameters are fixed
// descriptors of the hardware and should not change. so be sure to write them
// down once, and then re-use them wherever you use the touchscreen.
//
// --
//
// the sketch below provides a convenient means of translating screen coordinates
// to touch coordinates by drawing a crosshair on screen and printing the
// touch coordinate when touched. you may use the three random points a,b,c below
// or substitute them for your own.
//
// once the touch coordinates have been obtained, substitute their values into
// the points under the VERIFY_CALIBRATION macro below to see the calibrated 
// values on screen in place of the raw touch coordinates.
//
// be sure to use something pokey, fine-tipped to touch the crosshair center.
//
// not entirely sure why, but I've had best luck by performing steps 1-2 using
// landscape (rotation=1) with screen width,height=320,240. once calibrated,
// with proper translation points, then I'm free to rotate any way I choose.
//
// -----------------------------------------------------------------------------

#include <XPT2046_Calibrated.h>
#include <Adafruit_ILI9341.h> // or Adafruit_ILI9341.h
#include <SPI.h>

// --

#define TS_CS_PIN   4
#define TS_IRQ_PIN  3

#define TFT_CS_PIN  7
#define TFT_DC_PIN  5

static uint16_t const SCREEN_WIDTH    = 240;
static uint16_t const SCREEN_HEIGHT   = 320;
static uint8_t  const SCREEN_ROTATION = 2U;

// source points used for calibration
static TS_Point a( 13,  11);
static TS_Point b(312, 113);
static TS_Point c(167, 214);

#define VERIFY_CALIBRATION
#ifdef VERIFY_CALIBRATION
static TS_Point aT(3795,3735);
static TS_Point bT(482,2200);
static TS_Point cT(2084,583);
static TS_Calibration cal(a, aT, b, bT, c, cT, SCREEN_WIDTH, SCREEN_HEIGHT);
#endif


// --

#ifdef TIRQ_PIN
XPT2046_Calibrated ts(TS_CS_PIN, TS_IRQ_PIN);
#else
XPT2046_Calibrated ts(TS_CS_PIN);
#endif

Adafruit_ILI9341 tft(TFT_CS_PIN, TFT_DC_PIN);

// -----------------------------------------------------------------------------

void crosshair(TS_Point p) {
  tft.drawCircle   (p.x,     p.y,     6, ILI9341_WHITE);
  tft.drawFastHLine(p.x - 4, p.y,     9, ILI9341_WHITE);
  tft.drawFastVLine(p.x,     p.y - 4, 9, ILI9341_WHITE);
}

// --

void setup() {

  Serial.begin(115200);

  tft.begin();
  tft.setRotation(SCREEN_ROTATION);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_WHITE);

  ts.begin();
  ts.setRotation(SCREEN_ROTATION);
#ifdef VERIFY_CALIBRATION
  ts.calibrate(cal);
#endif

  while (!Serial && (millis() <= 1000U))
    { delay(5); }

  crosshair(a);
  crosshair(b);
  crosshair(c);
}

inline bool touched() {
#ifdef TIRQ_PIN
  if (ts.tirqTouched()) {
    return ts.touched();
  }
  return false;
#else
  return ts.touched();
#endif
}

void loop() {

  static uint16_t textHeight = (uint16_t)(1.5F * 8.0F + 0.5F);
  static uint16_t posTop  = SCREEN_HEIGHT - textHeight;
  static uint16_t posLeft = 6U;

  if (touched()) {

    tft.fillRect(
        posLeft, 
        posTop, 
        SCREEN_WIDTH - posLeft, 
        textHeight, 
        ILI9341_BLACK
    );

    TS_Point p = ts.getPoint();

    tft.setCursor(posLeft, posTop);
    tft.printf("(x,y) = (%u,%u)", p.x, p.y);

    delay(30);
  }
}

