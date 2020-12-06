package com.example.todolist;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import com.prolificinteractive.materialcalendarview.CalendarDay;
import com.prolificinteractive.materialcalendarview.CalendarMode;
import com.prolificinteractive.materialcalendarview.MaterialCalendarView;
import com.prolificinteractive.materialcalendarview.OnDateSelectedListener;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.widget.CalendarView;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Calendar;
import java.util.Locale;

public class CalendarActivity extends AppCompatActivity {
    TodoList[][] monthOfList = new TodoList[32][10];

    public static Context calendarContext;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_calendar);
        calendarContext = this;

        MaterialCalendarView cal = findViewById(R.id.calendarView);
        cal.state().edit()
                .setFirstDayOfWeek(Calendar.SUNDAY)
                .setMinimumDate(CalendarDay.from(2020, 12-1, 1))
                .setMaximumDate(CalendarDay.from(2020, 12-1, 31))
                .setCalendarDisplayMode(CalendarMode.MONTHS)
                .commit();

        cal.addDecorators(
                new SundayDecorator(),
                new SaturdayDecorator(),
                new OneDayDecorator());

        cal.setOnDateChangedListener(new OnDateSelectedListener() {
            @Override
            public void onDateSelected(@NonNull MaterialCalendarView widget, @NonNull CalendarDay date, boolean selected) {
                SimpleDateFormat format = new SimpleDateFormat("yyyy년 MMM dd일 [E]", Locale.KOREA);
                Intent intent = new Intent(getApplicationContext(),MainActivity.class);
                intent.putExtra("date",format.format(date.getDate()));
                int day = Integer.parseInt(format.format(date.getDate()).substring(10,12));
                intent.putExtra("day",day);

                ArrayList<TodoList> dayList = new ArrayList<TodoList>();

                for(int i = 0; i < monthOfList[day].length; i++){
                    if(monthOfList[day][i] != null){
                        dayList.add(monthOfList[day][i]);
                    }
                }

                if(dayList.size() > 0){
                    intent.putExtra("todo",dayList);
                    System.out.println("dayList!!!!"+ dayList);
                    ((MainActivity) MainActivity.mainContext).getInitList(dayList);
                }
                startActivity(intent);
            }
        });
    }

    public void onStart(){
        super.onStart();
    }

    public void getTodoLists(ArrayList<TodoList> todoLists, int day){
        TodoList[] d = new TodoList[10];

        for(int i = 0 ; i < todoLists.size(); i++){
            d[i] = todoLists.get(i);
        }
        monthOfList[day] = d;
    }

    public ArrayList<TodoList> abc(int day){
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
}