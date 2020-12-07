package com.example.todolist;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.DividerItemDecoration;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;

public class MainActivity extends AppCompatActivity {
    public static Context mainContext;
    public static ArrayList<TodoList> todoLists;
    public static CustomAdapter mAdapter;
    int day,currentTodoCount, totalTodoCount;

    Button createTodo;
    EditText input;
    TextView text;

    @SuppressLint("SetTextI18n")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //날짜 받아오기
        Intent intent = getIntent();
        setTitle(intent.getStringExtra("date"));
        day = intent.getIntExtra("day",0);

        //todocount
        totalTodoCount = intent.getIntExtra("total",0);
        currentTodoCount = intent.getIntExtra("current",0);

        //TodoCount Text
        text = findViewById(R.id.textTodoCount);
        text.setText("Total Todo : " + totalTodoCount + "   Current Todo : " + currentTodoCount);

        //Percent : LED
        double percent = (currentTodoCount / (double)totalTodoCount) * 100;

        if(percent >= 0 && percent <= 25){
            //0
            System.out.println("0");
        }else if (percent > 25 && percent <= 50){
            //1
            System.out.println("1");
        }else if (percent > 50 && percent <= 75){
            //2
            System.out.println("2");
        }else if (percent > 75 && percent <= 100){
            //3
            System.out.println("3");
        }


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
                    String todoCount = "Today's Todo : " + String.valueOf(totalTodoCount);
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

        ArrayList<TodoList> tt = ((CalendarActivity) CalendarActivity.calendarContext).getDayTodoList(day);
        if(tt != null){
            for(int i = 0 ; i < tt.size(); i++){
                addTodo(tt.get(i));
            }
        }

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
        totalTodoCount += 1;
        TodoList todo = new TodoList(false,s);
        todoLists.add(todo);
        mAdapter.notifyDataSetChanged();
    }

    void deleteTodo(int index){
        totalTodoCount -= 1;
        todoLists.remove(index);
        mAdapter.notifyDataSetChanged();
    }

    void setItemIndexToLast(TodoList todo){
        todoLists.remove(todo);
        todoLists.add(todo);
        todo.isFinished = true;
        mAdapter.notifyDataSetChanged();
    }

    void setItemIndexToFirst(TodoList todo){
        todoLists.remove(todo);
        todoLists.add(0,todo);
        todo.isFinished = false;
        mAdapter.notifyDataSetChanged();
    }

    public void onPause(){
        super.onPause();
        ((CalendarActivity) CalendarActivity.calendarContext).getTodoLists(todoLists,day);
    }
}


