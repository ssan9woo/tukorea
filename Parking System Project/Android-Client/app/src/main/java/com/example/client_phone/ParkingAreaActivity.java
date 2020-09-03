package com.example.client_phone;

import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.Intent;
import android.graphics.Region;
import android.media.Image;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

public class ParkingAreaActivity extends AppCompatActivity {

    Button goToRouteActivity;
    Intent intent;
    String region = "";
    EditText edit;
    public static Context mContext;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_parking_area);
        mContext = this;
        intent = new Intent(getApplicationContext(),findcarActivity.class);

        ActionBar actionBar = getSupportActionBar();
        assert actionBar != null;
        actionBar.setTitle("KPU PARKING ZONE");

        ImageView[] cars = new ImageView[6];

        goToRouteActivity = findViewById(R.id.button);
        edit = findViewById(R.id.edit);

        //Car image set-------------------------------
        cars[0] = findViewById(R.id.carA);
        cars[1] = findViewById(R.id.carB);
        cars[2] = findViewById(R.id.carC);
        cars[3] = findViewById(R.id.carD);
        cars[4] = findViewById(R.id.carE);
        cars[5] = findViewById(R.id.carF);
        //--------------------------------------------


        String reg = ((MainActivity)MainActivity.mContext).Region(); //Get region Flag
        for(int i = 0 ; i < reg.length(); i++){
            if(reg.charAt(i) == '1'){
                cars[i].setVisibility(View.VISIBLE);
            }
            else if(reg.charAt(i) == '0'){
                cars[i].setVisibility(View.INVISIBLE);
            }
        }


        goToRouteActivity.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(!edit.getText().toString().equals("")){
                    region = edit.getText().toString();
                    startActivity(intent);
                }
            }
        });
    }


    public String GiveRegion(){
        return region;
    }
}


