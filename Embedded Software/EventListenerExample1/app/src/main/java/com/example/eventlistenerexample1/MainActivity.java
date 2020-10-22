package com.example.eventlistenerexample1;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import org.w3c.dom.Text;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button car = findViewById(R.id.car);
        Button air = findViewById(R.id.air);
        final TextView text = findViewById(R.id.text);


        car.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                text.setText("car");
            }
        });

        air.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                text.setText("air");
            }
        });
    }
}