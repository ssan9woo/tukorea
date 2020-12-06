package com.example.todolist;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import android.graphics.Color;
import android.graphics.Paint;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;
import java.util.ArrayList;

public class CustomAdapter extends RecyclerView.Adapter<CustomAdapter.CustomViewHolder> {

    private ArrayList<TodoList> mList;

    public static class CustomViewHolder extends RecyclerView.ViewHolder {
        protected CheckBox isFinished;
        protected TextView todo;
        protected ImageButton delete;

        public CustomViewHolder(View view) {
            super(view);
            this.isFinished = (CheckBox) view.findViewById(R.id.finish);
            this.todo = (TextView) view.findViewById(R.id.todo);
            this.delete = (ImageButton) view.findViewById(R.id.delTodo);
        }
    }

    public CustomAdapter(ArrayList<TodoList> list) {
        this.mList = list;
    }

    @Override
    public CustomViewHolder onCreateViewHolder(ViewGroup viewGroup, int viewType) {

        View view = LayoutInflater.from(viewGroup.getContext())
                .inflate(R.layout.todo_list, viewGroup, false);

        CustomViewHolder viewHolder = new CustomViewHolder(view);

        return viewHolder;
    }

    @Override
    public void onBindViewHolder(@NonNull final CustomViewHolder viewholder, final int position) {
        viewholder.delete.setBackgroundColor(0xFFFFFFFF);
        viewholder.isFinished.setChecked(mList.get(position).getFinished());
        viewholder.todo.setText(mList.get(position).getTodo());
        viewholder.delete.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //delete Main에 있는 todoLists Controll해야함.
                ((MainActivity) MainActivity.mainContext).deleteTodo(position);
            }
        });

        viewholder.isFinished.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(viewholder.isFinished.isChecked()){
                    //밑으로
                    ((MainActivity) MainActivity.mainContext).setItemIndexToLast(MainActivity.todoLists.get(position));
                } else{
                    //위로
                    ((MainActivity) MainActivity.mainContext).setItemIndexToFirst(MainActivity.todoLists.get(position));
                }
            }
        });

        viewholder.isFinished.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if(viewholder.isFinished.isChecked()){
                    viewholder.todo.setPaintFlags(viewholder.todo.getPaintFlags() | Paint.STRIKE_THRU_TEXT_FLAG);
                }else{
                    viewholder.todo.setPaintFlags(0);
                }
            }
        });
    }

    @Override
    public int getItemCount() {
        return (null != mList ? mList.size() : 0);
    }
}