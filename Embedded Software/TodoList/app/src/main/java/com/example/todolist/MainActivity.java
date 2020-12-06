package com.example.todolist;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.DividerItemDecoration;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.KeyEvent;
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

import java.lang.reflect.Array;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.Objects;

public class MainActivity extends AppCompatActivity {
    public static Context mainContext;
    public static ArrayList<TodoList> todoLists;
    public static CustomAdapter mAdapter;
    int day;

    Button createTodo;
    EditText input;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //날짜 받아오기
        Intent intent = getIntent();
        setTitle(intent.getStringExtra("date"));
        day = intent.getIntExtra("day",0);

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
        mAdapter.notifyDataSetChanged();

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
        ArrayList<TodoList> tt = ((CalendarActivity) CalendarActivity.calendarContext).abc(day);
        if(tt != null){
            for(int i = 0 ; i < tt.size(); i++){
                addTodo(tt.get(i));
            }
        }

        System.out.println("size : " + todoLists.size());
        mAdapter.notifyDataSetChanged();
    }

    public native int textLcd(String s1, String s2);
    static {
        System.loadLibrary("native-lib");
    }

    void addTodo(boolean isFinished, String todo){
        TodoList to = new TodoList(isFinished,todo);
        todoLists.add(to);
        mAdapter.notifyDataSetChanged();
    }
    void addTodo(TodoList todo){
        todoLists.add(todo);
        mAdapter.notifyDataSetChanged();
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

    void setItemIndexToLast(TodoList todo){
        todoLists.remove(todo);
        todoLists.add(todo);
        todo.isFinished = true;
        mAdapter.notifyDataSetChanged();
        System.out.println("last");
    }

    void setItemIndexToFirst(TodoList todo){
        todoLists.remove(todo);
        todoLists.add(0,todo);
        todo.isFinished = false;
        mAdapter.notifyDataSetChanged();
        System.out.println("first");
    }

    public void onPause(){
        super.onPause();
        ((CalendarActivity) CalendarActivity.calendarContext).getTodoLists(todoLists,day);
    }

    public void getInitList(ArrayList<TodoList> todoLists){
        MainActivity.todoLists = todoLists;
        mAdapter.notifyDataSetChanged();

        System.out.println(MainActivity.todoLists);
    }

}


