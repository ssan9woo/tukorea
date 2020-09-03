package com.example.client_phone;

import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;

import android.media.Image;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import java.awt.font.TextAttribute;
import java.util.Set;

public class findcarActivity extends AppCompatActivity {

    ImageView[] region = new ImageView[6];

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.find);

        ActionBar actionBar = getSupportActionBar();
        assert actionBar != null;
        actionBar.setTitle("주차구역 찾기");

        region[0] = findViewById(R.id.imageA);
        region[1] = findViewById(R.id.imageB);
        region[2] = findViewById(R.id.imageC);
        region[3] = findViewById(R.id.imageD);
        region[4] = findViewById(R.id.imageE);
        region[5] = findViewById(R.id.imageF);

        String s = ((ParkingAreaActivity)ParkingAreaActivity.mContext).GiveRegion();
        int num = s.charAt(0) - 65;

        for(int i = 0; i < 6; i++){
            if(i == num)
                region[i].setVisibility(View.VISIBLE);
            else
                region[i].setVisibility(View.INVISIBLE);
        }
    }
}
