package com.example.todolist;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class LoginActivity extends AppCompatActivity {
    final static String ID = "sangwoo";
    final static String PW = "1234";
    EditText inputID, inputPW;
    Button loginButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        inputID = findViewById(R.id.inputID);
        inputPW = findViewById(R.id.inputPW);
        loginButton = findViewById(R.id.loginButton);

        //editText에서 pushSwitch로 입력받기
        loginButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(inputID.getText().toString().equals(ID) && inputPW.getText().toString().equals(PW)){
                    Intent intent = new Intent(getApplicationContext(),CalendarActivity.class);
                    startActivity(intent);
                } else{
                    //부저
                    //Toast
                    Toast.makeText(LoginActivity.this, "로그인 오류", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }
}