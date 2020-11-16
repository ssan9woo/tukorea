package com.example.todolist;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Paint;
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

    public void addItem(boolean isFinished, String s, boolean isImportant){
        TodoList todo = new TodoList();
        todo.setFinished(isFinished);
        todo.setTodo(s);
        todo.setImportant(isImportant);

        todoLists.add(todo);
        dataChange();
    }

    public void addItem_top(boolean isFinished, String s, boolean isImportant){
        TodoList todo = new TodoList();
        todo.setFinished(isFinished);
        todo.setTodo(s);
        todo.setImportant(isImportant);

        todoLists.add(0,todo);
        dataChange();
    }

    public void resetItem(int position,boolean isFinished, String s, boolean isImportant){
        TodoList t = new TodoList();
        t.isImportant = isImportant;
        t.todo = s;
        t.isFinished = isFinished;

        todoLists.set(position,t);
        dataChange();
    }

    public void remove(int position){
        todoLists.remove(position);
        dataChange();
    }

    public void removeAll(){
        todoLists.removeAll(todoLists);
        dataChange();
    }

    public void dataChange(){
        MainActivity.adapter.notifyDataSetChanged();
    }

    @SuppressLint("InflateParams")
    @Override
    public View getView(final int position, View convertView, ViewGroup parent) {
        final ViewHolder holder;
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
                    if(holder.isFinished.isChecked()){
                        holder.todo.setPaintFlags(holder.todo.getPaintFlags() | Paint.STRIKE_THRU_TEXT_FLAG);
                    }else{
                        holder.todo.setPaintFlags(0);
                    }
                }
            });
            holder.isFinished.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    System.out.println(holder.isFinished.isChecked());
                    if(holder.isFinished.isChecked()) {
                        //맨 밑으로 내려보냄
                        remove(position);
                        addItem(holder.isFinished.isChecked(), holder.todo.getText().toString(), holder.isImportant.isChecked());
                    }else{
                        //맨 위로 올려보냄
                        remove(position);
                        addItem_top(holder.isFinished.isChecked(), holder.todo.getText().toString(), holder.isImportant.isChecked());
                    }
                }
            });

            holder.isImportant.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    if (!holder.isFinished.isChecked()) {
                        if (holder.isImportant.isChecked()) {
                            remove(position);
                            addItem_top(holder.isFinished.isChecked(), holder.todo.getText().toString(), holder.isImportant.isChecked());
                        } else {
                            resetItem(position, holder.isFinished.isChecked(), holder.todo.getText().toString(), holder.isImportant.isChecked());
                        }
                    }
                }
            });
            convertView.setTag(holder);
        }else{
            holder = (ViewHolder) convertView.getTag();
        }
        TodoList to = todoLists.get(position);
        holder.todo.setText(to.todo);
        holder.isFinished.setChecked(to.isFinished);
        holder.isImportant.setChecked(to.isImportant);
        return convertView;
    }
}
