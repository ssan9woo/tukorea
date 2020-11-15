package com.example.todolist;

public class TodoList {
    boolean isFinished = false;
    String todo = null;
    boolean isImportant = false;

    public void setFinished(boolean isFinished){
        this.isFinished = isFinished;
    }

    public void setTodo(String todo){
        this.todo = todo;
    }

    public void setImportant(boolean isImportant){
        this.isImportant = isImportant;
    }

    public boolean getFinished(){
        return this.isFinished;
    }

    public String getTodo(){
        return this.todo;
    }

    public boolean getImportant(){
        return this.isImportant;
    }

}
