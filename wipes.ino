
int loop_speed = 1;  //multiplyer for the pause loop funcion, its loop_speed*led_step
int multiplyer = 1;

int loop_time = 100;  // time a pause takes, this is what loop_speed counts up to
int loop_step = 0;

int curPixel = 0;  // current active pixel, gets iterated with led_step
int maxPixel = 24;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FULLON - all LEDs only, dimmable

void fullon() {
  dmx_master.setChannelValue(1, dim);
  dmx_master.setChannelValue(7, dim);
  dmx_master.setChannelValue(13, dim);
  dmx_master.setChannelValue(19, dim);
  dmx_master.setChannelValue(25, dim);
  dmx_master.setChannelValue(31, dim);
  dmx_master.setChannelValue(37, dim);
  dmx_master.setChannelValue(43, dim);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PULSE - all LEDs on the same time fade up fade down
void pulse() {

  // dmx_master.setChannelValue(1, 0);
  // dmx_master.setChannelValue(7, 0);
  // dmx_master.setChannelValue(13, 0);
  // dmx_master.setChannelValue(19, 0);
  // dmx_master.setChannelValue(25, 0);
  // dmx_master.setChannelValue(31, 0);
  // dmx_master.setChannelValue(37, 0);
  // dmx_master.setChannelValue(43, 0);

  if (loop_step < loop_time) {
    loop_step += step * 10;
  } else {
    loop_step = 0;

    brightness = brightness + dim;

    if (brightness <= 1 || brightness >= 150) {
      dim = -dim;
    }


    dmx_master.setChannelValue(1, brightness);
    dmx_master.setChannelValue(7, brightness);
    dmx_master.setChannelValue(13, brightness);
    dmx_master.setChannelValue(19, brightness);
    dmx_master.setChannelValue(25, brightness);
    dmx_master.setChannelValue(31, brightness);
    dmx_master.setChannelValue(37, brightness);
    dmx_master.setChannelValue(43, brightness);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RANDOMIZER - random on/off ledstrips
void randomizer() {
}


void strobe() {

  if (loop_step < loop_time) {
    loop_step += step * 10;

    dmx_master.setChannelValue(1, dim);
    dmx_master.setChannelValue(7, dim);
    dmx_master.setChannelValue(13, dim);
    dmx_master.setChannelValue(19, dim);
    dmx_master.setChannelValue(25, dim);
    dmx_master.setChannelValue(31, dim);
    dmx_master.setChannelValue(37, dim);
    dmx_master.setChannelValue(43, dim);
  }

  if (loop_step < loop_time) {
    loop_step += step * loop_speed;

    dimmAll();
  }
}

void lauflicht() {


    dimmAll();

  if (loop_step < loop_time) {
    loop_step += step * 10;
  } else {
    for (brightness = 0; brightness <= dim; brightness++) {
      dmx_master.setChannelValue(1, brightness);
      dmx_master.setChannelValue(43, brightness);
    }
    loop_step = 0;
  }

  if (loop_step < loop_time) {
    loop_step += step * 10;
  } else {
    for (brightness = 0; brightness <= dim; brightness++) {
      dmx_master.setChannelValue(7, brightness);
      dmx_master.setChannelValue(37, brightness);
    }
    loop_step = 0;
  }

  if (loop_step < loop_time) {
    loop_step += step * 10;
  } else {
    for (brightness = 0; brightness <= dim; brightness++) {
      dmx_master.setChannelValue(13, brightness);
      dmx_master.setChannelValue(31, brightness);
    }
    loop_step = 0;
  }

  if (loop_step < loop_time) {
    loop_step += step * 10;
  } else {
    for (brightness = 0; brightness <= dim; brightness++) {
      dmx_master.setChannelValue(19, brightness);
      dmx_master.setChannelValue(25, brightness);
    }
    loop_step = 0;
  }
}


void dimmAll() {

    fadeAmount = dim;
    brightness = brightness - fadeAmount;

    // reverse the direction of the fading at the ends of the fade:
    if (brightness <= 0) {
      fadeAmount = 0;
    }

    dmx_master.setChannelValue(1, brightness);

    dmx_master.setChannelValue(7, brightness);

    dmx_master.setChannelValue(13, brightness);

    dmx_master.setChannelValue(19, brightness);

    dmx_master.setChannelValue(25, brightness);

    dmx_master.setChannelValue(31, brightness);

    dmx_master.setChannelValue(37, brightness);

    dmx_master.setChannelValue(43, brightness);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// chaseCol - toggle left and right col, fade

void chaseCol() {
    uint8_t g_slow = 5;
    static uint8_t step;
    static unsigned long last;
    unsigned long now = micros();
    unsigned long diff = now - last;
    last = now;
    if(diff > 200*g_slow) {
      step++;
      uint8_t leftInt = 0;
      uint8_t rightInt = 0;
      if(step < 64) {
        leftInt = step * 4;
      } else if(step < 128) {
        leftInt = 255 - ((step-64) * 4);
      } else if(step < 192) {
        rightInt = (step - 128) * 4;
      } else {
        rightInt = 256 - ((step - 192) * 4);
      }
      setRoofCol(0, leftInt, leftInt>>2, 0);
      setRoofCol(1, rightInt, rightInt>>2, 0);
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// chaseCol - toggle left and right col, fade

void chaseRow() {
    //for (uint8_t i=72; i< 128;i+=6) {
    //  dmx_master.setChannelValue(i, 255);
    //  dmx_master.setChannelValue(i+1, 255);
    //}return;

    static uint8_t step;
    static uint8_t row;
    static uint8_t brig;

    bool next = true;//diff > (g_slow * 10);

    switch (step)
    {
    case 0:
      if(brig>200) {
        step++;
        brig = 0;
        return;
      }
      if(next)
      {
        brig+=g_fast;
        setRoofRow(row, brig, 0, 0);
      }
      break;
    case 1:
      if(next)
        brig+=g_fast;//delay only, dont set value
      if(brig>=200) {
        step++;
      }
      break;
    case 2:
      if(brig<24) {
        step = 0;
        setRoofRow(row, 0, 0, 0);
        if (row==4)
          row=0;
        else
          row++;
        return;
      }
      if(next)
      {
       brig-=g_fast;;
        setRoofRow(row, brig, 0, 0);
      } 
      break;
    default:
      break;
    }

}


