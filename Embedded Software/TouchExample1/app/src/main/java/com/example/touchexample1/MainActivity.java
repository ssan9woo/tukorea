package com.example.touchexample1;

import androidx.fragment.app.FragmentActivity;

import android.animation.ObjectAnimator;
import android.annotation.SuppressLint;
import android.content.Context;
import android.os.Bundle;
import android.os.Vibrator;
import android.view.Display;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;

public class MainActivity extends FragmentActivity {
    int previous_x, previous_y, width, height, img_height, img_width, x, y;
    ImageView star;
    Vibrator mVibe;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        setContentView(R.layout.activity_main);
        star = findViewById(R.id.star);
        mVibe = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);

        Display display = getWindowManager().getDefaultDisplay();
        width = display.getWidth();
        height = display.getHeight();

        star.measure(star.getMeasuredWidth(),star.getMeasuredHeight());
        img_height  = star.getMeasuredHeight();
        img_width = star.getMeasuredWidth();

        x = width / 2 - img_width / 2;
        y = height / 2 - img_height / 2;

        star.setX(x);
        star.setY(y);
        previous_x = x;
        previous_y = y;

    }
    public boolean onTouchEvent(MotionEvent event){

        switch (event.getAction()){
            case MotionEvent.ACTION_MOVE:
                int touch_x = (int)event.getX();
                int touch_y = (int)event.getY();

                x = touch_x - img_width / 2;
                y = touch_y - img_height / 2;

                @SuppressLint("ObjectAnimatorBinding")
                ObjectAnimator smileX = ObjectAnimator.ofFloat(star,"translation",previous_x,x);
                smileX.start();
                @SuppressLint("ObjectAnimatorBinding")
                ObjectAnimator smileY = ObjectAnimator.ofFloat(star,"translation",previous_y,y);
                smileY.start();

                mVibe.vibrate(50);

                previous_x = x;
                previous_y = y;

                System.out.println(x + " "  + y);
                break;
            case MotionEvent.ACTION_DOWN:
                break;
            case MotionEvent.ACTION_UP:
                break;

        }
        return false;
    }
}