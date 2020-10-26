package com.example.calculator;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Context;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.TextView;

import java.util.Stack;

public class MainActivity extends AppCompatActivity {

    TextView result, memoryViewText;
    Stack<Integer> operationStack;
    double previousResult, currentResult;
    Calculation calculation;
    static Stack<Double> memoryValue;
    final static int MEMORY_SIZE = 5;
    public static Context mContext;
    boolean flag = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        result = findViewById(R.id.result);
        operationStack = new Stack<>();
        calculation = new Calculation();
        memoryValue = new Stack<>();
        mContext = this;
    }

    // 연산자 연산 수행
    public void calculateNum(View v){
        int op = v.getId();

        if(operationStack.isEmpty()){
            if(op != R.id.equalBtn){
                operationStack.add(op);
                previousResult = Double.parseDouble(result.getText().toString());
                //result.setText("0");
            }
        } else{
            int willCalculateOperation = operationStack.pop();
            if (op != R.id.equalBtn)
                operationStack.add(op);

            switch (willCalculateOperation){
                case R.id.addBtn:
                    currentResult = calculation.add(previousResult, Double.parseDouble(result.getText().toString()));
                    break;
                case R.id.subBtn:
                    currentResult = calculation.sub(previousResult, Double.parseDouble(result.getText().toString()));
                    break;
                case R.id.mulBtn:
                    currentResult = calculation.mul(previousResult, Double.parseDouble(result.getText().toString()));
                    break;
                case R.id.divBtn:
                    currentResult = calculation.div(previousResult, Double.parseDouble(result.getText().toString()));
                    break;
            }
            result.setText(roundNum(currentResult));
            previousResult = currentResult;
            currentResult = 0;
            flag = false;
        }
    }

    // inputNumber
    @SuppressLint("ResourceType")
    public void inputNumber(View v){
        String calculationValue = "";
        System.out.println(operationStack + " "  + flag);

        if (operationStack.size() > 0) {
            if(!flag){
                flag = true;
                calculationValue = "";
            }else{
                calculationValue = result.getText().toString();
            }
        }else{
            calculationValue = result.getText().toString();
            flag = false;
        }

        if (calculationValue.length() > 10)
            return;

        if(calculationValue.equals("0")) { calculationValue = ""; }

        Button num = findViewById(v.getId());
        calculationValue += num.getText();
        result.setText(calculationValue);
    }

    // C
    public void clearAll(View v){
        previousResult = 0;
        currentResult = 0;
        operationStack.removeAllElements();
        result.setText(String.valueOf(Math.round(0)));
    }

    // CE
    public void clearCurrentValue(View v){
        result.setText("0");
    }

    // ±
    public void convertToNegative(View v){
        if(!result.getText().toString().equals("0")){
            double num = Double.parseDouble(result.getText().toString()) * -1;
            result.setText(roundNum(num));
        }else{
            result.setText("0");
        }
    }

    // .
    @SuppressLint("SetTextI18n")
    public void appendPoint(View v){
        result.setText(result.getText() + ".");
    }

    // DEL
    public void deleteNum(View v){
        String num = result.getText().toString();
        if(num.length() > 1){
            result.setText(num.substring(0,num.length() - 1));
        }else{
            result.setText("0");
        }
    }

    // root
    public void root(View v){
        result.setText(String.valueOf(roundNum(Math.sqrt(Double.parseDouble(result.getText().toString())))));
    }

    // percentage
    public void percentage(View v){
        result.setText(String.valueOf(roundNum(Double.parseDouble(result.getText().toString()) / 100.0)));
    }

    // reciprocal
    public void reciprocal(View v){
        result.setText(String.valueOf(roundNum(1 / Double.parseDouble(result.getText().toString()))));
    }

    // power
    public void power(View v){
        result.setText(String.valueOf(roundNum(Math.pow(Double.parseDouble(result.getText().toString()),2))));
    }

    // Memory Clear
    public void clearMemory(View v){
        // 메모리에 있는 모든 Data clear
        memoryValue.removeAllElements();
    }

    // Memory Read
    public void readMemory(View v){
        // MemoryValue의 Top값 return
        result.setText(roundNum(memoryValue.get(0)));
    }

    // M+
    public void add_Memory(View v){
        memoryValue.set(0, memoryValue.get(0) + Double.parseDouble(result.getText().toString())) ;
    }

    // M-
    public void sub_Memory(View v){
        memoryValue.set(0,+ memoryValue.get(0) - Double.parseDouble(result.getText().toString())) ;
    }

    // Memory Set
    public void setMemory(View v){
        if (memoryValue.size() < MEMORY_SIZE)
            memoryValue.add(Double.parseDouble(result.getText().toString()));
    }

    // Memory View
    @SuppressLint("SetTextI18n")
    public void viewMemory(View v){
        CustomDialog customDialog = new CustomDialog(this);
        customDialog.show();
    }

    // Clear Memory View Text
    public void clearMemoryViewText(View v){
        memoryViewText.setText("");
    }

    public static String roundNum(double num){
        if(String.valueOf(num - (int)num).equals("0.0")){
            return String.valueOf(Math.round(num));
        }else{
            return String.valueOf(Math.round(num*1000)/1000.0);
        }
    }

    public String getCurrentResultValue(){
        return result.getText().toString();
    }
}




