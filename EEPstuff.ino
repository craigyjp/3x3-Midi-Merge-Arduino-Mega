
void factory() { //reset all EEPROM to factory "USER MIDI CFG nn"
  // ** note: This must be called from setup while variables are loaded with declaration values
  //  for (int i = 0; i < 512; i++) { //make bigger
  //    EEPROM.write(i, 0xFF);
  //  }
  OLEDwerds("FACTORY", 10, 0, 0);
  for (int k = 0; k < 10; k++) { //160 total bytes
    name = "USER MIDI CFG "; if (k < 10)name += "0";
    name += String(k + 1);
    if (k == 0) name = "INITIAL CONFIG 1";
    for (int i = 0; i < 16; i++) {
      EEPROM.write(i + (k * 16), name.charAt(i));
      //Serial.println(name.charAt(i));
    }
  }
  //EEPROM.write(256 + i + (k * 16), i); // 160 to
  for ( int k = 0; k < 16; k++) {
    in1out1[k] = k ; //in1out1[inChan]-->out1, bit 4=off
    in1out2[k] = k ;
    in1out3[k] = k ;
    in2out1[k] = k ;
    in2out2[k] = k ;
    in2out3[k] = k ;
    in3out1[k] = k ;
    in3out2[k] = k ;
    in3out3[k] = k ;
  }
  //quick write these values to all 10 user configs 0-15 90 times from160+
  byte c = 0;
  for (int i = 0; i < 1440; i++) {
    EEPROM.write(i + 160, c);
    c++; if (c == 16) c = 0;
  }//next is 1600


  //load defaults set in variable declaraion
  for (int k = 0; k < 10; k++) { //splits: 18 * 10user=180 bytes
    int kk = k * 18;
    EEPROM.write(1600 + kk, split1); EEPROM.write(1601 + kk, split1low); EEPROM.write(1602 + kk, split1hi); EEPROM.write(1603 + kk, split1chan1); EEPROM.write(1604 + kk, split1chan2); EEPROM.write(1605 + kk, split1chan3);
    EEPROM.write(1606 + kk, split1); EEPROM.write(1607 + kk, split1low); EEPROM.write(1608 + kk, split1hi); EEPROM.write(1609 + kk, split1chan1); EEPROM.write(1610 + kk, split1chan2); EEPROM.write(1611 + kk, split1chan3);
    EEPROM.write(1612 + kk, split1); EEPROM.write(1613 + kk, split1low); EEPROM.write(1614 + kk, split1hi); EEPROM.write(1615 + kk, split1chan1); EEPROM.write(1616 + kk, split1chan2); EEPROM.write(1617 + kk, split1chan3);
    // next is 1780
  }


  for (int i = 0; i < 3; i++) {
    clk1[i] = 0x0e; //out[] from inX? bit0=clk,bit1=trnspt,bit2=PC,bit3=CC's
    clk2[i] = 0x0e; //clock is off by default!
    clk3[i] = 0x0e;
  }

  for (int i = 0; i < 90; i++) { //0 to 8 (9 bytes per user cfg)*10=90 -->1870
    int ii = 1780 + i;
    EEPROM.write(ii, 0x0e);
  } //1870 next
  for (int i = 0; i < 90; i++) { //Program Changes to be sent for each channel/output
    EEPROM.write(1870 + i, 0); //all default to piano1
  }
  for (int i = 0; i < 81; i++) { //90-9 (last config is fixed at 36's) entries for Split Note Offsets...default=36 for all
    EEPROM.write(1960 + i, 36);
  }
  //Next is 2041 for any odd settings

  EEPROM.write(2047, user);
  userSel();
}

void userSel() { //load present user configuration
  //char Str[17];
  int i = 0;
  int k = user * 16; //(0 - 144 {+15})
  for (i = 0; i < 16; i++) {
    Str[i] = EEPROM.read(i + k);
  }
  Str[i] = 0;
  name = Str;
  //Serial.println(name);
  k = user;
  // load all port channels for this config from 160. 0-15 * 9 *user
  for (int i = 0; i < 16; i++) {
    int kk = (k * 144) + i; //user0:160-303, user1:304-447 user9:1456-1599
    in1out1[i] = EEPROM.read(160 + kk);
    in1out2[i] = EEPROM.read(176 + kk);
    in1out3[i] = EEPROM.read(192 + kk);
    in2out1[i] = EEPROM.read(208 + kk);
    in2out2[i] = EEPROM.read(224 + kk);
    in2out3[i] = EEPROM.read(240 + kk);
    in3out1[i] = EEPROM.read(256 + kk);
    in3out2[i] = EEPROM.read(272 + kk);
    in3out3[i] = EEPROM.read(288 + kk);
  }
  //split variables, 18 per user config!
  int kk = user * 18;
  split1 = EEPROM.read(1600 + kk); split1low = EEPROM.read(1601 + kk); split1hi = EEPROM.read(1602 + kk); split1chan1 = EEPROM.read(1603 + kk); split1chan2 = EEPROM.read(1604 + kk); split1chan3 = EEPROM.read(1605 + kk);
  split2 = EEPROM.read(1606 + kk); split2low = EEPROM.read(1607 + kk); split2hi = EEPROM.read(1608 + kk); split2chan1 = EEPROM.read(1609 + kk); split2chan2 = EEPROM.read(1610 + kk); split2chan3 = EEPROM.read(1611 + kk);
  split3 = EEPROM.read(1612 + kk); split3low = EEPROM.read(1613 + kk); split3hi = EEPROM.read(1614 + kk); split3chan1 = EEPROM.read(1615 + kk); split3chan2 = EEPROM.read(1616 + kk); split3chan3 = EEPROM.read(1617 + kk);
  overLap[0] = split1 >> 5; split1 &= 0x1F; overLap[1] = split2 >> 5; split2 &= 0x1F; overLap[2] = split3 >> 5; split3 &= 0x1F;
  overLap[0] = overLap[0] << 1; overLap[1] = overLap[1] << 1; overLap[2] = overLap[2] << 1; //double them so 0-14 notes
  //1780 is next
  for (int i = 0; i < 3; i++) {
    int ii = (user * 9) + i;
    clk1[i] = EEPROM.read(ii + 1780); //out[] from inX? bit0=clk,bit1=trnspt,bit2=PC,bit3=CC's
    clk2[i] = EEPROM.read(ii + 1783);
    clk3[i] = EEPROM.read(ii + 1786);
  }//1870 is next
  k = user * 9;
  split1PC1 = EEPROM.read(1870 + k); //program changes for splits
  split1PC2 = EEPROM.read(1871 + k);
  split1PC3 = EEPROM.read(1872 + k);
  split2PC1 = EEPROM.read(1873 + k);
  split2PC2 = EEPROM.read(1874 + k);
  split2PC3 = EEPROM.read(1875 + k);
  split3PC1 = EEPROM.read(1876 + k);
  split3PC2 = EEPROM.read(1877 + k);
  split3PC3 = EEPROM.read(1878 + k);
  // next is 1960
  if (user < 9) {
    split1NO1 = EEPROM.read(1960 + k);
    split1NO2 = EEPROM.read(1961 + k);
    split1NO3 = EEPROM.read(1962 + k);
    split2NO1 = EEPROM.read(1963 + k);
    split2NO2 = EEPROM.read(1964 + k);
    split2NO3 = EEPROM.read(1965 + k);
    split3NO1 = EEPROM.read(1966 + k);
    split3NO2 = EEPROM.read(1967 + k);
    split3NO3 = EEPROM.read(1968 + k);
  } else {
    split1NO1 = 36;
    split1NO2 = 36;
    split1NO3 = 36;
    split2NO1 = 36;
    split2NO2 = 36;
    split2NO3 = 36;
    split3NO1 = 36;
    split3NO2 = 36;
    split3NO3 = 36;
  }
  splitPC1 = 0; splitPC2 = 0; splitPC3 = 0; // send program change at first split event
  userWrite = 5; //EEPROM.write(2047, user); //update for next power-up <<do after a 5 second delay!!
}



void saveConfig() {
  int k = user;
  for (int i = 0; i < 16; i++) {
    EEPROM.write(i + (k * 16), name.charAt(i));
  }
  for (int i = 0; i < 16; i++) {
    int kk = (k * 144) + i; //user0:160-303, user1:304-447 user9:1456-1599
    EEPROM.write(160 + kk, in1out1[i]);
    EEPROM.write(176 + kk, in1out2[i]);
    EEPROM.write(192 + kk, in1out3[i]);
    EEPROM.write(208 + kk, in2out1[i]);
    EEPROM.write(224 + kk, in2out2[i]);
    EEPROM.write(240 + kk, in2out3[i]);
    EEPROM.write(256 + kk, in3out1[i]);
    EEPROM.write(272 + kk, in3out2[i]);
    EEPROM.write(288 + kk, in3out3[i]);
  }
  int kk = k * 18; byte o = 0; //to save 2x note overlap (0-14, lowest bit always 0)
  o = overLap[0] << 4; o = split1 | o; EEPROM.write(1600 + kk, o); EEPROM.write(1601 + kk, split1low); EEPROM.write(1602 + kk, split1hi); EEPROM.write(1603 + kk, split1chan1); EEPROM.write(1604 + kk, split1chan2); EEPROM.write(1605 + kk, split1chan3);
  o = overLap[1] << 4; o = split2 | o; EEPROM.write(1606 + kk, o); EEPROM.write(1607 + kk, split2low); EEPROM.write(1608 + kk, split2hi); EEPROM.write(1609 + kk, split2chan1); EEPROM.write(1610 + kk, split2chan2); EEPROM.write(1611 + kk, split2chan3);
  o = overLap[2] << 4; o = split3 | o; EEPROM.write(1612 + kk, o); EEPROM.write(1613 + kk, split3low); EEPROM.write(1614 + kk, split3hi); EEPROM.write(1615 + kk, split3chan1); EEPROM.write(1616 + kk, split3chan2); EEPROM.write(1617 + kk, split3chan3);
  //1780 is next

  for (int i = 0; i < 3; i++) {
    int ii = (k * 9) + i; //user0:
    EEPROM.write(ii + 1780, clk1[i]); //out[] from inX? bit0=clk,bit1=trnspt,bit2=PC,bit3=CC's
    EEPROM.write(ii + 1783, clk2[i]);
    EEPROM.write(ii + 1786, clk3[i]);
  }//1870 is next

  k = user * 9;
  EEPROM.write(1870 + k, split1PC1); //program changes for splits
  EEPROM.write(1871 + k, split1PC2);
  EEPROM.write(1872 + k, split1PC3);
  EEPROM.write(1873 + k, split2PC1);
  EEPROM.write(1874 + k, split2PC2);
  EEPROM.write(1875 + k, split2PC3);
  EEPROM.write(1876 + k, split3PC1);
  EEPROM.write(1877 + k, split3PC2);
  EEPROM.write(1878 + k, split3PC3);
  //1960 is next
  if (user < 9) { //can'r save last config note offsets, uses default
    EEPROM.write(1960 + k, split1NO1);
    EEPROM.write(1961 + k, split1NO2);
    EEPROM.write(1962 + k, split1NO3);
    EEPROM.write(1963 + k, split2NO1);
    EEPROM.write(1964 + k, split2NO2);
    EEPROM.write(1965 + k, split2NO3);
    EEPROM.write(1966 + k, split3NO1);
    EEPROM.write(1967 + k, split3NO2);
    EEPROM.write(1968 + k, split3NO3);
  }

  userWrite = 5;
  OLEDwerds(F("SAVED "), 0, 0, 0);

}



//
