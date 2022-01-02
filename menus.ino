/*
  Main Menu (Button Press)
  SELECT: Will assume USER:nn change and highlight nn
  UP or DOWN: Selects action menu (big letters) as:
  USER:nn  (if up first)
  NAME:   Enter Name
  M.OUT:1 select on this will open "OUT:1 FROM IN:1", "OUT:1 FROM IN:2", "OUT:1 FROM IN:3" ->> CHANNEL LIST 1-16 on/offs
  M.OUT:2
  M.OUT:3
  SPLIT:1 Select--> Shows CHAN:nn LO:nn  HI:nn X   (Shows 2 split points for notes on selected channel)
  SPLIT:2   ...     Use Up/Dn buttons to hi-light parameters the Select to use U/D buttons to change
  SPLIT:3   ...     Select X to exit. CHAN:OFF,1-16  on this input
  LEARN   Select--> every new channel message will open a menu of SEND TO OUTPUT 1:YES/NO ... and FINISHED!



*/



void upButton() {
  retToStby = 60;
  if (upHeld == 0 || upHeld > 80) {
    if (upHeld > 80) upHeld = 60;
    //Serial.println("UP");
    if (menuLev == 11) {
      bDir = 1;
      if (!splitEd) {
        splitSel--;  if (splitSel < 0)splitSel = 0;
        if (splitSel < splitFid) {
          splitFid--; splitList(); //update
        }
        OLED_UD(16, 0, 3, 4, 7); arrow(splitSel - splitFid);
      } else {
        splitEd = 0x80; splitAdj();
      }
    }
    if (menuLev == 10) {
      if (!letEdit) {
        letSel++; if (letSel > 15)letSel = 0;
      } else {
        Str[letSel]++; if (Str[letSel] > 127)Str[letSel] = 32;
      }
      nameIt();
    }
    if (menuLev == 9) {
      ccSel--; if (ccSel == 255) ccSel = 2;
      OLED_UD(4, 0, 3, ccSel + 1, ccSel + 1); arrow(ccSel + 1);
    }
    if (menuLev == 8) {
      tranSel--; if (tranSel == 255) tranSel = 2;
      OLED_UD(4, 0, 3, tranSel + 1, tranSel + 1); arrow(tranSel + 1);
    }
    if (menuLev == 7) {
      pcSel--; if (pcSel == 255) pcSel = 2;
      OLED_UD(4, 0, 3, pcSel + 1, pcSel + 1); arrow(pcSel + 1);
    }
    if (menuLev == 6) {
      clkSel--; if (clkSel == 255) clkSel = 2;
      OLED_UD(4, 0, 3, clkSel + 1, clkSel + 1); arrow(clkSel + 1);
    }
    if (menuLev == 5) {
      optSel--; if (optSel == 255)optSel = 4;
      menuOpt();
    }
    if (menuLev == 4) {
      chanAdj++; if (chanAdj > 16) chanAdj = 0;
      outListSel1();
    }
    if (menuLev == 3) {
      outListPtr--; if (outListPtr > 254)outListPtr = 0;
      if (outListPtr < outListFid) {
        outListFid--;
        outList(); //re-load
      }
      arrow(outListPtr - outListFid);
    }
    if (menuLev == 2) {
      lev2Ptr--; if (lev2Ptr > 254)lev2Ptr = 5;
      menuLev2();
    }
    if (menuLev == 1) {
      lev1Ptr--; if (lev1Ptr > 254)lev1Ptr = 5;
      menuLev1();
    }
    if (menuLev == 0) {
      menuLev = 1;
      menuLev1();
    }
  }
  upHeld++;
}
void dnButton() {
  retToStby = 60;
  if (dnHeld == 0 || dnHeld > 80) {
    if (dnHeld > 80) dnHeld = 60;
    //Serial.println("DN");
    if (menuLev == 11) {
      bDir = -1;
      if (!splitEd) {
        splitSel++;  if (splitSel > 15)splitSel = 15;
        if (splitSel > splitFid + 3) {
          splitFid++; splitList(); //update
        }
        OLED_UD(16, 0, 3, 4, 7); arrow(splitSel - splitFid);
      } else {
        splitEd = 0x80; splitAdj();
      }
    }
    if (menuLev == 10) {
      if (!letEdit) {
        letSel--; if (letSel == 255)letSel = 15;
      } else {
        Str[letSel]--; if (Str[letSel] < 32) Str[letSel] = 127;
      }
      nameIt();
    }
    if (menuLev == 9) {
      ccSel++; if (ccSel > 2) ccSel = 0;
      OLED_UD(4, 0, 3, ccSel + 1, ccSel + 1); arrow(ccSel + 1); //<<UD no need!wrong page, test remove
    }
    if (menuLev == 8) {
      tranSel++; if (tranSel > 2) tranSel = 0;
      OLED_UD(4, 0, 3, tranSel + 1, tranSel + 1); arrow(tranSel + 1);
    }
    if (menuLev == 7) {
      pcSel++; if (pcSel > 2) pcSel = 0;
      OLED_UD(4, 0, 3, pcSel + 1, pcSel + 1); arrow(pcSel + 1);
    }
    if (menuLev == 6) {
      clkSel++; if (clkSel > 2) clkSel = 0;
      OLED_UD(4, 0, 3, clkSel + 1, clkSel + 1); arrow(clkSel + 1);
    }
    if (menuLev == 5) {
      optSel++; if (optSel > 4)optSel = 0;
      menuOpt();
    }
    if (menuLev == 4) {
      chanAdj--; if (chanAdj == 255) chanAdj = 16;
      outListSel1();
    }
    if (menuLev == 3) {
      outListPtr++; if (outListPtr > 16)outListPtr = 16;//16 is "disable"
      if (outListPtr > 3 + outListFid) {
        outListFid++; outList(); //re-load
      }
      arrow(outListPtr - outListFid);
    }
    if (menuLev == 2) {
      lev2Ptr++; if (lev2Ptr > 5)lev2Ptr = 0;
      menuLev2();
    }
    if (menuLev == 1) {
      lev1Ptr++; if (lev1Ptr > 5)lev1Ptr = 0;
      menuLev1();
    }
    if (menuLev == 0) {
      menuLev = 1;
      menuLev1();
    }

  }
  dnHeld++;
}
void selButton() {
  retToStby = 60;
  if (selHeld == 0) {
    //Serial.print("SEL:");
    // Serial.println(menuLev);
    if (menuLev == 0) {
      user++; if (user > 9) user = 0; //temp here, must use up down after selection
      OLED_UD(512, 0, 127, 4, 7); //full screen
      userSel(); nameScroll = 0; stbyStep = 99; nameStep = 0; timeShow = 2; standBy();
      allNotesOff(16, 4);
    }
    else if (menuLev == 1) {
      menuLev1Sel();
    }
    else if (menuLev == 2) {
      menuLev2Sel(); //if (lev2Ptr < 3)arrow(outListPtr - outListFid);
    }
    else if (menuLev == 3) { //select outlist member
      outListSel1(); if (outListPtr != 16)menuLev = 4;
    }
    else if (menuLev == 4) {
      inOutPut(outListPtr); outList(); menuLev = 3; arrow(outListPtr - outListFid);
    }
    else if (menuLev == 5) {
      optList();
    }
    else  if (menuLev == 6) {
      clockSetSel();
    }
    else  if (menuLev == 7) {
      pcSetSel();
    }
    else  if (menuLev == 8) {
      tranSetSel();
    }
    else  if (menuLev == 9) {
      ccSetSel();
    }
    else if (menuLev == 10) {
      if (!letEdit) {
        letEdit = true;
      } else {
        letEdit = false;
      }
      nameIt();
    }
    else  if (menuLev == 11) { //SPLIT
      bDir = 0;
      if (!splitEd) {
        splitEd = 0x80; splitAdj();
      } else {
        splitEd = 0; splitAdj();
      }
    }
  }
  selHeld++;
  if (selHeld == 100 && holdExit) {
    holdExit = false;
    if (menuLev == 3 || menuLev == 4) {
      if (outListPtr == 16) togAll();  // toggle off flags on all!
      menuLev = 2; menuLev2();
    }
    if (menuLev == 6) { //exited but will have toggled last clock Y/N so redo
      clockSetSel(); menuOpt(); menuLev = 5;
    }
    if (menuLev == 7) { //exited but will have toggled last pc Y/N so redo
      pcSetSel(); menuOpt(); menuLev = 5;
    }
    if (menuLev == 8) { //exited but will have toggled last tran Y/N so redo
      tranSetSel(); menuOpt(); menuLev = 5;
    }
    if (menuLev == 9) { //exited but will have toggled last CC Y/N so redo
      ccSetSel(); menuOpt(); menuLev = 5;
    }
    if (menuLev == 10) { //exited name
      letEdit = false; menuLev = 1; menuLev1(); lev2Ptr = 0;
    }
    if (menuLev == 11) { //exited split
      menuLev = 2; splitEd = 0; menuLev2();
    }
  }
}

void UPrelease() {
  //Serial.println(upHeld);
  upHeld = 0; butMillisNext += 50;
}
void DNrelease() {
  // Serial.println(dnHeld);
  dnHeld = 0; butMillisNext += 50;
}
void SELrelease() {
  //Serial.println(selHeld);
  if (menuLev > 2 && menuLev != 5) holdExit = true; //(this is to stop hold during entry to a list exiting it again!)
  if (selHeld >= 100) {

  } else {
    if (onRelease == 1) clockSet(); //update display after release!
    if (onRelease == 2) pcSet();
    if (onRelease == 3) tranSet();
    if (onRelease == 4) ccSet();
  }
  onRelease = 0;
  selHeld = 0; butMillisNext += 50;
}







void menuLev1() { //for up down
  OLED_UD(512, 0, 127, 4, 7);
  String  t = F("SAVE:"); t += String(user + 1);
  switch (lev1Ptr) {
    case 0: OLEDwerds(t, 0, 0, 0); break; //SAVE
    case 1: OLEDwerds(F("M_IN:1"), 0, 0, 0); break;
    case 2: OLEDwerds(F("M_IN:2"), 0, 0, 0); break;
    case 3: OLEDwerds(F("M_IN:3"), 0, 0, 0); break;
    case 4: OLEDwerds(F("NAME:"), 0, 0, 0); OLEDwerds(String(user + 1), 80, 0, 0); break;
    case 5: OLEDwerds(F("USER:"), 0, 0, 0); OLEDwerds(String(user + 1), 80, 0, 0); break;
  }
}

void menuLev1Sel() { //select
  switch (lev1Ptr) {


    case 1: //M_IN:1
    case 2: //M_IN:2
    case 3: //M_IN:3
      menuLev = 2; menuLev2();
      break;
    case 4: //NAME
      menuLev = 10; OLED_UD(512, 0, 127, 4, 7); nameIt();
      break;
    case 0:
      OLEDwerds(F("SAVING"), 0, 0, 0); saveConfig(); allNotesOff(16, 4); // //do a save!
    case 5:
      menuLev = 0; nameScroll = 0;  nameStep = 0; stbyStep = 0; //USER (stby)
      break;
  }
}

void menuLev2() {
  OLED_UD(512, 0, 127, 4, 7);
  OLEDwerdsSm("I", 0, 0, 1); OLEDwerdsSm("N", 0, 1, 1); OLEDwerdsSm(String(lev1Ptr), 0, 3, 1);
  String  t = "SPLIT"; t += String(lev1Ptr); //1,2 or 3
  switch (lev2Ptr) {
    case 0: OLEDwerds(F("M_OUT:1"), 8, 0, 0); break;
    case 1: OLEDwerds(F("M_OUT:2"), 8, 0, 0); break;
    case 2: OLEDwerds(F("M_OUT:3"), 8, 0, 0); break;
    case 3: OLEDwerds(t, 8, 0, 0); break; // SPLIT
    case 4: OLEDwerds(F("OPTION"), 8, 0, 0); break;
    case 5: OLEDwerds(F(":EXIT:"), 16, 0, 0); break;
  }
}

void menuLev2Sel() {
  switch (lev2Ptr) {
    case 0: //M_OUT1
    case 1: //M_OUT2
    case 2: //M_OUT3
      outList(); menuLev = 3; arrow(outListPtr - outListFid);
      break;
    case 3: //SPLITn
      splitList(); menuLev = 11;
      break;
    case 4: //OPTIONs
      menuLev = 5; menuOpt();
      break;
    case 5: // exit
      menuLev = 1; menuLev1(); lev2Ptr = 0; //so doesn't show "EXIT" on entry! could have last edited?
      break;
  }
}

void nameIt() { //menuLev=10. up/down to select a letter
  name = Str;
  OLED_UD(256, 0, 127, 5, 6); OLEDwerdsSm(F("CFG:"), 30, 0, 16); OLEDwerdsSm(String(user + 1), 54, 0 , 16); OLEDwerdsSm(name, 0, 1, 16);
  if (!letEdit) OLEDwerdsSm("^", 6 * letSel, 2, 1);
  if (letEdit) {
    String ff = name.substring(letSel, letSel + 1); OLEDwerdsSm(ff, 6 * letSel, 1 + 0x80, 1); OLEDwerdsSm(ff, 6 * letSel, 2, 1);
  }
}


void outList() { //show list at last fid ... not for edit!
  OLED_UD(512, 0, 127, 4, 7);
  for (byte i = 0; i < 4; i++) {
    if (i + outListFid == 16 && i == 3) {
      OLEDwerdsSm(F("DISABLE?"), 6, i, 8);
    } else {
      byte dat = i + outListFid;
      OLEDwerdsSm(F("IN"), 6, i, 2); OLEDwerdsSm(String(lev1Ptr), 18, i, 1); OLEDwerdsSm(" CH:", 24, i, 4); //need arrow char
      if (dat + 1 < 10) {
        OLEDwerdsSm("0", 48, i, 4); OLEDwerdsSm(String(dat + 1), 54, i, 2);
      } else {
        OLEDwerdsSm(String(dat + 1), 48, i, 2);
      }
      if ((split1 < 16 && lev1Ptr == 1 && dat == split1) || (split2 < 16 && lev1Ptr == 2 && dat == split2) || (split3 < 16 && lev1Ptr == 3 && dat == split3)) {
        OLEDwerdsSm(F("-SPLIT-"), 66, i, 0);
      } else {
        OLEDwerdsSm(F(">OUT"), 60, i, 4); OLEDwerdsSm(String(lev2Ptr + 1), 84, i, 1);
        OLEDwerdsSm("CH:", 96, i, 3); inOutGet(dat);
        if (!(chanAdj & 0x10))OLEDwerdsSm(String((chanAdj & 0x0f) + 1), 114, i, 3); //is on?
        if ((chanAdj & 0x10))OLEDwerdsSm("--", 114, i, 3);
      }
    }
  }
}
void outListSel1() {
  byte i = outListPtr - outListFid; i += 0x80;
  if (menuLev == 3) {
    if (outListPtr == 16) { // toggle "off flags" (bit4) on all!
      togAll(); outList(); i &= 0x7f;
    } else {
      inOutGet(outListPtr); //get value if entering line
    }
  }
  if (!(chanAdj & 0x10))OLEDwerdsSm(String((chanAdj & 0x0f) + 1), 114, i, 3); //is on?
  if ((chanAdj & 0x10))OLEDwerdsSm("--", 114, i, 3);
}

void inOutGet(byte ch) { //send 0-15 (ch) returns array value to chanAdj
  switch (lev1Ptr - 1) { //in's
    case 0:
      switch (lev2Ptr) { //out's
        case 0: chanAdj = in1out1[ch]; break;
        case 1: chanAdj = in1out2[ch]; break;
        case 2: chanAdj = in1out3[ch]; break;
      }
      break;
    case 1:
      switch (lev2Ptr) { //out's
        case 0: chanAdj = in2out1[ch]; break;
        case 1: chanAdj = in2out2[ch]; break;
        case 2: chanAdj = in2out3[ch]; break;
      }
      break;
    case 2:
      switch (lev2Ptr) { //out's
        case 0: chanAdj = in3out1[ch]; break;
        case 1: chanAdj = in3out2[ch]; break;
        case 2: chanAdj = in3out3[ch]; break;
      }
      break;
  }
}

void inOutPut(byte ch) { //send 0-15 (ch) returns array value to chanAdj
  allNotesOff(16, lev2Ptr); //turn off all notes on all outputs
  switch (lev1Ptr - 1) { //in's
    case 0:
      switch (lev2Ptr) { //out's
        case 0: in1out1[ch] = chanAdj; break;
        case 1: in1out2[ch] = chanAdj; break;
        case 2: in1out3[ch] = chanAdj; break;
      }
      break;
    case 1:
      switch (lev2Ptr) { //out's
        case 0: in2out1[ch] = chanAdj; break;
        case 1: in2out2[ch] = chanAdj; break;
        case 2: in2out3[ch] = chanAdj; break;
      }
      break;
    case 2:
      switch (lev2Ptr) { //out's
        case 0: in3out1[ch] = chanAdj; break;
        case 1: in3out2[ch] = chanAdj; break;
        case 2: in3out3[ch] = chanAdj; break;
      }
      break;
  }
}

void togAll() {
  inOutGet(0);
  if (!(chanAdj & 0x10)) {
    for (byte i = 0; i < 16; i++) { //all off
      inOutGet(i); chanAdj |= 0x10; inOutPut(i);
    }
  } else {
    for (byte i = 0; i < 16; i++) { //all on
      inOutGet(i); chanAdj &= 0xEF; inOutPut(i);
    }
  }
}

void menuOpt() { //menuLev=5
  OLED_UD(512, 0, 127, 4, 7);
  OLEDwerdsSm("I", 0, 0, 1); OLEDwerdsSm("N", 0, 1, 1); OLEDwerdsSm(String(lev1Ptr), 0, 3, 1);
  switch (optSel) {
    case 0: OLEDwerds(F("CLOCK"), 8, 0, 0); break;
    case 1: OLEDwerds(F("MIDIPC"), 8, 0, 0); break;
    case 2: OLEDwerds(F("TRANSP"), 8, 0, 0); break;
    case 3: OLEDwerds(F("MIDICC"), 8, 0, 0); break;
    case 4: OLEDwerds(F(":EXIT:"), 16, 0, 0); break;
  }
}

void optList() {
  OLED_UD(512, 0, 127, 4, 7);
  switch (optSel) {
    case 0: clockSet(); menuLev = 6; break; //OLEDwerds("CLOCK", 8, 0, 0)
    case 1: pcSet(); menuLev = 7; break; //OLEDwerds("MIDIPC", 8, 0, 0);
    case 2: tranSet(); menuLev = 8; break; //OLEDwerds("TRANSP", 8, 0, 0);
    case 3: ccSet(); menuLev = 9; break; //OLEDwerds("MIDICC", 8, 0, 0);
    case 4: menuLev = 2; menuLev2(); break; //EXIT
  }
}

void clockSet() { //menuLev=6 <<<<May eveolve into auto-clock
  OLEDwerdsSm(F("CLOCK TO:"), 6, 0, 0); OLEDwerdsSm(F("OUT 1:"), 6, 1, 0); OLEDwerdsSm(F("OUT 2:"), 6, 2, 0); OLEDwerdsSm(F("OUT 3:"), 6, 3, 0);
  //Serial.print("lev1Ptr="); Serial.println(lev1Ptr - 1);
  for (byte i = 0; i < 3; i++) {
    byte k = 0;
    switch (lev1Ptr - 1) {
      case 0: k = clk1[i]; break;
      case 1: k = clk2[i]; break;
      case 2: k = clk3[i]; break;
    }
    if ((k & 0x01)) { //clock on this out?
      OLEDwerdsSm("Y", 48, i + 1, 0);
    } else {
      OLEDwerdsSm("N", 48, i + 1, 0);
    }
    arrow(clkSel + 1);
  }
}
void clockSetSel() { // toggle y/n
  byte k = 0; byte i = clkSel;
  switch (lev1Ptr - 1) {
    case 0: k = clk1[i]; break;
    case 1: k = clk2[i]; break;
    case 2: k = clk3[i]; break;
  }
  if ((k & 0x01)) { // just turn off
    switch (lev1Ptr - 1) {
      case 0: clk1[i] &= 0xFE; break;
      case 1: clk2[i] &= 0xFE; break;
      case 2: clk3[i] &= 0xFE; break;
    }
    //Serial.print("Turned OFF clk"); Serial.print(lev1Ptr); Serial.print(" ="); Serial.println(clk1[i], HEX);
  } else { //turn on this clk for output#_ turn others for this output off
    switch (lev1Ptr - 1) {
      case 0: clk1[i] |= 0x01; clk2[i] &= 0xFE; clk3[i] &= 0xFE; break;
      case 1: clk2[i] |= 0x01; clk1[i] &= 0xFE; clk3[i] &= 0xFE; break;
      case 2: clk3[i] |= 0x01; clk2[i] &= 0xFE; clk1[i] &= 0xFE; break;
    }
    // Serial.print("Turned ON clk"); Serial.print(lev1Ptr); Serial.print(" ="); Serial.println(clk1[i], HEX);
  }
  onRelease = 1; //clockSet();
}

void pcSet() {
  OLEDwerdsSm(F("PROGRAM CHANGE TO:"), 6, 0, 0); OLEDwerdsSm(F("OUT 1:"), 6, 1, 0); OLEDwerdsSm(F("OUT 2:"), 6, 2, 0); OLEDwerdsSm(F("OUT 3:"), 6, 3, 0);
  for (byte i = 0; i < 3; i++) {
    byte k = 0;
    switch (lev1Ptr - 1) {
      case 0: k = clk1[i]; break;
      case 1: k = clk2[i]; break;
      case 2: k = clk3[i]; break;
    }
    if ((k & 0x04)) { //PC's on this out for this Input?
      OLEDwerdsSm("Y", 48, i + 1, 0);
    } else {
      OLEDwerdsSm("N", 48, i + 1, 0);
    }
    arrow(pcSel + 1);
  }
}

void pcSetSel() {
  byte k = 0; byte i = pcSel;
  switch (lev1Ptr - 1) {
    case 0: k = clk1[i]; break;
    case 1: k = clk2[i]; break;
    case 2: k = clk3[i]; break;
  }
  if ((k & 0x04)) { // just turn off PC for this input to that output
    switch (lev1Ptr - 1) {
      case 0: clk1[i] &= 0xFB; break;
      case 1: clk2[i] &= 0xFB; break;
      case 2: clk3[i] &= 0xFB; break;
    }
    // Serial.print("Turned OFF PC"); Serial.print(lev1Ptr); Serial.print(" ="); Serial.println(clk1[i], HEX);
  } else { //turn on this input's PC for output#_
    switch (lev1Ptr - 1) {
      case 0: clk1[i] |= 0x04;  break;
      case 1: clk2[i] |= 0x04;  break;
      case 2: clk3[i] |= 0x04;  break;
    }
    //Serial.print("Turned ON PC"); Serial.print(lev1Ptr); Serial.print(" ="); Serial.println(clk1[i], HEX);
  }
  onRelease = 2; //update display after release
}

void tranSet() {
  String t = String(lev1Ptr);
  OLEDwerdsSm(F("MIDI TRANSPORT TO:"), 6, 0, 0);
  for (byte i = 1; i < 4; i++) {
    OLEDwerdsSm(F("IN:"), 6, i, 0); OLEDwerdsSm(t, 24, i, 0);
  }
  OLEDwerdsSm(F("> OUT 1:"), 36, 1, 0); OLEDwerdsSm(F("> OUT 2:"), 36, 2, 0); OLEDwerdsSm(F("> OUT 3:"), 36, 3, 0);
  for (byte i = 0; i < 3; i++) {
    byte k = 0;
    switch (lev1Ptr - 1) {
      case 0: k = clk1[i]; break;
      case 1: k = clk2[i]; break;
      case 2: k = clk3[i]; break;
    }
    if ((k & 0x02)) { //PC's on this out for this Input?
      OLEDwerdsSm("Y", 90, i + 1, 0);
    } else {
      OLEDwerdsSm("N", 90, i + 1, 0);
    }
    arrow(tranSel + 1);
  }
}

void tranSetSel() { //transpot on/off?
  byte k = 0; byte i = tranSel;
  switch (lev1Ptr - 1) {
    case 0: k = clk1[i]; break;
    case 1: k = clk2[i]; break;
    case 2: k = clk3[i]; break;
  }
  if ((k & 0x02)) { // just turn off transport mess. for this input to that output
    switch (lev1Ptr - 1) {
      case 0: clk1[i] &= 0xFD; break;
      case 1: clk2[i] &= 0xFD; break;
      case 2: clk3[i] &= 0xFD; break;
    }
    //Serial.print("Turned OFF MIDItransp"); Serial.print(lev1Ptr); Serial.print(" ="); Serial.println(clk1[i], HEX);
  } else { //turn on this input's transports for output#_
    switch (lev1Ptr - 1) {
      case 0: clk1[i] |= 0x02;  break;
      case 1: clk2[i] |= 0x02;  break;
      case 2: clk3[i] |= 0x02;  break;
    }
    //Serial.print("Turned ON MIDI transp"); Serial.print(lev1Ptr); Serial.print(" ="); Serial.println(clk1[i], HEX);
  }
  onRelease = 3; //update display after release
}

void ccSet() {
  String t = String(lev1Ptr);
  OLEDwerdsSm(F("MIDI CC'S TO:"), 6, 0, 0);
  for (byte i = 1; i < 4; i++) {
    OLEDwerdsSm(F("IN:"), 6, i, 0); OLEDwerdsSm(t, 24, i, 0);
  }
  OLEDwerdsSm(F("> OUT 1:"), 36, 1, 0); OLEDwerdsSm(F("> OUT 2:"), 36, 2, 0); OLEDwerdsSm(F("> OUT 3:"), 36, 3, 0);
  for (byte i = 0; i < 3; i++) {
    byte k = 0;
    switch (lev1Ptr - 1) {
      case 0: k = clk1[i]; break;
      case 1: k = clk2[i]; break;
      case 2: k = clk3[i]; break;
    }
    if ((k & 0x08)) { //PC's on this out for this Input?
      OLEDwerdsSm("Y", 90, i + 1, 0);
    } else {
      OLEDwerdsSm("N", 90, i + 1, 0);
    }
    arrow(ccSel + 1);
  }
}

void ccSetSel() {
  byte k = 0; byte i = ccSel;
  switch (lev1Ptr - 1) {
    case 0: k = clk1[i]; break;
    case 1: k = clk2[i]; break;
    case 2: k = clk3[i]; break;
  }
  if ((k & 0x08)) { // just turn off transport mess. for this input to that output
    switch (lev1Ptr - 1) {
      case 0: clk1[i] &= 0xF7; break;
      case 1: clk2[i] &= 0xF7; break;
      case 2: clk3[i] &= 0xF7; break;
    }
    //Serial.print("Turned OFF MIDI CC"); Serial.print(lev1Ptr); Serial.print(" ="); Serial.println(clk1[i], HEX);
  } else { //turn on this input's transports for output#_
    switch (lev1Ptr - 1) {
      case 0: clk1[i] |= 0x08;  break;
      case 1: clk2[i] |= 0x08;  break;
      case 2: clk3[i] |= 0x08;  break;
    }
    //Serial.print("Turned ON MIDI CC"); Serial.print(lev1Ptr); Serial.print(" ="); Serial.println(clk1[i], HEX);
  }
  onRelease = 4; //update display after release
}

void splitList() { //display/refresh SPLIT menu
  ip = splitFid; // splitSel is actual line select #
  //byte sch = 0; //pull from split1,2,3
  OLED_UD(512, 0, 127, 4, 7);
  arrow(splitSel - splitFid);
  if (!splitEd) splitGet();
  sf0(); sf1(); sf2(); sf3(); sf4(); sf5(); sf6(); sf7(); sf8(); sf9(); sf10(); sf11(); sf12(); sf13(); sf14(); sf15();
  splitPut();
}

void splitGet() {
  switch (lev1Ptr - 1) { //get values (if config=9 then note offsets are fixed at 36 (eep mem ran out!!)
    case 0: spLow = split1low; spHi = split1hi; spCh1 = split1chan1 & 0x0f; spCh2 = split1chan2 & 0x0f; spCh3 = split1chan3 & 0x0f; spOut1 = split1chan1 >> 4;
      spOut2 = split1chan2 >> 4; spOut3 = (split1chan3 >> 4); spNO1 = split1NO1; spNO2 = split1NO2; spNO3 = split1NO3;
      spPC1 = split1PC1; spPC2 = split1PC2; spPC3 = split1PC3; sch = split1;
      break;
    case 1: spLow = split2low; spHi = split2hi; spCh1 = split2chan1 & 0x0f; spCh2 = split2chan2 & 0x0f; spCh3 = split2chan3 & 0x0f; spOut1 = split2chan1 >> 4;
      spOut2 = split2chan2 >> 4; spOut3 = (split2chan3 >> 4); spNO1 = split2NO1; spNO2 = split2NO2; spNO3 = split2NO3;
      spPC1 = split2PC1; spPC2 = split2PC2; spPC3 = split2PC3; sch = split2;
      break;
    case 2: spLow = split3low; spHi = split3hi; spCh1 = split3chan1 & 0x0f; spCh2 = split3chan2 & 0x0f; spCh3 = split3chan3 & 0x0f; spOut1 = split3chan1 >> 4;
      spOut2 = split3chan2 >> 4; spOut3 = (split3chan3 >> 4); spNO1 = split3NO1; spNO2 = split3NO2; spNO3 = split3NO3;
      spPC1 = split3PC1; spPC2 = split3PC2; spPC3 = split3PC3; sch = split3;
      break;
  }

}

void splitPut() {
  switch (lev1Ptr - 1) { //put values back
    case 0: split1low = spLow; split1hi = spHi; split1chan1 = spCh1; split1chan2 = spCh2; split1chan3 = spCh3; split1chan1 |= spOut1 << 4;
      split1chan2 |= spOut2 << 4; split1chan3 |= (spOut3 << 4); split1NO1 = spNO1; split1NO2 = spNO2; split1NO3 = spNO3;
      split1PC1 = spPC1; split1PC2 = spPC2; split1PC3 = spPC3; split1 = sch;
      break;
    case 1: split2low = spLow; split2hi = spHi; split2chan1 = spCh1; split2chan2 = spCh2; split2chan3 = spCh3; split2chan1 |= spOut1 << 4;
      split2chan2 |= spOut2 << 4; split2chan3 |= (spOut3 << 4); split2NO1 = spNO1; split2NO2 = spNO2; split2NO3 = spNO3;
      split2PC1 = spPC1; split2PC2 = spPC2; split2PC3 = spPC3; split2 = sch;
      break;
    case 2: split3low = spLow; split3hi = spHi; split3chan1 = spCh1; split3chan2 = spCh2; split3chan3 = spCh3; split3chan1 |= spOut1 << 4;
      split3chan2 |= spOut2 << 4; split3chan3 |= (spOut3 << 4); split3NO1 = spNO1; split3NO2 = spNO2; split3NO3 = spNO3;
      split3PC1 = spPC1; split3PC2 = spPC2; split3PC3 = spPC3; split3 = sch;
      break;
  }
}

void splitAdj() {
  splitGet();
  ip = splitFid;
  allNotesOff(16, 4); //turn off all notes for variety of changes!
  switch (splitSel) {
    case 0: allNotesOff(16, 4); sch += bDir; if (sch == 255) sch = 16;
      if (sch > 16) sch = 0;
      sf0(); break; // 0-16 (16=off)
    case 1: spLow += bDir; spLow &= 0x7F; sf1(); break;
    case 2: spHi += bDir; spHi &= 0x7f; sf2(); break;
    case 3: spNO1 += bDir; spNO1 &= 0x7F; sf3(); break;
    case 4: spNO2 += bDir; spNO2 &= 0x7F; sf4(); break;
    case 5: spNO3 += bDir; spNO3 &= 0x7F; sf5(); break;
    case 6:  spCh1 += bDir; spCh1 &= 0x0f; sf6(); break;
    case 7:  spCh2 += bDir; spCh2 &= 0x0f; sf7(); break;
    case 8:  spCh3 += bDir; spCh3 &= 0x0f; sf8(); break;
    case 9: spPC1 += bDir; if (spPC1 == 255)spPC1 = 128;
      if (spPC1 > 128)spPC1 = 0;
      sf9(); break;
    case 10: spPC2 += bDir;  if (spPC2 == 255)spPC2 = 128;
      if (spPC2 > 128)spPC2 = 0;
      sf10(); break;
    case 11: spPC3 += bDir;  if (spPC3 == 255)spPC3 = 128;
      if (spPC3 > 128)spPC3 = 0;
      sf11(); break;
    case 12:  spOut1 += bDir; spOut1 &= 0x03; sf12(); break;
    case 13:  spOut2 += bDir; spOut2 &= 0x03; sf13(); break;
    case 14:  spOut3 += bDir; spOut3 &= 0x03; sf14(); break;
    case 15: overLap[lev1Ptr - 1] += (bDir * 2);
      if (overLap[lev1Ptr - 1] > 14) overLap[lev1Ptr - 1] = 14;
      if (overLap[lev1Ptr - 1] > 253) overLap[lev1Ptr - 1] = 0;
      sf15();
      break;
  }
  splitPut();
  if ( lev1Ptr == 1) {
    splitPC1 = 0; splitPC1send();
  }
  if ( lev1Ptr == 2) {
    splitPC2 = 0; splitPC2send();
  }
  if ( lev1Ptr == 3) {
    splitPC3 = 0; splitPC3send();
  }
}

void sf0() {
  if (splitFid < 1) { //channel
    OLEDwerdsSm(F("IN:"), 6, ip, 3); OLEDwerdsSm(String(lev1Ptr), 24, ip, 2); OLEDwerdsSm(F("SPLIT CH:"), 36, ip, 9);
    if (sch != 16) {
      OLEDwerdsSm(String(sch + 1), 90, ip + splitEd, 2);
    } else {
      OLEDwerdsSm(F("--"), 90, ip + splitEd, 2);
    }
  }
}
void sf1() {
  if (splitFid < 2) {
    OLEDwerdsSm(F("SPLIT LOW:<"), 6, 1 - ip, 0); OLEDwerdsSm(String(spLow), 78, 1 - ip + splitEd, 3);
  }
}
void sf2() {
  if (splitFid < 3) {
    OLEDwerdsSm(F("SPLIT HIGH:="), 6, 2 - ip, 0); OLEDwerdsSm(String(spHi), 78, 2 - ip + splitEd, 3);
  }
}
void sf3() {
  if (splitFid < 4) {
    OLEDwerdsSm(F("LOW START NOTE:"), 6, 3 - ip, 0); OLEDwerdsSm(String(spNO1), 102, 3 - ip + splitEd, 3);
  }
}
void sf4() {
  if (splitFid > 0 && splitFid < 5) {
    OLEDwerdsSm(F("MID START NOTE:"), 6, 4 - ip, 0); OLEDwerdsSm(String(spNO2), 102, 4 - ip + splitEd, 3);
  }
}
void sf5() {
  if (splitFid > 1 && splitFid < 6) {
    OLEDwerdsSm(F("HIGH START NOTE:"), 6, 5 - ip, 0); OLEDwerdsSm(String(spNO3), 102, 5 - ip + splitEd, 3);
  }
}
void sf6() {
  if (splitFid > 2 && splitFid < 7) {
    OLEDwerdsSm(F("LOW CHANNEL:"), 6, 6 - ip, 0); OLEDwerdsSm(String(spCh1 + 1), 84, 6 - ip + splitEd, 3);
  }
}
void sf7() {
  if (splitFid > 3 && splitFid < 8) {
    OLEDwerdsSm(F("MID CHANNEL:"), 6, 7 - ip, 0); OLEDwerdsSm(String(spCh2 + 1), 84, 7 - ip + splitEd, 3);
  }
}
void sf8() {
  if (splitFid > 4 && splitFid < 9) {
    OLEDwerdsSm(F("HIGH CHANNEL:"), 6, 8 - ip, 0); OLEDwerdsSm(String(spCh3 + 1), 84, 8 - ip + splitEd, 3);
  }
}
void sf9() {
  if (splitFid > 5 && splitFid < 10) {
    OLEDwerdsSm(F("LOW INSTR:"), 6, 9 - ip, 0);
    if (spPC1 < 128) {
      OLEDwerdsSm(String(spPC1 + 1), 78, 9 - ip + splitEd, 3);
    } else {
      OLEDwerdsSm(F("--"), 78, 9 - ip + splitEd, 3);
    }
  }
}
void sf10() {
  if (splitFid > 6 && splitFid < 11) {
    OLEDwerdsSm(F("MID INSTR:"), 6, 10 - ip, 0);
    if (spPC2 < 128) {
      OLEDwerdsSm(String(spPC2 + 1), 78, 10 - ip + splitEd, 3);
    } else {
      OLEDwerdsSm(F("--"), 78, 10 - ip + splitEd, 3);
    }
  }
}
void sf11() {
  if (splitFid > 7 && splitFid < 12) {
    OLEDwerdsSm(F("HIGH INSTR:"), 6, 11 - ip, 0);
    if (spPC3 < 128) {
      OLEDwerdsSm(String(spPC3 + 1), 78, 11 - ip + splitEd, 3);
    } else {
      OLEDwerdsSm(F("--"), 78, 11 - ip + splitEd, 3);
    }
  }
}
void sf12() {
  if (splitFid > 8 && splitFid < 13) {
    OLEDwerdsSm(F("LOW OUTPUT:"), 6, 12 - ip, 0);
    if (spOut1 > 0) OLEDwerdsSm(String(spOut1), 90, 12 - ip + splitEd, 1); //--,1,2,3
    if (spOut1 == 0) OLEDwerdsSm(F("--"), 90, 12 - ip + splitEd, 1);
  }
}
void sf13() {
  if (splitFid > 9 && splitFid < 14) {
    OLEDwerdsSm(F("MID OUTPUT:"), 6, 13 - ip, 0);
    if (spOut2 > 0)OLEDwerdsSm(String(spOut2), 90, 13 - ip + splitEd, 1); //--,1,2,3
    if (spOut2 == 0) OLEDwerdsSm(F("--"), 90, 13 - ip + splitEd, 1);
  }
}
void sf14() {
  if (splitFid > 10 && splitFid < 15) {
    OLEDwerdsSm(F("HIGH OUTPUT:"), 6, 14 - ip, 0);
    if (spOut3 > 0)OLEDwerdsSm(String(spOut3), 90, (14 - ip) + splitEd, 1); //--,1,2,3
    if (spOut3 == 0) OLEDwerdsSm(F("--"), 90, 14 - ip + splitEd, 1);
  }
}

void sf15() { //overLap[lev1Ptr - 1]
  if (splitFid > 11 && splitFid < 16) {
    OLEDwerdsSm(F("OVERLAP:"), 6, 15 - ip, 0);
    OLEDwerdsSm(String(overLap[lev1Ptr - 1]), 60, (15 - ip) + splitEd, 2);
  }
}





void ret2standby() {
  menuLev = 0; lev1Ptr = 0; splitEd = 0; letEdit = 0;
}
