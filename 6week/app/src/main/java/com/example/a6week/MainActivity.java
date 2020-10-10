package com.example.a6week;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.Display;
import android.view.Gravity;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    Button button;
    Button button2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        setTitle("토스트 연습!!");

        button = findViewById(R.id.button);
        button2 = findViewById(R.id.button2);

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Toast msg = Toast.makeText(MainActivity.this,"안녕하세용",Toast.LENGTH_SHORT);

                Display display = ((WindowManager) getSystemService(WINDOW_SERVICE)).getDefaultDisplay();

                int x = (int) (Math.random() * display.getWidth());
                int y = (int) (Math.random() * display.getHeight());

                msg.setGravity(Gravity.TOP, x,y);
                msg.show();

            }
        });

        button2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(getApplicationContext(),SecondActivity.class);
                startActivity(intent);
            }
        });
    }
}