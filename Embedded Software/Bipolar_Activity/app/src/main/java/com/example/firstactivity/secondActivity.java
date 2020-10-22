package com.example.firstactivity;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class secondActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_second);
        setTitle("두번째");

        Intent intent = getIntent();
        final int hapValue = intent.getIntExtra("num1",0) + intent.getIntExtra("num2",0);

        Button b = findViewById(R.id.btnReturn);

        b.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent out = new Intent(getApplicationContext(),MainActivity.class);
                out.putExtra("hap",hapValue);
                setResult(RESULT_OK,out);
                finish();
            }
        });
    }
}