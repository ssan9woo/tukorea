package com.example.todolist;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.drawable.Drawable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;

public class ListViewAdapter extends BaseAdapter {
    private Context mContext = null;
    private ArrayList<TodoList> todoLists = new ArrayList<TodoList>();

    public ListViewAdapter(Context mContext) {
        super();
        this.mContext = mContext;
    }

    @Override
    public int getCount() {
        return todoLists.size();
    }

    @Override
    public Object getItem(int position) {
        return todoLists.get(position);
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    public void addItem(String s){
        TodoList todo = new TodoList();
        todo.setFinished(false);
        todo.setTodo(s);
        todo.setImportant(false);

        todoLists.add(todo);
        dataChange();
    }

    public void remove(int position){
        todoLists.remove(position);
        dataChange();
    }


    public void dataChange(){
        MainActivity.adapter.notifyDataSetChanged();
    }

    @SuppressLint("InflateParams")
    @Override
    public View getView(final int position, View convertView, ViewGroup parent) {
        ViewHolder holder;
        if (convertView == null) {
            holder = new ViewHolder();

            LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            convertView = inflater.inflate(R.layout.todo_list, null);

            holder.isFinished = (CheckBox) convertView.findViewById(R.id.finish);
            holder.todo = (TextView) convertView.findViewById(R.id.todo);
            holder.isImportant = (CheckBox) convertView.findViewById(R.id.important);

            holder.isFinished.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
                @Override
                public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                    //
                }
            });
            holder.isImportant.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
                @Override
                public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                    //
                }
            });

            convertView.setTag(holder);
        }else{
            holder = (ViewHolder) convertView.getTag();
        }
        TodoList to = todoLists.get(position);
        holder.todo.setText(to.todo);
        return convertView;
    }
}
