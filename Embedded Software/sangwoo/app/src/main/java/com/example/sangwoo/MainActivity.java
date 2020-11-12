package com.example.sangwoo;

import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.os.Bundle;
import android.widget.RadioGroup;
import android.widget.TextView;

public class MainActivity extends Activity implements RadioGroup.OnCheckedChangeListener {
    RadioGroup mRadioGroupData;
    RadioGroup mRadioGroupCtrl;

    static {
        System.loadLibrary("fpga-led-jni");
    }
    public native String ReceiveLedValue(int x);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mRadioGroupData = findViewById(R.id.RadioGroup01);
        mRadioGroupData.setOnCheckedChangeListener(this);

        mRadioGroupCtrl = findViewById(R.id.RadioGroup01);
        mRadioGroupCtrl.setOnCheckedChangeListener(this);
    }

    public void onCheckedChanged(RadioGroup group, int checkedid){
        switch (checkedid){
            case R.id.RB01:
                ReceiveLedValue(0);
                break;
            case R.id.RB02:
                ReceiveLedValue(1);
                break;
            case R.id.RB03:
                ReceiveLedValue(2);
                break;
            case R.id.RB04:
                ReceiveLedValue(4);
                break;
            case R.id.RB05:
                ReceiveLedValue(4);
                break;
            case R.id.RB06:
                ReceiveLedValue(5);
                break;
            case R.id.RB07:
                ReceiveLedValue(6);
                break;
            case R.id.RB08:
                ReceiveLedValue(7);
                break;
            case R.id.RB10:
                ReceiveLedValue(9);
                break;
            case R.id.RB20:
                ReceiveLedValue(9);
                break;
        }
    }



}
