void servoCheck(){
  //if(nichromeStarted && nichromeFinished){
  if(millis() > 15000 && !servoRan){
    releaseServo.write(130);
    servoRan = true;
    Serial.println("Servo");
  }
}

