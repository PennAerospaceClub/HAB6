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

  Serial.println("IMU1");
  
  imuData = "";     
  imuData.concat(hour()); imuData.concat(":"); imuData.concat(minute()); imuData.concat(":"); imuData.concat(second()); imuData.concat(",");
    
  accel.getEvent(&accel_event);
  if (dof.accelGetOrientation(&accel_event, &orientation))
  {
    /* 'orientation' should have valid .roll and .pitch fields */
    imuData.concat(orientation.roll); imuData.concat(",");
    imuData.concat(orientation.pitch); imuData.concat(",");
  }
  else{
    imuData.concat(","); imuData.concat(",");
  }

  Serial.println("IMU2");
  
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  mag.getEvent(&mag_event);
  if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
  {
    imuData.concat(orientation.heading); imuData.concat(",");
  }
  else{
    imuData.concat(",");
  }

  Serial.println("IMU3");

  /* Display the results (gyrocope values in rad/s) */
  gyro.getEvent(&gyro_event);
  imuData.concat(gyro_event.gyro.x); imuData.concat(",");
  imuData.concat(gyro_event.gyro.y); imuData.concat(",");
  imuData.concat(gyro_event.gyro.z); imuData.concat(",");
 }

