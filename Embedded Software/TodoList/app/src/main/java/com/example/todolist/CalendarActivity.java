package com.example.todolist;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import com.prolificinteractive.materialcalendarview.CalendarDay;
import com.prolificinteractive.materialcalendarview.CalendarMode;
import com.prolificinteractive.materialcalendarview.MaterialCalendarView;
import com.prolificinteractive.materialcalendarview.OnDateSelectedListener;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.widget.CalendarView;
import android.widget.TextView;

import org.w3c.dom.Text;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;

public class CalendarActivity extends AppCompatActivity {
    TodoList[][] monthOfList = new TodoList[32][10];
    TextView today, todayList;
    int todayNum = 0;
    public static Context calendarContext;

    @SuppressLint("SetTextI18n")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_calendar);
        today = (TextView) findViewById(R.id.today);
        todayList = (TextView) findViewById(R.id.todayList);


        led(0);
        dot(0);

        calendarContext = this;

        MaterialCalendarView cal = findViewById(R.id.calendarView);
        cal.state().edit()
                .setFirstDayOfWeek(Calendar.SUNDAY)
                .setMinimumDate(CalendarDay.from(2020, 1-1, 1))
                .setMaximumDate(CalendarDay.from(2020, 1-1, 31))
                .setCalendarDisplayMode(CalendarMode.MONTHS)
                .commit();

        cal.addDecorators(
                new SundayDecorator(),
                new SaturdayDecorator(),
                new OneDayDecorator());

        //today : n월 n일
        SimpleDateFormat format = new SimpleDateFormat("MMM dd일", Locale.KOREA);
        Date now = new Date();
        String time = format.format(now);
        today.setText("Today : " + time);
        todayNum = Integer.parseInt(time.substring(3,5));


        cal.setOnDateChangedListener(new OnDateSelectedListener() {
            @Override
            public void onDateSelected(@NonNull MaterialCalendarView widget, @NonNull CalendarDay date, boolean selected) {
                SimpleDateFormat format = new SimpleDateFormat("yyyy년 MMM dd일 [E]", Locale.KOREA);
                Intent intent = new Intent(getApplicationContext(),MainActivity.class);
                intent.putExtra("date",format.format(date.getDate()));
                int day = Integer.parseInt(format.format(date.getDate()).substring(9,11));
                intent.putExtra("day",day);

                //todo count
                intent.putExtra("total",getTotalTodoCount(day));
                intent.putExtra("current",getCurrentTodoCount(day));

                startActivity(intent);
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
                    textLcd(time,"               ");
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

    @SuppressLint("SetTextI18n")
    public void onStart(){
        super.onStart();
        System.out.println("starrt");
        //TextView 설정 -> Current and total
        //todayList : 1. 2. 3. ....
        SimpleDateFormat format = new SimpleDateFormat("dd", Locale.KOREA);
        Date today = new Date();

        int day = todayNum;
        System.out.println("dhahaha : " + day);
        int cnt = 0;

        StringBuilder s = new StringBuilder();
        for(int i = 0; i < monthOfList[day].length; i++){
            if(monthOfList[day][i] != null){
                if(!monthOfList[day][i].isFinished){
                    s.append(cnt).append(". ").append(monthOfList[day][i].todo).append("\n");
                    cnt += 1;
                    System.out.println(s);
                }
            }
        }
        todayList.setText(s);
    }

    public void getTodoLists(ArrayList<TodoList> todoLists, int day){
        TodoList[] d = new TodoList[10];

        for(int i = 0 ; i < todoLists.size(); i++){
            d[i] = todoLists.get(i);
        }
        monthOfList[day] = d;
    }

    public ArrayList<TodoList> getDayTodoList(int day){
        ArrayList<TodoList> dayList = new ArrayList<TodoList>();

        for(int i = 0; i < monthOfList[day].length; i++){
            if(monthOfList[day][i] != null){
                dayList.add(monthOfList[day][i]);
            }
        }

        if(dayList.size() > 0){
            return dayList;
        }
        return null;
    }

    public int getTotalTodoCount(int day){
        int count = 0;
        for(int i = 0; i < monthOfList[day].length; i++){
            if(monthOfList[day][i] != null){
                count += 1;
            }
        }
        return count;
    }

    public int getCurrentTodoCount(int day){
        int count = 0;
        for(int i = 0; i < monthOfList[day].length; i++){
            if(monthOfList[day][i] != null){
                if(!monthOfList[day][i].isFinished){
                    count += 1;
                }
            }
        }
        return count;
    }
    public native int dot(int x);
    public native int led(int x);
    public native int textLcd(String s1, String s2);

    static{
        System.loadLibrary("native-lib");
    }
}