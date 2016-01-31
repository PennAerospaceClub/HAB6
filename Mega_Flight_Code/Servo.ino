void servoCheck(){
  if(nichromeStarted && nichromeFinished){
    releaseServo.write(130);
  }
}

