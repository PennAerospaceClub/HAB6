boolean initSane(){

  if(inBdryBox()){
    Serial.println("bdry");
    inBdry = true;
  }
  if(!isFalling()){
    falling = false;
    Serial.println("fall");
  }
  if(!initSD){
    if(SD.begin(CSPIN)){
      Serial.println("sd");
      initSD = true;
    }
  }
  if(!initUV){
    if (!uv.begin()) {
      Serial.println("uv");
      initUV = true;
    }
  }  
  if(!initServo){
    releaseServo.attach(SERVO_PIN);
    if(releaseServo.attached()){
      Serial.println("servo");
      initServo = true;
    } 
  }    
  
  if(inBdry && !falling && initSD && initUV){
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
    onLED = true;

    logSD("Sanity Passed");
    dataSD("time,roll,pitch,heading,gyrox,gyroy,gyroz,lat,long,alt,uv,fall,bdry,cut");
    
    return true;
  }
  else{
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    return false;
  }
}

boolean inBdryBox(){
  if ((lat < maxLat) && (lat > minLat) && (longit < maxLong) && (longit > minLong) && (alt < maxWantedAlt)){
    return true;
  }
  else{
    return false;
  }
}

boolean isFalling(){
  if (alt + 500 < maxAlt){
    return true;
  }
  else{
    return false;
  }
}


