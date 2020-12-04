package com.example.todolist;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.DividerItemDecoration;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

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
    public static Context mainContext;
    private ArrayList<TodoList> todoLists;
    private CustomAdapter mAdapter;

    Button createTodo;
    EditText input;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mainContext = this;

        createTodo = findViewById(R.id.createTodo);
        input = findViewById(R.id.input);

        RecyclerView mRecyclerView = (RecyclerView) findViewById(R.id.recyclerview_main_list);
        LinearLayoutManager mLinearLayoutManager = new LinearLayoutManager(this);
        mRecyclerView.setLayoutManager(mLinearLayoutManager);

        todoLists = new ArrayList<>();
        mAdapter = new CustomAdapter(todoLists);
        mRecyclerView.setAdapter(mAdapter);

        DividerItemDecoration dividerItemDecoration = new DividerItemDecoration(mRecyclerView.getContext(),
                mLinearLayoutManager.getOrientation());
        mRecyclerView.addItemDecoration(dividerItemDecoration);

        createTodo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(!input.getText().toString().equals("")){
                    addTodo(input.getText().toString());
                    input.setText("");
                }
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

                    //textLcd(time,todoCount);
                    try{
                        Thread.sleep(1000);
                    }catch (Exception e){
                        e.printStackTrace();
                    }
                }
            }
        }
//        Clock clock = new Clock();
//        Thread clockThread = new Thread(clock);
//        clockThread.start();
    }

    public native int textLcd(String s1, String s2);
    static {
        System.loadLibrary("native-lib");
    }

    void addTodo(String s){
        TodoList todo = new TodoList(false,s);
        todoLists.add(todo);
        mAdapter.notifyDataSetChanged();
    }

    void deleteTodo(int index){
        todoLists.remove(index);
        mAdapter.notifyDataSetChanged();
    }
}


