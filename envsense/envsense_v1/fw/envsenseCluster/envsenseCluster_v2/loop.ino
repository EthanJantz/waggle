void loop() {
    #ifdef debug_serial
    Serial.println("...entering Sensor Loop...");
    #endif //debug_serial
    
    #ifdef IR_D6T_44L_06_ADD
    if(EEPROM.read(14+128))
    {
        #ifdef debug_serial
        Serial.println("IR_D6T_44L_06");
        #endif //debug_serial
        data_check = D6T_get_data();
        //         wdt_reset();
    }
    #endif //IR_D6T_44L_06_ADD
    wdt_reset();
    #ifdef SHT15_ADD
    if(EEPROM.read(5+128))
    {
        #ifdef debug_serial
        Serial.println("SHT_15");
        #endif //debug_serial
        SHT15_1_error = SHT15_1_tempSensor.measTemp(&SHT15_1_rawData);
        SHT15_1_temperature = SHT15_1_tempSensor.calcTemp(SHT15_1_rawData);
        SHT15_1_error = SHT15_1_tempSensor.measHumi(&SHT15_1_rawData);
        SHT15_1_humidity = SHT15_1_tempSensor.calcHumi(SHT15_1_rawData, SHT15_1_temperature);
        //         wdt_reset();
    }
    #endif //SHT15_ADD
    wdt_reset();
    #ifdef SHT75_ADD
    if(EEPROM.read(1+128))
    {
        #ifdef debug_serial
        Serial.println("SHT75");
        #endif //debug_serial
        SHT75_1_error = SHT75_1_tempSensor.measTemp(&SHT75_1_rawData);
        SHT75_1_temperature = SHT75_1_tempSensor.calcTemp(SHT75_1_rawData);
        SHT75_1_error = SHT75_1_tempSensor.measHumi(&SHT75_1_rawData);
        SHT75_1_humidity = SHT75_1_tempSensor.calcHumi(SHT75_1_rawData, SHT75_1_temperature);
        //         wdt_reset();
    }
    #endif //SHT75_ADD
    wdt_reset();
    #ifdef GA1A1S201WP_ADD
    if(EEPROM.read(10+128))
    {
        #ifdef debug_serial
        Serial.println("GA1A1S201WP");
        Serial.println("Arrival");
        #endif //debug_serial
        AMBI_1_Value = analogRead(AMBI_1_Pin);
        //         AMBI_1_Value=0;
        //         wdt_reset();
    }
    #endif //GA1A1S201WP_ADD
    wdt_reset();
    #ifdef PhoRes_ADD
    if(EEPROM.read(16+128))
    {
        #ifdef debug_serial
        Serial.println("PhoRes");
        #endif //debug_serial
        PhoRes_1_Value = analogRead(PhoRes_PIN);
        //         wdt_reset();
    }
    #endif //PhoRes_ADD
    wdt_reset();
    #ifdef HTU21D_ADD
    if(EEPROM.read(18+128))
    {
        #ifdef debug_serial
        Serial.println("HTU21D");
        #endif //debug_serial
        HTU21D_T = myHumidity.readTemperature();
        wdt_reset();
        HTU21D_H = myHumidity.readHumidity();
        //         wdt_reset();
    }
    #endif //HTU21D_ADD
    wdt_reset();
    //     Serial.println("GET HERE!!!!!!");
    #ifdef HIH4030_ADD
    if(EEPROM.read(7+128))
    {
        #ifdef debug_serial
        Serial.println("HIH4030");
        #endif //debug_serial
        HIH4030_1_Value = analogRead(HIH4030_PIN);
        //         wdt_reset();
    }
    #endif //HIH4030_ADD
    wdt_reset();
    #ifdef THERMIS_100K_ADD
    if(EEPROM.read(11+128))
    {
        #ifdef debug_serial
        Serial.println("THERMIS_100K");
        #endif //debug_serial
        THERMIS_1_Value = analogRead(THERMIS_100K_Pin);
        //         wdt_reset();
    }
    #endif //THERMIS_100K_ADD
    wdt_reset();
    #ifdef DS18B20_ADD
    if(EEPROM.read(8+128))
    {
        #ifdef debug_serial
        Serial.println("DS18B20");
        #endif //debug_serial
        DS18B20_1_temperature = DS18B20_1_getTemp();
        //         wdt_reset();
    }
    #endif //DS18B20_ADD
    wdt_reset();
    #ifdef TMP421_ADD
    if(EEPROM.read(13+128))
    {
        #ifdef debug_serial
        Serial.println("TMP421");
        #endif //debug_serial
        TMP421_1_temperature = TMP421_1.GetTemperature(); // Some error, check this out!
        //         wdt_reset();
    }
    #endif //TMP421_ADD
    wdt_reset();
    #ifdef RHT03_ADD
    if(EEPROM.read(3+128))
    {
        #ifdef debug_serial
        Serial.println("RHT03");
        #endif //debug_serial
        chk_RHT03_1 = RHT03_1.read22(RHT03_1_PIN);
        //         wdt_reset();
    }
    #endif //RHT03_ADD
    wdt_reset();
    #ifdef BMP180_ADD
    if(EEPROM.read(2+128))
    {
        #ifdef debug_serial
        Serial.println("BMP180");
        #endif //debug_serial
        BMP_180_1.getPressure(&BMP_180_1_pressure);
        BMP_180_1.getTemperature(&BMP_180_1_temperature);
        //         wdt_reset();
    }
    #endif //BMP180_ADD
    wdt_reset();
    #ifdef TMP102_ADD
    if(EEPROM.read(4+128))
    {
        #ifdef debug_serial
        Serial.println("TMP102");
        #endif //debug_serial
        err = Wire.requestFrom(tmp102_1_Address,2);
        if (err != 0) {
            byte tmp102_1_MSB = Wire.read();
            byte tmp102_1_LSB = Wire.read();
            tmp102_1_temperature = (((tmp102_1_MSB << 8) | tmp102_1_LSB) >> 4) * 0.11250 + 32; //it's a 12bit int, using two's compliment for negative
        }
        else {
            tmp102_1_temperature = -999;
        }
        //         wdt_reset();
    }
    #endif
    wdt_reset();
    #ifdef HIH6130_ADD
    if(EEPROM.read(7+128))
    {
        #ifdef debug_serial
        Serial.println("HIH6130");
        #endif //debug_serial
        HIH_6130_1_hih.start();
        wdt_reset();
        HIH_6130_1_hih.update();
        wdt_reset();
    }
    #endif //HIH6130_ADD
    //     Serial.println("GET HERE 2");
    wdt_reset();
    #ifdef MLX90614_ADD
    if(EEPROM.read(9+128))
    {
        #ifdef debug_serial
        Serial.println("MLX90614");
        #endif //debug_serial
        start_err = i2c_start(dev+I2C_WRITE);
        write_err = i2c_write(0x07);
        
        // read
        rep_start_err = i2c_rep_start(dev+I2C_READ);
        data_low = i2c_readAck(); //Read 1 byte and then send ack
        data_high = i2c_readAck(); //Read 1 byte and then send ack
        pec = i2c_readNak();
        i2c_stop();
        wdt_reset();
        //This converts high and low bytes together and processes temperature, MSB is a error bit and is ignored for temps
        tempData = 0x0000; // zero out the data
        
        // This masks off the error bit of the high byte, then moves it left 8 bits and adds the low byte.
        tempData = (double)(((data_high & 0x007F) << 8) + data_low);
        tempData = (tempData * tempFactor)-0.01;
        
        celcius = tempData - 273.15;
        fahrenheit = (celcius*1.8) + 32;
        wdt_reset();
    }
    #endif //MLX90614_ADD
    wdt_reset();
    #ifdef HMC5883_ADD
    if(EEPROM.read(19+128))
    {
        Serial.println("HMC5883");
        mag.getEvent(&HMC5883_event);
        /*
         *       // Changing for Verical mount
         *       HMC5883_heading = atan2(HMC5883_event.magnetic.y, HMC5883_event.magnetic.z);
         * 
         *       // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
         *       // Find yours here: http://www.magnetic-declination.com/
         *       // Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
         *       // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
         *       HMC5883_declinationAngle = -0.052; //Chicago
         *       HMC5883_heading += HMC5883_declinationAngle;
         * 
         *       // Correct for when signs are reversed.
         *       if(HMC5883_heading < 0)
         *           HMC5883_heading += 2*PI;
         * 
         *       // Check for wrap due to addition of declination.
         *       if(HMC5883_heading > 2*PI)
         *           HMC5883_heading -= 2*PI;
         * 
         *       // Convert radians to degrees for readability.
         *       HMC5883_headingDegrees = HMC5883_heading * 180/M_PI;*/
        wdt_reset();
    }
    #endif //HMC5883_ADD
    
    //Interrupt based continuous sampling devices begin here
    for (unsigned int perform_loop = 0; perform_loop < MMA_Buff_size - 10 ; perform_loop ++ )
    {
        increment_time();
        delay(sleep_time_loop);
        wdt_reset();
    }
    //Serial.println("Completed Interrupt Based Continuous Sampling Devices");
    
    #ifdef MMA8452
    if(EEPROM.read(6+128))
    {
        #ifdef debug_serial
        Serial.println("MMA8452");
        Serial.print("Number of samples:");
        Serial.println(q);
        #endif //debug_serial
        wdt_reset();
        calc_MMA_RMS();
    }
    #endif
    wdt_reset();
    sensor_print();
    wdt_reset();
    // #ifdef WindVel_ADD
    // pos = 0;
    // max_airspeed = 0;
    // #endif
    #ifdef MAX4466_ADD
    if(EEPROM.read(12+128))
    {
        #ifdef debug_serial
        Serial.println("MAX4466");
        #endif //debug_serial
        MAX4466_Value = 0;
        wdt_reset();
    }
    #endif
    
    #ifdef MMA8452
    if(EEPROM.read(6+128))
    {
        Serial.println("MMA8452");
        q = 0;
        xmean = 0;
        ymean = 0;
        zmean = 0;
        rt_mean_sq = 0;
        xvar = 0;
        yvar = 0;
        zvar = 0;
        mean_square_var = 0;
        wdt_reset();
    }
    #endif
}