#include <Conceptinetics.h>

#define DMX_MASTER_CHANNELS   512
#define RXEN_PIN                2
DMX_Master        dmx_master ( DMX_MASTER_CHANNELS, RXEN_PIN );

int cr = 100;
int cg=0;
int cb=0;
int ledbrig = 50;


void setup() {
//Serial.begin(9600);

  dmx_master.enable ();
  dmx_master.setChannelRange ( 1, 512, 0);
}


void doit() {
    dmx_master.setChannelValue ( 1, ledbrig*cr/400);
    dmx_master.setChannelValue ( 2, ledbrig*cg/400);
    dmx_master.setChannelValue ( 3, ledbrig*cb/400);
}
void setBrig(int brig) {
  ledbrig = brig;
  doit();
}
void setCol(int pr, int pg, int pb) {
  cr = pr;
  cg = pg;
  cb = pb;
  doit();
}
void strobe(int son, int soff) {
    setBrig(90);
    delay(son);
    setBrig(0);
    delay(soff);
}


void loop() {

  while(1) {
    int a = 80;
    while ((a--) > 50) {
      setCol(100, 10, 0);
      setBrig(60);

      delay(1000);
    }
    int tbrig = 80;
    int curr = 0;
    int curg = 0;
    int curb = 99;
    while(a-- > 45) {
      for(int brignow = 90; brignow>0; brignow=brignow - 3) {
        setCol(curr, curg, curb);
        setBrig(brignow);
        delay(30);
      }
      curr += 19;
      curb -= 19;
    }
    setCol(100, 0, 0);
    setBrig(40);
    while(a-- > 20) {
      setBrig(30+(a-20)*2);
      delay(1000);
    }
    setBrig(0);
    delay(1000);
    strobe(160, 200);
    strobe(80, 200);
    strobe(80, 1000);
    while ((a--) > 30) {
      setCol(90, 40, 0);
      
      strobe(80, 200);
      strobe(80, 200);
    }
    while ((a--) > 3) {
      setCol(90, 40, 0);
      
      strobe(150, 243);
      strobe(150, 243);
    }
    while ((a--) > 0) {
      strobe(75, 121);
      strobe(75, 121);
      strobe(75, 121);
      strobe(75, 121);
    }
  }
}

