package com.example.todolist;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    CustomAdapter adapter = null;
    Button createTodo;
    ArrayList<TodoList> todoLists = new ArrayList<TodoList>();
    EditText input;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        createTodo = findViewById(R.id.createTodo);
        input = findViewById(R.id.input);


        //Adapter
        adapter = new CustomAdapter(this,R.layout.todo_list, todoLists);
        ListView listView = (ListView) findViewById(R.id.list);
        listView.setAdapter(adapter);


        adapter.notifyDataSetChanged();
    }
    public native String stringFromJNI();

    static {
        System.loadLibrary("native-lib");
    }


}



class CustomAdapter extends ArrayAdapter<TodoList> {
    private ArrayList<TodoList> todoList;

    public CustomAdapter(Context context, int textViewResourceId, ArrayList<TodoList> todoList){
        super(context, textViewResourceId, todoList);
        this.todoList = new ArrayList<TodoList>();
        this.todoList.addAll(todoList);
    }

    private static class ViewHolder{
        CheckBox finish, important;
        TextView todo;
    }

    public View getView(int position, View convertView, ViewGroup parent){
        ViewHolder holder = null;

        if(convertView == null){
            LayoutInflater vi = (LayoutInflater) getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            convertView = vi.inflate(R.layout.todo_list,null);
            holder = new ViewHolder();
            holder.finish = (CheckBox) convertView.findViewById(R.id.finish);
            holder.todo = (TextView) convertView.findViewById(R.id.todo);
            holder.important = (CheckBox) convertView.findViewById(R.id.important);
            convertView.setTag(holder);

            //check box onclickListener
            holder.finish.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
                @Override
                public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                    System.out.println("투두 체크 버튼");
                }
            });

            holder.important.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
                @Override
                public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                    System.out.println("별 체크 버튼");
                }
            });
        }else{
            holder = (ViewHolder) convertView.getTag();
        }

        System.out.println(position);
        TodoList todo = todoList.get(position);
        holder.finish.setChecked(todo.isFinished);
        holder.todo.setText(todo.todo);
        holder.important.setChecked(todo.isImportant);

        return convertView;
    }

    public void addItem(boolean isFinished, String todo, boolean isImportant){
        TodoList list = new TodoList();

        list.setFinished(isFinished);
        list.setTodo(todo);
        list.setImportant(isImportant);

        todoList.add(list);
    }
}

