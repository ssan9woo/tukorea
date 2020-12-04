package com.example.calendartest;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.prolificinteractive.materialcalendarview.CalendarDay;
import com.prolificinteractive.materialcalendarview.CalendarMode;
import com.prolificinteractive.materialcalendarview.MaterialCalendarView;
import com.prolificinteractive.materialcalendarview.OnDateSelectedListener;

import java.util.Calendar;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        MaterialCalendarView cal = findViewById(R.id.calendarView);
        cal.state().edit()
                .setFirstDayOfWeek(Calendar.SUNDAY)
                .setMinimumDate(CalendarDay.from(2020, 12-1, 1))
                .setMaximumDate(CalendarDay.from(2020, 12-1, 31))
                .setCalendarDisplayMode(CalendarMode.MONTHS)
                .commit();


        cal.setOnDateChangedListener(new OnDateSelectedListener() {
            @Override
            public void onDateSelected(@NonNull MaterialCalendarView widget, @NonNull CalendarDay date, boolean selected) {
                //System.out.println(date.getDate());
                Intent intent = new Intent(MainActivity.this,MainActivity2.class);
                intent.putExtra("key",date.getDate().toString());
                startActivity(intent);
            }
        });
    }
    public native String stringFromJNI();
}
