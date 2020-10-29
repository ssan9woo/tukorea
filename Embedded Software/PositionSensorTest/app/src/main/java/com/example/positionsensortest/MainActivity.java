package com.example.positionsensortest;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import java.util.Arrays;

public class MainActivity extends AppCompatActivity implements SensorEventListener{
    TextView ambient_pressure, illuminance;
    SensorManager sm;
    Sensor sensor_ambient_pressure, sensor_illuminance;

    @SuppressLint("SetTextI18n")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ambient_pressure = findViewById(R.id.ambient_pressure);
        illuminance = findViewById(R.id.illuminance);

        sm = (SensorManager) getSystemService(SENSOR_SERVICE);

        sensor_ambient_pressure = sm.getDefaultSensor(Sensor.TYPE_PRESSURE);
        sensor_illuminance = sm.getDefaultSensor(Sensor.TYPE_LIGHT);

    }

    protected void onResume(){
        super.onResume();
        sm.registerListener(this,sensor_ambient_pressure,SensorManager.SENSOR_DELAY_NORMAL);
        sm.registerListener(this,sensor_illuminance,SensorManager.SENSOR_DELAY_NORMAL);
    }

    protected void onPause(){
        super.onPause();
        sm.unregisterListener(this);
    }

    public void onAccuracyChanged(Sensor sensor, int accuracy){

    }

    public void onSensorChanged(SensorEvent event){
        switch (event.sensor.getType()){
            case Sensor.TYPE_PRESSURE:
                ambient_pressure.setText(Arrays.toString(event.values));
                break;
            case Sensor.TYPE_LIGHT:
                illuminance.setText(Arrays.toString(event.values));
                break;
        }
    }
}