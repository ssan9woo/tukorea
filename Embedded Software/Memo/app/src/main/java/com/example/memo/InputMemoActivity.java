package com.example.memo;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class InputMemoActivity extends AppCompatActivity {
    EditText edit1,edit2;
    Button editComplete;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_input_memo);
        edit1 = findViewById(R.id.edit1);
        edit2 = findViewById(R.id.edit2);
        editComplete = findViewById(R.id.editComplete);

        editComplete.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent();
                if(!edit1.getText().toString().equals("") && !edit2.getText().toString().equals("")){
                    intent.putExtra("title",edit1.getText().toString());
                    intent.putExtra("contents",edit2.getText().toString());
                }
                setResult(RESULT_OK,intent);
                finish();
            }
        });
    }
}