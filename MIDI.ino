void serialEvent1() {
  //  Main MIDI input, (TX is VLT2 output)
  while (Serial1.available()) {
    midi1in = Serial1.read(); //read raw serial data bytes
    if (midi1in == 0xF8) {
      wasClock1=5;
      if ((clk1[0] & 0x01) == 1) {
        out1circle[oc1] = 0xF8; oc1++; //clk1[] is clock input on input 1 to [output]
      }
      if ((clk1[1] & 0x01)) {
        out2circle[oc2] = 0xF8; oc2++;
      }
      if ((clk1[2] & 0x01)) {
        out3circle[oc3] = 0xF8; oc3++;
      }
      analogWrite(IN_LED1, 20); inLED1 = 20;
      Serial.print("MIDI DATA 1");
    } else {
      //Serial1.write(midi1in);
      rxMIDI1();
    }
  }
}
void serialEvent2() {
  //  Main MIDI input, (TX is VLT2 output)
  while (Serial2.available()) {
    midi2in = Serial2.read(); //read raw serial data byte
    if (midi2in == 0xF8) {
      wasClock2=5;
      if ((clk2[0] & 0x01)) {
        out1circle[oc1] = 0xF8; oc1++;
      }
      if ((clk2[1] & 0x01)) {
        out2circle[oc2] = 0xF8; oc2++;
      }
      if ((clk2[2] & 0x01)) {
        out3circle[oc3] = 0xF8; oc3++;
      }
      analogWrite(IN_LED2, 20); inLED2 = 20;
//      Serial.print("MIDI DATA 2");
    } else {
      rxMIDI2();
    }
  }
}
void serialEvent3() {
  //  Main MIDI input, (Ttput)X is VLT2 ou
  while (Serial3.available()) {
    midi3in = Serial3.read(); //read raw serial data bytes
    if (midi3in == 0xF8) {
      wasClock3=5;
      if ((clk3[0] & 0x01)) {
        out1circle[oc1] = 0xF8; oc1++;
      }
      if ((clk3[1] & 0x01)) {
        out2circle[oc2] = 0xF8; oc2++;
      }
      if ((clk3[2] & 0x01)) {
        out3circle[oc3] = 0xF8; oc3++;
      }
      analogWrite(IN_LED3, 20); inLED3 = 20;
//      Serial.print("MIDI DATA 3");
    } else {
      rxMIDI3();
    }
  }
}


void rxMIDI1() {

  if (midi1in == 0xFE || midi1in == 0xF8) return;
  if (midi1in == 0xF7) {
    sysExCnt1 = 0; return; //not sure..can't really combine sysex's so maybe just send through on same port out??
  }
  if (midi1in == 0xF0 || sysExCnt1) {
    sysExCnt1 = 1;
    return;
  }
  if (p1 == 0 && !(midi1in & 0x80)) {
    p1++; //running status!! use last command again. ALways remove running status
    rx1Buf[0] = lastCom1;
    if ((lastCom1 & 0xF0) != 0xF0 && lastCom1 > 127) { //strip channel here for running status!
      midiChannel1 = lastCom1 & 0x0f;
      rx1Buf[0] &= 0xf0; //chop channel for now
    }

  }
  rx1Buf[p1] = midi1in;
  //

  //--MIDI SENTENCE LOADED----
  if (p1 >= 2) {
    p1 = 0;
   // Serial.println(rx1Buf[0], HEX);
    if (rx1Buf[0] == 0x90 && rx1Buf[2] == 0) {
      rx1Buf[0] = 0x80; rx1Buf[2] = 64;
    }
    if ((rx1Buf[0] == 0x90 || rx1Buf[0] == 0x80 || rx1Buf[0] == 0xE0 ) && midiChannel1 == split1) { // is on split channel??
      split1handler();
    } else {
      if ((in1out1[midiChannel1] < 16)) { // this chann is assigned?
        byte csw = rx1Buf[0] | (in1out1[midiChannel1] & 0x0F);
        if ((rx1Buf[0] == 0xC0 && !(clk1[0] & 0x04)) || (rx1Buf[0] == 0xB0 && !(clk1[0] & 0x08))) {
          // don't send PC
        } else {
          midiOut1(csw, rx1Buf[1], rx1Buf[2]);
        }
      }
      if ((in1out2[midiChannel1] < 16)) { // this chann is assigned?
        byte csw = rx1Buf[0] | (in1out2[midiChannel1] & 0x0F);
        if ((rx1Buf[0] == 0xC0 && !(clk1[1] & 0x04)) || (rx1Buf[0] == 0xB0 && !(clk1[1] & 0x08))) {
          // don't send PC
        } else {
          midiOut2(csw, rx1Buf[1], rx1Buf[2]);
        }
      }
      if ((in1out3[midiChannel1] < 16)) { // this chann is assigned?
        byte csw = rx1Buf[0] | (in1out3[midiChannel1] & 0x0F);
        if ((rx1Buf[0] == 0xC0 && !(clk1[2] & 0x04)) || (rx1Buf[0] == 0xB0 && !(clk1[2] & 0x08))) {
          // don't send PC/cc
        } else {
          midiOut3(csw, rx1Buf[1], rx1Buf[2]);
        }
      }
    }
    analogWrite(IN_LED1, 100); inLED1 = 100;
  }

  if (p1 == 1) p1++; //continue loading Command sentence
  if (p1 == 0) {
    if ((midi1in & 0xF0) != 0xF0 && midi1in > 127) { //strip channel
      lastCom1 = midi1in; //for running status
      midiChannel1 = midi1in & 0x0f;
      midi1in &= 0xf0; //chop channel for now
      rx1Buf[0] = midi1in;
    }

    switch (midi1in) {
      case 0x90: //m++;break;//noteOn (or off)
      case 0x80: //m++;break;//noteOff
      case 0xA0: //key pressure ignored
      case 0xE0: //pitch bend
      case 0xF2:
      case 0xB0: p1++; break; // CC ignored
      case 0xC0: // PC is 2 byte
      case 0xD0: p1 += 2; break; // also 2 byte
        break;

      case 0xFA:
      case 0xFB:
      case 0xFC:
        wasClock1=0;
        if ((clk1[0] & 0x02)) { // send transport messages??
          out1circle[oc1] = midi1in; oc1++;
        }
        if ((clk1[1] & 0x02)) {
          out2circle[oc2] = midi1in; oc2++;
        }
        if ((clk1[2] & 0x02)) {
          out3circle[oc3] = midi1in; oc3++;
        }
        splitPC1 = 0; //reset so split event sends PC's again
        break;
    }
  }
}



void rxMIDI2() {

  if (midi2in == 0xFE || midi2in == 0xF8) return;
  if (midi2in == 0xF7) {
    sysExCnt2 = 0; return; //not sure..can't really combine sysex's so maybe just send through on same port out??
  }
  if (midi2in == 0xF0 || sysExCnt2) {
    sysExCnt2 = 1;
    return;
  }
  if (p2 == 0 && !(midi2in & 0x80)) {
    p2++; //running status!! use last command again. ALways remove running status
    rx2Buf[0] = lastCom2;
    if ((lastCom2 & 0xF0) != 0xF0 && lastCom2 > 127) { //strip channel here for running status!
      midiChannel2 = lastCom2 & 0x0f;
      rx2Buf[0] &= 0xf0; //chop channel for now
    }
  }
  rx2Buf[p2] = midi2in;
  //

  //--MIDI SENTENCE LOADED----
  if (p2 >= 2) {
    p2 = 0;
    if (rx2Buf[0] == 0x90 && rx2Buf[2] == 0) {
      rx2Buf[0] = 0x80; rx2Buf[2] = 64;
    }
    if ((rx2Buf[0] == 0x90 || rx2Buf[0] == 0x80 || rx2Buf[0] == 0xE0 ) && midiChannel2 == split2) { // is on split channel??
      split2handler();
    } else {
      if ((in2out1[midiChannel2] < 16)) { // this chann is assigned?
        byte csw = rx2Buf[0] | (in2out1[midiChannel2] & 0x0F);
        if ((rx2Buf[0] == 0xC0 && !(clk2[0] & 0x04)) || (rx2Buf[0] == 0xB0 && !(clk2[0] & 0x08))) {
          // don't send PC or CC
        } else {
          midiOut1(csw, rx2Buf[1], rx2Buf[2]);
        }
      }
      if ((in2out2[midiChannel2] < 16)) { // this chann is assigned?
        byte csw = rx2Buf[0] | (in2out2[midiChannel2] & 0x0F);
        if ((rx2Buf[0] == 0xC0 && !(clk2[1] & 0x04)) || (rx2Buf[0] == 0xB0 && !(clk2[1] & 0x08))) {
          // don't send PC or CC
        } else {
          midiOut2(csw, rx2Buf[1], rx2Buf[2]);
        }
      }
      if ((in2out3[midiChannel2] < 16)) { // this chann is assigned?
        byte csw = rx2Buf[0] | (in2out3[midiChannel2] & 0x0F);
        if ((rx2Buf[0] == 0xC0 && !(clk2[2] & 0x04)) || (rx2Buf[0] == 0xB0 && !(clk2[2] & 0x08))) {
          // don't send PC or CC
        } else {
          midiOut3(csw, rx2Buf[1], rx2Buf[2]);
        }
      }
    }
    analogWrite(IN_LED2, 100); inLED2 = 100;
  }

  if (p2 == 1) p2++; //continue loading Command sentence
  if (p2 == 0) {
    if ((midi2in & 0xF0) != 0xF0 && midi2in > 127) { //strip channel
      lastCom2 = midi2in; //for running status
      midiChannel2 = midi2in & 0x0f;
      midi2in &= 0xf0; //chop channel for now
      rx2Buf[0] = midi2in;
    }

    switch (midi2in) {
      case 0x90: //m++;break;//noteOn (or off)
      case 0x80: //m++;break;//noteOff
      case 0xA0: //key pressure ignored
      case 0xE0:
      case 0xF2:
      case 0xB0: p2++; break; // CC ignored
      case 0xC0: // PC is 2 byte
      case 0xD0: p2 += 2; break; // also 2 byte
        break;
      case 0xFA:
      case 0xFB:
      case 0xFC:
      wasClock2=0;
        if ((clk2[0] & 0x02)) { // send transport messages??
          out1circle[oc1] = midi2in; oc1++;
        }
        if ((clk2[1] & 0x02)) {
          out2circle[oc2] = midi2in; oc2++;
        }
        if ((clk2[2] & 0x02)) {
          out3circle[oc3] = midi2in; oc3++;
        }
        splitPC2 = 0; //reset so split event sends PC's again
        break;
    }
  }
}




void rxMIDI3() {

  if (midi3in == 0xFE || midi3in == 0xF8) return;
  if (midi3in == 0xF7) {
    sysExCnt3 = 0; return; //not sure..can't really combine sysex's so maybe just send through on same port out??
  }
  if (midi3in == 0xF0 || sysExCnt3) {
    sysExCnt3 = 1;
    return;
  }
  if (p3 == 0 && !(midi3in & 0x80)) {
    p3++; //running status!! use last command again. ALways remove running status
    rx3Buf[0] = lastCom3;
    if ((lastCom3 & 0xF0) != 0xF0 && lastCom3 > 127) { //strip channel here for running status!
      midiChannel3 = lastCom3 & 0x0f;
      rx3Buf[0] &= 0xf0; //chop channel for now
    }
  }
  rx3Buf[p3] = midi3in;
  //

  //--MIDI SENTENCE LOADED----
  if (p3 >= 2) {
    p3 = 0;
    if (rx3Buf[0] == 0x90 && rx3Buf[2] == 0) {
      rx3Buf[0] = 0x80; rx3Buf[2] = 64;
    }
    if ((rx3Buf[0] == 0x90 || rx3Buf[0] == 0x80 || rx3Buf[0] == 0xE0 ) && midiChannel3 == split3) { // is on split channel??
      split3handler();
    } else {
      if ((in3out1[midiChannel3] < 16)) { // this chann is assigned?
        byte csw = rx3Buf[0] | (in3out1[midiChannel3] & 0x0F);
        if ((rx3Buf[0] == 0xC0 && !(clk3[0] & 0x04)) || (rx3Buf[0] == 0xB0 && !(clk3[0] & 0x08))) {
          // don't send PC or CC
        } else {
          midiOut1(csw, rx3Buf[1], rx3Buf[2]);
        }
      }
      if ((in3out2[midiChannel3] < 16)) { // this chann is assigned?
        byte csw = rx3Buf[0] | (in3out2[midiChannel3] & 0x0F);
        if ((rx3Buf[0] == 0xC0 && !(clk3[1] & 0x04)) || (rx3Buf[0] == 0xB0 && !(clk3[1] & 0x08))) {
          // don't send PC or CC
        } else {
          midiOut2(csw, rx3Buf[1], rx3Buf[2]);
        }
      }
      if ((in3out3[midiChannel3] < 16)) { // this chann is assigned?
        byte csw = rx3Buf[0] | (in3out3[midiChannel3] & 0x0F);
        if ((rx3Buf[0] == 0xC0 && !(clk3[2] & 0x04)) || (rx3Buf[0] == 0xB0 && !(clk3[2] & 0x08))) {
          // don't send PC or CC
        } else {
          midiOut3(csw, rx3Buf[1], rx3Buf[2]);
        }
      }
    }
    analogWrite(IN_LED3, 100); inLED3 = 100;
  }

  if (p3 == 1) p3++; //continue loading Command sentence
  if (p3 == 0) {
    if ((midi3in & 0xF0) != 0xF0 && midi3in > 127) { //strip channel
      lastCom3 = midi3in; //for running status
      midiChannel3 = midi3in & 0x0f;
      midi3in &= 0xf0; //chop channel for now
      rx3Buf[0] = midi3in;
    }

    switch (midi3in) {
      case 0x90: //m++;break;//noteOn (or off)
      case 0x80: //m++;break;//noteOff
      case 0xA0: //key pressure ignored
      case 0xE0:
      case 0xF2:
      case 0xB0: p3++; break; // CC ignored
      case 0xC0: // PC is 2 byte
      case 0xD0: p3 += 2; break; // also 2 byte
        break;
      case 0xFA:
      case 0xFB:
      case 0xFC:
      wasClock3=0;
        if ((clk3[0] & 0x02)) { // send transport messages??
          out1circle[oc1] = midi3in; oc1++;
        }
        if ((clk3[1] & 0x02)) {
          out2circle[oc2] = midi3in; oc2++;
        }
        if ((clk3[2] & 0x02)) {
          out3circle[oc3] = midi3in; oc3++;
        }
        splitPC3 = 0; //reset so split event sends PC's again
        break;
    }
  }
}









// ------------MIDI OUTS ---------------

void midiOut1(byte c, byte m, byte v) { // must handle 2 byte messages!
  if (m > 127)return; //stop PC=128 and maybe otehrs...
  if ((c & 0xF0) == 0x90) {
    for (byte i = 0; i < 255; i++) {
      if ((no1Note[i] & 0x80)) { // find opening
     //   Serial.print("non:");Serial.println(m);
        no1Chan[i] = c & 0x0F; no1Note[i] = m; break;
      }
    }
  }
  if ((c & 0xF0) == 0x80) no1remove(c, m);
  out1circle[oc1] = c; oc1++;
  if ((c & 0xF0) != 0xC0 && (c & 0xF0) != 0xD0) {
    out1circle[oc1] = m; oc1++;
  }
  out1circle[oc1] = v; oc1++;
  if ((c & 0xf0) != 0x80) { //don't show noteoffs
    analogWrite(OUT_LED1, v); outLED1 = v;
    // Serial.println(c,HEX);
  }
}
void midiOut2(byte c, byte m, byte v) { // must handle 2 byte messages!
  if (m > 127)return;
  if ((c & 0xF0) == 0x90) {
    for (byte i = 0; i < 255; i++) {
      if ((no2Note[i] & 0x80)) {
        no2Chan[i] = c & 0x0F; no2Note[i] = m; break;
      }
    }
  }
  if ((c & 0xF0) == 0x80) no2remove(c, m);
  out2circle[oc2] = c; oc2++;
  if ((c & 0xF0) != 0xC0 && (c & 0xF0) != 0xD0) {
    out2circle[oc2] = m; oc2++;
  }
  out2circle[oc2] = v; oc2++;
  if ((c & 0xf0) != 0x80) { //don't show noteoffs
    analogWrite(OUT_LED2, v); outLED2 = v;
  }
}
void midiOut3(byte c, byte m, byte v) { // must handle 2 byte messages!
  if (m > 127)return;
  if ((c & 0xF0) == 0x90) {
    for (byte i = 0; i < 255; i++) {
      if ((no3Note[i] & 0x80)) {
        no3Chan[i] = c & 0x0F; no3Note[i] = m; break;
      }
    }
  }
  if ((c & 0xF0) == 0x80) no3remove(c, m);
  out3circle[oc3] = c; oc3++;
  if ((c & 0xF0) != 0xC0 && (c & 0xF0) != 0xD0) {
    out3circle[oc3] = m; oc3++;
  }
  out3circle[oc3] = v; oc3++;
  if ((c & 0xf0) != 0x80) { //don't show noteoffs
    analogWrite(OUT_LED3, v); outLED3 = v;
  }
}

void midiOutF8in1() { //send F8's to which outs? **** but which in's also!!
  if (clk1[0]) {
    out1circle[oc1] = 0xF8; oc1++;
  }
  if (clk2[0]) {
    out2circle[oc2] = 0xF8; oc2++;
  }
  if (clk3[0]) {
    out3circle[oc3] = 0xF8; oc3++;
  }
}

void split1handler() {
  if (rx1Buf[0] != 0xE0) {
    splitPC1send();
    if (rx1Buf[1] < split1low + overLap[0]) {
      byte note = rx1Buf[1] + split1NO1; byte csw = rx1Buf[0] | (split1chan1 & 0x0f);
      while (note > 127)note += 12;
      if ((split1chan1 & 0x30) == 0x10) midiOut1(csw, note, rx1Buf[2]); //to which outs..
      if ((split1chan1 & 0x30) == 0x20) midiOut2(csw, note, rx1Buf[2]);
      if ((split1chan1 & 0x30) == 0x30) midiOut3(csw, note, rx1Buf[2]);
    }
    if (rx1Buf[1] >= split1low && rx1Buf[1] < split1hi + overLap[0]) {
      byte note = rx1Buf[1] + split1NO2 - split1low; byte csw = rx1Buf[0] | (split1chan2 & 0x0f);
      while (note > 127)note += 12;
      if ((split1chan2 & 0x30) == 0x10) midiOut1(csw, note, rx1Buf[2]);
      if ((split1chan2 & 0x30) == 0x20) midiOut2(csw, note, rx1Buf[2]);
      if ((split1chan2 & 0x30) == 0x30) midiOut3(csw, note, rx1Buf[2]);
    }
    if (rx1Buf[1] >= split1hi) {
      byte note = rx1Buf[1] + split1NO3 - split1hi; byte csw = rx1Buf[0] | (split1chan3 & 0x0f);
      while (note > 127)note += 12;
      if ((split1chan3 & 0x30) == 0x10) midiOut1(csw, note, rx1Buf[2]);
      if ((split1chan3 & 0x30) == 0x20) midiOut2(csw, note, rx1Buf[2]);
      if ((split1chan3 & 0x30) == 0x30) midiOut3(csw, note, rx1Buf[2]);
    }
  } else { // pitch wheel!! must send to all channels in split
    splitPW1send();
  }
}


void split2handler() {
  if (rx2Buf[0] != 0xE0) {
    splitPC2send();
    if (rx2Buf[1] < split2low + overLap[1]) {
      byte note = rx2Buf[1] + split2NO1; byte csw = rx2Buf[0] | (split2chan1 & 0x0f);
      while (note > 127)note += 12;
      if ((split2chan1 & 0x30) == 0x10) midiOut1(csw, note, rx2Buf[2]); //to which outs..
      if ((split2chan1 & 0x30) == 0x20) midiOut2(csw, note, rx2Buf[2]);
      if ((split2chan1 & 0x30) == 0x30) midiOut3(csw, note, rx2Buf[2]);
    }
    if (rx2Buf[1] >= split2low && rx2Buf[1] < split2hi + overLap[1]) {
      byte note = rx2Buf[1] + split2NO2 - split2low; byte csw = rx2Buf[0] | (split2chan2 & 0x0f);
      while (note > 127)note += 12;
      if ((split2chan2 & 0x30) == 0x10) midiOut1(csw, note, rx2Buf[2]);
      if ((split2chan2 & 0x30) == 0x20) midiOut2(csw, note, rx2Buf[2]);
      if ((split2chan2 & 0x30) == 0x30) midiOut3(csw, note, rx2Buf[2]);
    }
    if (rx2Buf[1] >= split2hi) {
      byte note = rx2Buf[1] + split2NO3 - split2hi; byte csw = rx2Buf[0] | (split2chan3 & 0x0f);
      while (note > 127)note += 12;
      if ((split2chan3 & 0x30) == 0x10) midiOut1(csw, note, rx2Buf[2]);
      if ((split2chan3 & 0x30) == 0x20) midiOut2(csw, note, rx2Buf[2]);
      if ((split2chan3 & 0x30) == 0x30)midiOut3(csw, note, rx2Buf[2]);
    }
  } else { // pitch wheel!! must send to all channels in split
    splitPW2send();
  }
}
void split3handler() {
  if (rx3Buf[0] != 0xE0) {
    splitPC3send();
    if (rx3Buf[1] < split3low + overLap[2]) {
      byte note = rx3Buf[1] + split3NO1; byte csw = rx3Buf[0] | (split3chan1 & 0x0f);
      while (note > 127)note += 12;
      if ((split3chan1 & 0x30) == 0x10) midiOut1(csw, note, rx3Buf[2]); //to which outs..
      if ((split3chan1 & 0x30) == 0x20) midiOut2(csw, note, rx3Buf[2]);
      if ((split3chan1 & 0x30) == 0x30) midiOut3(csw, note, rx3Buf[2]);
    }
    if (rx3Buf[1] >= split3low && rx3Buf[1] < split3hi + overLap[2]) {
      byte note = rx3Buf[1] + split3NO2 - split3low; byte csw = rx3Buf[0] | (split3chan2 & 0x0f);
      while (note > 127)note += 12;
      if ((split3chan2 & 0x30) == 0x10) midiOut1(csw, note, rx3Buf[2]);
      if ((split3chan2 & 0x30) == 0x20) midiOut2(csw, note, rx3Buf[2]);
      if ((split3chan2 & 0x30) == 0x30) midiOut3(csw, note, rx3Buf[2]);
    }
    if (rx3Buf[1] >= split3hi) {
      byte note = rx3Buf[1] + split3NO3 - split3hi; byte csw = rx3Buf[0] | (split3chan3 & 0x0f);
      while (note > 127)note += 12;
      if ((split3chan3 & 0x30) == 0x10) midiOut1(csw, note, rx3Buf[2]);
      if ((split3chan3 & 0x30) == 0x20) midiOut2(csw, note, rx3Buf[2]);
      if ((split3chan3 & 0x30) == 0x30)midiOut3(csw, note, rx3Buf[2]);
    }
  } else { // pitch wheel!! must send to all channels in split
    splitPW3send();
  }
}


void splitPC1send() {
  if (splitPC1 == 0) { //send Prog change on each active port for channels used in split
    byte csw = 0xC0 | (split1chan1 & 0x0f); if ((split1chan1 & 0x30) != 0) {
      switch ((split1chan1 & 0x30)) {
        case 0x10: midiOut1(csw, 0, split1PC1 ); break;
        case 0x20: midiOut2(csw, 0, split1PC1 ); break;
        case 0x30: midiOut3(csw, 0, split1PC1 ); break;
      }
    }
    csw = 0xC0 | (split1chan2 & 0x0f); if ((split1chan2 & 0x30) != 0) {
      switch ((split1chan2 & 0x30)) {
        case 0x10: midiOut1(csw, 0, split1PC2 ); break;
        case 0x20: midiOut2(csw, 0, split1PC2 ); break;
        case 0x30: midiOut3(csw, 0, split1PC2 ); break;
      }
    }
    csw = 0xC0 | (split1chan3 & 0x0f); if ((split1chan3 & 0x30) != 0) {
      switch ((split1chan3 & 0x30)) {
        case 0x10: midiOut1(csw, 0, split1PC3 ); break;
        case 0x20: midiOut2(csw, 0, split1PC3 ); break;
        case 0x30: midiOut3(csw, 0, split1PC3 ); break;
      }
    }
    splitPC1 = 1; //could also be re-armed from play/cont/stop commands?
  }
}
void splitPC2send() {
  if (splitPC2 == 0) { //send Prog change on each active port for channels used in split
    byte csw = 0xC0 | (split2chan1 & 0x0f); if ((split2chan1 & 0x30) != 0) {
      switch ((split2chan1 & 0x30)) {
        case 0x10: midiOut1(csw, 0, split2PC1 ); break;
        case 0x20: midiOut2(csw, 0, split2PC1 ); break;
        case 0x30: midiOut3(csw, 0, split2PC1 ); break;
      }
    }
    csw = 0xC0 | (split2chan2 & 0x0f); if ((split2chan2 & 0x30) != 0) {
      switch ((split2chan2 & 0x30)) {
        case 0x10: midiOut1(csw, 0, split2PC2 ); break;
        case 0x20: midiOut2(csw, 0, split2PC2 ); break;
        case 0x30: midiOut3(csw, 0, split2PC2 ); break;
      }
    }
    csw = 0xC0 | (split2chan3 & 0x0f); if ((split2chan3 & 0x30) != 0) {
      switch ((split2chan3 & 0x30)) {
        case 0x10: midiOut1(csw, 0, split2PC3 ); break;
        case 0x20: midiOut2(csw, 0, split2PC3 ); break;
        case 0x30: midiOut3(csw, 0, split2PC3 ); break;
      }
    }
    splitPC2 = 1; //could also be re-armed from play/cont/stop commands?
  }
}
void splitPC3send() {
  if (splitPC3 == 0) { //send Prog change on each active port for channels used in split
    byte csw = 0xC0 | (split3chan1 & 0x0f); if ((split3chan1 & 0x30) != 0) {
      switch ((split3chan1 & 0x30)) {
        case 0x10: midiOut1(csw, 0, split3PC1 ); break;
        case 0x20: midiOut2(csw, 0, split3PC1 ); break;
        case 0x30: midiOut3(csw, 0, split3PC1 ); break;
      }
    }
    csw = 0xC0 | (split3chan2 & 0x0f); if ((split3chan2 & 0x30) != 0) {
      switch ((split3chan2 & 0x30)) {
        case 0x10: midiOut1(csw, 0, split3PC2 ); break;
        case 0x20: midiOut2(csw, 0, split3PC2 ); break;
        case 0x30: midiOut3(csw, 0, split3PC2 ); break;
      }
    }
    csw = 0xC0 | (split3chan3 & 0x0f); if ((split3chan3 & 0x30) != 0) {
      switch ((split3chan3 & 0x30)) {
        case 0x10: midiOut1(csw, 0, split3PC3 ); break;
        case 0x20: midiOut2(csw, 0, split3PC3 ); break;
        case 0x30: midiOut3(csw, 0, split3PC3 ); break;
      }
    }
    splitPC3 = 1; //could also be re-armed from play/cont/stop commands?
  }
}
void splitPW1send() {
  byte csw = 0xE0 | (split1chan1 & 0x0f); if ((split1chan1 & 0x30) != 0) {
    switch ((split1chan1 & 0x30)) {
      case 0x10: midiOut1(csw, rx1Buf[1], rx1Buf[2]); break;
      case 0x20: midiOut2(csw, rx1Buf[1], rx1Buf[2]); break;
      case 0x30: midiOut3(csw, rx1Buf[1], rx1Buf[2]); break;
    }
  }
  csw = 0xE0 | (split1chan2 & 0x0f); if ((split1chan2 & 0x30) != 0) {
    switch ((split1chan2 & 0x30)) {
      case 0x10: midiOut1(csw, rx1Buf[1], rx1Buf[2]); break;
      case 0x20: midiOut2(csw, rx1Buf[1], rx1Buf[2]); break;
      case 0x30: midiOut3(csw, rx1Buf[1], rx1Buf[2]); break;
    }
  }
  csw = 0xE0 | (split1chan3 & 0x0f); if ((split1chan3 & 0x30) != 0) {
    switch ((split1chan3 & 0x30)) {
      case 0x10: midiOut1(csw, rx1Buf[1], rx1Buf[2] ); break;
      case 0x20: midiOut2(csw, rx1Buf[1], rx1Buf[2]); break;
      case 0x30: midiOut3(csw, rx1Buf[1], rx1Buf[2]); break;
    }
  }
}

void splitPW2send() {
  byte csw = 0xE0 | (split2chan1 & 0x0f); if ((split2chan1 & 0x30) != 0) {
    switch ((split2chan1 & 0x30)) {
      case 0x10: midiOut1(csw, rx2Buf[1], rx2Buf[2]); break;
      case 0x20: midiOut2(csw, rx2Buf[1], rx2Buf[2]); break;
      case 0x30: midiOut3(csw, rx2Buf[1], rx2Buf[2]); break;
    }
  }
  csw = 0xE0 | (split2chan2 & 0x0f); if ((split2chan2 & 0x30) != 0) {
    switch ((split2chan2 & 0x30)) {
      case 0x10: midiOut1(csw, rx2Buf[1], rx2Buf[2]); break;
      case 0x20: midiOut2(csw, rx2Buf[1], rx2Buf[2]); break;
      case 0x30: midiOut3(csw, rx2Buf[1], rx2Buf[2]); break;
    }
  }
  csw = 0xE0 | (split2chan3 & 0x0f); if ((split2chan3 & 0x30) != 0) {
    switch ((split2chan3 & 0x30)) {
      case 0x10: midiOut1(csw, rx2Buf[1], rx2Buf[2] ); break;
      case 0x20: midiOut2(csw, rx2Buf[1], rx2Buf[2]); break;
      case 0x30: midiOut3(csw, rx2Buf[1], rx2Buf[2]); break;
    }
  }
}

void splitPW3send() {
  byte csw = 0xE0 | (split3chan1 & 0x0f); if ((split3chan1 & 0x30) != 0) {
    switch ((split3chan1 & 0x30)) {
      case 0x10: midiOut1(csw, rx3Buf[1], rx3Buf[2]); break;
      case 0x20: midiOut2(csw, rx3Buf[1], rx3Buf[2]); break;
      case 0x30: midiOut3(csw, rx3Buf[1], rx3Buf[2]); break;
    }
  }
  csw = 0xE0 | (split3chan2 & 0x0f); if ((split3chan2 & 0x30) != 0) {
    switch ((split3chan2 & 0x30)) {
      case 0x10: midiOut1(csw, rx3Buf[1], rx3Buf[2]); break;
      case 0x20: midiOut2(csw, rx3Buf[1], rx3Buf[2]); break;
      case 0x30: midiOut3(csw, rx3Buf[1], rx3Buf[2]); break;
    }
  }
  csw = 0xE0 | (split3chan3 & 0x0f); if ((split3chan3 & 0x30) != 0) {
    switch ((split3chan3 & 0x30)) {
      case 0x10: midiOut1(csw, rx3Buf[1], rx3Buf[2] ); break;
      case 0x20: midiOut2(csw, rx3Buf[1], rx3Buf[2]); break;
      case 0x30: midiOut3(csw, rx3Buf[1], rx3Buf[2]); break;
    }
  }
}

void no1remove(byte c, byte n) {
  c &= 0x0F;
  for (byte i = 0; i < 255; i++) {
    if (!(no1Note[i] & 0x80)) {
      if (no1Note[i] == n && no1Chan[i] == c) {
        no1Note[i] |= 0x80; //preserve in case need in future
        break;
      }
    }
  }
}

void no2remove(byte c, byte n) {
  c &= 0x0F;
  for (byte i = 0; i < 255; i++) {
    if (!(no2Note[i] & 0x80)) {
      if (no2Note[i] == n && no2Chan[i] == c) {
        no2Note[i] |= 0x80; //preserve in case need in future
        break;
      }
    }
  }
}

void no3remove(byte c, byte n) {
  c &= 0x0F;
  for (byte i = 0; i < 255; i++) {
    if (!(no3Note[i] & 0x80)) {
      if (no3Note[i] == n && no3Chan[i] == c) {
        no3Note[i] |= 0x80; //preserve in case need in future
        break;
      }
    }
  }
}

void allNotesOff(byte c, byte o) { //called when config is changed. c is channel, if 16 then is all chans. o is output0-2, if 4 then all
  // most now use all of these at once in menus 
//Serial.print("ANO--");Serial.print(c);Serial.print(" o:");Serial.println(o);
  for (byte i = 0; i < 255; i++) {
    if (o == 0 || o == 4) {
      if (!(no1Note[i] & 0x80)) {
        if (no1Chan[i] == c || c == 16) {
         // Serial.print("1noff:");Serial.print(no1Chan[i] | 0x80,HEX);Serial.print(" # ");Serial.println(no1Note[i]);
          out1circle[oc1] = no1Chan[i] | 0x80; oc1++; out1circle[oc1] = no1Note[i]; oc1++; out1circle[oc1] = 0x40; oc1++;
          no1Note[i] |= 0x80; //preserve in case need in future
        }
      }
    }
    if (o == 1 || o == 4) {
      if (!(no2Note[i] & 0x80)) {
        if (no2Chan[i] == c || c == 16) {
         // Serial.print("2noff:");Serial.print(no2Chan[i] | 0x80,HEX);Serial.print(" # ");Serial.println(no2Note[i]);
          out2circle[oc2] = (no2Chan[i] | 0x80); oc2++; out2circle[oc2] = no2Note[i]; oc2++; out2circle[oc2] = 0x40; oc2++;
          no2Note[i] |= 0x80; //preserve in case need in future
        }
      }
    }
    if (o == 2 || o == 4) {
      if (!(no3Note[i] & 0x80)) {
      // Serial.print("3noff:");Serial.print(no3Chan[i] | 0x80,HEX);Serial.print(" # ");Serial.println(no3Note[i]);
        if (no3Chan[i] == c || c == 16) {
          out3circle[oc3] = no3Chan[i] | 0x80; oc3++; out3circle[oc3] = no3Note[i]; oc3++; out3circle[oc3] = 0x40; oc3++;
          no3Note[i] |= 0x80; //preserve in case need in future
        }
      }
    }
  }
}
