package com.example.calculator;

import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import java.util.Arrays;
import java.util.Objects;
import java.util.Stack;

class CustomDialog extends Dialog {
    int[][] layoutArray = {{R.id.l1, R.id.t1},{R.id.l2, R.id.t2},{R.id.l3, R.id.t3},{R.id.l4, R.id.t4},{R.id.l5, R.id.t5}};
    int[][] buttonArray = {{R.id.b11, R.id.b12, R.id.b13}, {R.id.b21, R.id.b22, R.id.b23}, {R.id.b31, R.id.b32, R.id.b33}, {R.id.b41, R.id.b42, R.id.b43}, {R.id.b51, R.id.b52, R.id.b53}};
    final int CLEAR = 0;
    final int ADD = 1;
    final int SUB = 2;
    Stack<Double> memoryValue;

    Context context;
    LinearLayout[] linearLayouts;
    TextView[] textViews;
    Calculation calculation;

    public CustomDialog(@NonNull Context context) {
        super(context);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        WindowManager.LayoutParams layoutParams = new WindowManager.LayoutParams();
        layoutParams.flags = WindowManager.LayoutParams.FLAG_DIM_BEHIND;
        layoutParams.dimAmount = 0.8f;
        Objects.requireNonNull(getWindow()).setAttributes(layoutParams);
        setContentView(R.layout.custom);

        memoryValue = MainActivity.memoryValue;
        calculation = new Calculation();

        layoutInit();
        viewUpdate();
    }

    View.OnClickListener memoryIndexClear = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            System.out.println("clear");
            int index = getLayoutIndex(view.getId(),CLEAR);

            for(int i = index + 1; i < memoryValue.size(); i++){
                textViews[i-1].setText(textViews[i].getText().toString());
            }

            textViews[memoryValue.size()-1].setText("");
            linearLayouts[memoryValue.size()-1].setVisibility(View.INVISIBLE);
            memoryValue.remove(index);
        }
    };

    View.OnClickListener memoryIndexAdd = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            memoryAddOrSub(view.getId(), ADD);
        }
    };

    View.OnClickListener memoryIndexSub = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            memoryAddOrSub(view.getId(), SUB);
        }
    };

    public void viewUpdate(){
        for(int i = memoryValue.size(); i < MainActivity.MEMORY_SIZE; i++){
            linearLayouts[i].setVisibility(View.INVISIBLE);
        }
        for(int i = 0; i < memoryValue.size(); i++){
            textViews[i].setText(MainActivity.roundNum(memoryValue.get(i)));
            for(int j = 0 ; j < 3; j++){
                Button b = findViewById(buttonArray[i][j]);
                switch (j){
                    case 0:
                        b.setOnClickListener(memoryIndexClear);
                        break;
                    case 1:
                        b.setOnClickListener(memoryIndexAdd);
                        break;
                    case 2:
                        b.setOnClickListener(memoryIndexSub);
                        break;
                }
            }
        }
    }

    public void layoutInit(){
        linearLayouts = new LinearLayout[MainActivity.MEMORY_SIZE];
        textViews = new TextView[MainActivity.MEMORY_SIZE];

        for(int i = 0; i < MainActivity.MEMORY_SIZE; i++){
            linearLayouts[i] = findViewById(layoutArray[i][0]);
            textViews[i] = findViewById(layoutArray[i][1]);
        }
    }

    public int getLayoutIndex(int id, int operator){
        for(int i = 0 ; i < memoryValue.size(); i++){
            if (id == buttonArray[i][operator]){
                return i;
            }
        }
        return -1;
    }

    public void memoryAddOrSub(int id, int operator){
        int index = getLayoutIndex(id,operator);
        String resultString = ((MainActivity) MainActivity.mContext).getCurrentResultValue();

        double resultNum = Double.parseDouble(resultString);
        double currentNum = memoryValue.get(index);

        if(operator == ADD)
            currentNum = calculation.add(currentNum, resultNum);
        else
            currentNum = calculation.sub(currentNum, resultNum);

        memoryValue.set(index,currentNum);
        textViews[index].setText(MainActivity.roundNum(currentNum));
    }

}
