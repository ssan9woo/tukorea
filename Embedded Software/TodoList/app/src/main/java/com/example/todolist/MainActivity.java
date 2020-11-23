package com.example.todolist;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Context;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    @SuppressLint("StaticFieldLeak")
    static ListViewAdapter adapter = null;

    int numberOfTodoLists = 0;
    ListView listView;
    Button createTodo,delete;
    EditText input;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        createTodo = findViewById(R.id.createTodo);
        delete = findViewById(R.id.delete);
        input = findViewById(R.id.input);

        //Adapter
        adapter = new ListViewAdapter(this);
        listView = (ListView) findViewById(R.id.list);
        listView.setAdapter(adapter);

        createTodo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(!input.getText().toString().equals("")){
                    adapter.addItem(false,input.getText().toString(),false);
                    input.setText("");
                    // System.out.println(((ListViewAdapter) ListViewAdapter.listViewContext).getCount());
                }
            }
        });

        delete.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                adapter.removeAll();
            }
        });

        class Clock implements Runnable{
            Calendar cal = Calendar.getInstance();
            @SuppressLint("SimpleDateFormat")
            SimpleDateFormat sdf = new SimpleDateFormat("MM/dd HH:mm:ss");

            @Override
            public void run(){
                while(true){
                    Date now = new Date();
                    String time = sdf.format(now);
                    String todoCount = "Today's TodoList : " + String.valueOf(numberOfTodoLists);

                    //textLcd(time,todoCount);
                    try{
                        Thread.sleep(1000);
                    }catch (Exception e){
                        e.printStackTrace();
                    }
                }
            }
        }

        Clock clock = new Clock();
        Thread clockThread = new Thread(clock);
        clockThread.start();
    }

    public native int textLcd(String s1, String s2);
    static {
        System.loadLibrary("native-lib");
    }
}


