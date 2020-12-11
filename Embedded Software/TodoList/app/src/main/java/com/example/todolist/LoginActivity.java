package com.example.todolist;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.util.Timer;
import java.util.TimerTask;

public class LoginActivity extends AppCompatActivity {
    final static String ID = "sangwoo";
    final static String PW = "1234";
    EditText inputID, inputPW;
    Button loginButton;
    boolean flag = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        inputID = findViewById(R.id.inputID);
        inputPW = findViewById(R.id.inputPW);
        loginButton = findViewById(R.id.loginButton);

        //임시
//        inputID.setText(ID);
//        inputPW.setText(PW);

        //editText에서 pushSwitch로 입력받기
        loginButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(inputID.getText().toString().equals(ID) && inputPW.getText().toString().equals(PW)){
                    Intent intent = new Intent(getApplicationContext(),CalendarActivity.class);
                    startActivity(intent);
                    Toast.makeText(getApplicationContext(),"start",Toast.LENGTH_SHORT).show();
                } else{
                    //부저
                    buzzer(1);
                    Handler delayHandler = new Handler();
                    delayHandler.postDelayed(new Runnable() {
                        @Override
                        public void run() {
                            buzzer(0);
                        }
                    }, 200);
                    Toast.makeText(LoginActivity.this, "로그인 오류", Toast.LENGTH_SHORT).show();
                }
            }
        });

        TimerTask task = new TimerTask() {
            Handler handler = new Handler();
            @Override
            public void run() {
                handler.postDelayed(new Runnable() {
                    @SuppressLint("SetTextI18n")
                    @Override
                    public void run() {
                        int value;
                        value = DeviceOpen();

                        if(value != -1)
                            value = ReceivePushSwitchValue();
                        if(value != -1)
                            DeviceClose();

                        //click 구현
                        String str = Integer.toString(value);

                        if(!flag) {
                            if (!str.equals("0")) {
                                flag = true;

                                String s = "";
                                if (str.equals("1"))
                                    s = "1";
                                else if (str.equals("2"))
                                    s = "2";
                                else if (str.equals("4"))
                                    s = "3";
                                else if (str.equals("8"))
                                    s = "4";
                                else if (str.equals("16"))
                                    s = "5";
                                else if (str.equals("32"))
                                    s = "6";
                                else if (str.equals("64"))
                                    s = "7";
                                else if (str.equals("128"))
                                    s = "8";
                                else if (str.equals("256"))
                                    s = "9";
                                inputPW.setText(inputPW.getText().toString() + s);
                                System.out.println(str);
                            }
                        }else{
                            flag = false;
                        }
                    }
                },1000);
            }
        };

        Timer t = new Timer();
        t.schedule(task,100,100);
    }

    public native int DeviceOpen();
    public native int DeviceClose();
    public native int ReceivePushSwitchValue();
    public native int buzzer(int x);

    static{
        System.loadLibrary("native-lib");
    }
}