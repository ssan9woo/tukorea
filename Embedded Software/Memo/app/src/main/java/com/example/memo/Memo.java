package com.example.memo;

public class Memo {
    private String title;
    private String contents;

    public Memo(String title, String contents){
        this.title = title;
        this.contents = contents;
    }
    public String getTitle(){
        return title;
    }

    public String getContents(){
        return contents;
    }

    public void setTitle(String title){
        this.title = title;
    }

    public void setContents(String contents){
        this.contents = contents;
    }
}
