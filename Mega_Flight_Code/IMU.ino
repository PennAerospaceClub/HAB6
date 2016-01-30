void initIMU(){
  if(!accel.begin())
  {
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
  }
  if(!mag.begin())
  {
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  if(!gyro.begin())
  {
    Serial.print("Ooops, no L3GD20 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
}

void runIMU(){
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_event_t gyro_event;
  sensors_vec_t   orientation;
  
  imuDataString = "";     
  imuDataString.concat(hour()); imuDataString.concat(":"); imuDataString.concat(minute()); imuDataString.concat(":"); imuDataString.concat(second()); imuDataString.concat(",");
    
  accel.getEvent(&accel_event);
  if (dof.accelGetOrientation(&accel_event, &orientation))
  {
    /* 'orientation' should have valid .roll and .pitch fields */
    imuDataString.concat(orientation.roll); imuDataString.concat(",");
    imuDataString.concat(orientation.pitch); imuDataString.concat(",");
  }
  
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  mag.getEvent(&mag_event);
  if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
  {
    imuDataString.concat(orientation.heading); imuDataString.concat(",");
  }

  /* Display the results (gyrocope values in rad/s) */
  gyro.getEvent(&gyro_event);
  imuDataString.concat(gyro_event.gyro.x); imuDataString.concat(",");
  imuDataString.concat(gyro_event.gyro.y); imuDataString.concat(",");
  imuDataString.concat(gyro_event.gyro.z); imuDataString.concat(",");
 }

