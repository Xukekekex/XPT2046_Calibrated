# XPT2046_Calibrated Arduino touchscreen library

This library is a fork of the excellent XPT2046_Touchscreen library by Paul Stoffregen.

The only addition to this library is support for 3-point calibration as described in the paper by Fang & Chang (Texas Instruments):

http://www.ti.com/lit/an/slyt277/slyt277.pdf

See [examples/TouchCalibration/TouchCalibration.ino](https://github.com/ardnew/XPT2046_Calibrated/blob/master/examples/TouchCalibration/TouchCalibration.ino) for usage example.

The header from that example sketch follows.

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

