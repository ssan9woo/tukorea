package com.example.hello_ndk;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.KeyEvent;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    BilliardBall ball;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        ball = new BilliardBall(this);
        setContentView(ball);
    }
    public boolean onKeyDown(int keyCode, KeyEvent event){
        if(keyCode == KeyEvent.KEYCODE_VOLUME_UP){
            BilliardBall.dx += 10;
            BilliardBall.dy += 10;
        }
        if(keyCode == KeyEvent.KEYCODE_VOLUME_DOWN){
            if(BilliardBall.dx >= 0 && BilliardBall.dy >= 0){
                BilliardBall.dx -= 10;
                BilliardBall.dy -= 10;
            }

        }
        return true;
    }


    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}
