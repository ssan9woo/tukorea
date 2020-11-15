package com.example.todolist;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.CheckBox;
import android.widget.ImageView;
import android.widget.TextView;
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

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        ViewHolder holder;
        if (convertView == null) {
            holder = new ViewHolder();

            LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            convertView = inflater.inflate(R.layout.todo_list, null);

            holder.isFinished = (CheckBox) convertView.findViewById(R.id.finish);
            holder.todo = (TextView) convertView.findViewById(R.id.todo);
            holder.isImportant = (CheckBox) convertView.findViewById(R.id.important);

            convertView.setTag(holder);
        }else{
            holder = (ViewHolder) convertView.getTag();
        }

        TodoList to = todoLists.get(position);

        holder.isFinished.setChecked(to.isFinished);
        holder.todo.setText(to.todo);
        holder.isImportant.setChecked(to.isImportant);
        return convertView;
    }
}
