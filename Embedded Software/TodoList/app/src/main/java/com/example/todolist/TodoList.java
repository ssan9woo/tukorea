package com.example.todolist;

public class TodoList {
    boolean isFinished = false;
    String todo = null;


    public TodoList(boolean isFinished, String todo){
        this.isFinished = isFinished;
        this.todo = todo;
    }

    public void setFinished(boolean isFinished){
        this.isFinished = isFinished;
    }

    public void setTodo(String todo){
        this.todo = todo;
    }

    public boolean getFinished(){
        return this.isFinished;
    }

    public String getTodo(){
        return this.todo;
    }

}
