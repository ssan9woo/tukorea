package com.example.todolist;

public class TodoList {
    boolean isFinished = false;
    String todo = null;
    boolean isImportant = false;

    public TodoList(boolean isFinished, String todo, boolean isImportant){
        this.isFinished = isFinished;
        this.todo = todo;
        this.isImportant = isImportant;
    }
}
