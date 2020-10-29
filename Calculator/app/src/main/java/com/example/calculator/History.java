package com.example.calculator;

import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.annotation.NonNull;

import java.util.Objects;
import java.util.Stack;


class History extends Dialog {
    Context context;
    int[] layoutArray = {R.id.hl1,R.id.hl2,R.id.hl3,R.id.hl4,R.id.hl5};
    int[] buttonArray = {R.id.hb1,R.id.hb2,R.id.hb3,R.id.hb4,R.id.hb5};
    Stack<Double> history;
    LinearLayout[] linearLayouts;
    Button[] buttons;
    ImageButton removeAllHistoryButton;
    int memorySize = MainActivity.MEMORY_SIZE;

    public History(@NonNull Context context) {
        super(context);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        WindowManager.LayoutParams layoutParams = new WindowManager.LayoutParams();
        layoutParams.flags = WindowManager.LayoutParams.FLAG_DIM_BEHIND;
        layoutParams.dimAmount = 0.8f;
        Objects.requireNonNull(getWindow()).setAttributes(layoutParams);
        setContentView(R.layout.custom_history);

        this.history = MainActivity.history;
        layoutInit();
        viewUpdate();
    }

    View.OnClickListener removeAllHistory = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            for(int i = 0; i < history.size(); i++){
                buttons[i].setText("");
                linearLayouts[i].setVisibility(View.INVISIBLE);
            }
            MainActivity.history.removeAllElements();
        }
    };

    View.OnClickListener changeResultValue = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            int index = getLayoutIndex(view.getId());

            ((MainActivity) MainActivity.mContext).zero(buttons[index].getText().toString());
        }
    };

    public void layoutInit(){
        linearLayouts = new LinearLayout[memorySize];
        buttons = new Button[memorySize];

        for(int i = 0; i < memorySize; i++) {
            linearLayouts[i] = findViewById(layoutArray[i]);
            buttons[i] = findViewById(buttonArray[i]);

            buttons[i].setOnClickListener(changeResultValue);
        }
        removeAllHistoryButton = findViewById(R.id.delete);
        removeAllHistoryButton.setOnClickListener(removeAllHistory);
    }

    public void viewUpdate(){
        for(int i = history.size(); i < MainActivity.MEMORY_SIZE; i++){
            linearLayouts[i].setVisibility(View.INVISIBLE);
        }

        for(int i = 0; i < history.size(); i++){
            buttons[i].setText(MainActivity.roundNum(history.get(i)));
        }
    }

    public int getLayoutIndex(int id){
        for(int i = 0 ; i < history.size(); i++){
            if (id == buttonArray[i]){
                return i;
            }
        }
        return -1;
    }


}
