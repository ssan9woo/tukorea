package com.example.calculator;

public class Calculation {
    public double add(double previousResult, double currentResult){
        return previousResult + currentResult;
    }
    public double sub(double previousResult, double currentResult){
        return previousResult - currentResult;
    }
    public double mul(double previousResult, double currentResult){
        return previousResult * currentResult;
    }
    public double div(double previousResult, double currentResult){
        return previousResult / currentResult;
    }
}

