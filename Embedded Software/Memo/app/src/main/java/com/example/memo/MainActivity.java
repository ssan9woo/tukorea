package com.example.memo;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.DividerItemDecoration;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteException;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import java.sql.SQLInput;
import java.util.ArrayList;
import java.util.HashMap;

public class MainActivity extends AppCompatActivity {
    public static Context mainContext;
    private ArrayList<Memo> MemoList;
    private CustomAdapter mAdapter;
    SQLiteDatabase db = null;
    private final String dbName = "db";
    private final String tableName = "memo";
    Button addMemo;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mainContext = this;

        RecyclerView mRecyclerView = (RecyclerView) findViewById(R.id.recyclerview_main_list);
        LinearLayoutManager mLinearLayoutManager = new LinearLayoutManager(this);
        mRecyclerView.setLayoutManager(mLinearLayoutManager);

        MemoList = new ArrayList<>();
        mAdapter = new CustomAdapter(MemoList);
        mRecyclerView.setAdapter(mAdapter);

        DividerItemDecoration dividerItemDecoration = new DividerItemDecoration(mRecyclerView.getContext(),
                mLinearLayoutManager.getOrientation());
        mRecyclerView.addItemDecoration(dividerItemDecoration);

        addMemo = findViewById(R.id.addMemo);
        addMemo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(getApplicationContext(),InputMemoActivity.class);
                startActivityForResult(intent,1234);
            }
        });
        //db
        try{
            db = this.openOrCreateDatabase(dbName,MODE_PRIVATE,null);
            db.execSQL("CREATE TABLE IF NOT EXISTS " + tableName
                    + " (title VARCHAR(20), contents VARCHAR(100) );");
            db.execSQL("DELETE FROM " + tableName  );
            for(int i = 0; i < MemoList.size(); i++){
                db.execSQL("INSERT INTO " + tableName
                        + " (title, contents)  Values ('" + MemoList.get(i).getTitle() + "', '" + MemoList.get(i).getContents()+"');");
                db.close();
            }
        }catch (SQLiteException e){
            e.printStackTrace();
        }
        showList();
    }

    public void showList(){
        SQLiteDatabase db = this.openOrCreateDatabase(dbName,MODE_PRIVATE,null);
        Cursor c = db.rawQuery("SELECT * FROM " + tableName, null);
        if (c != null) {
            if (c.moveToFirst()) {
                do {
                    System.out.println("불러오기 ");
                    //테이블에서 두개의 컬럼값을 가져와서
                    String title = c.getString(c.getColumnIndex("title"));
                    String contents = c.getString(c.getColumnIndex("contents"));
                    Memo memo = new Memo(title, contents);

                    MemoList.add(memo);
                    mAdapter.notifyDataSetChanged();
                } while (c.moveToNext());
            }
        }

        db.close();
    }
    public void deleteMemo(int index){
        MemoList.remove(index);
        mAdapter.notifyDataSetChanged();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if(requestCode == 1234 && resultCode == RESULT_OK){
            assert data != null;
            String title = data.getStringExtra("title");
            String contents = data.getStringExtra("contents");
            db.execSQL("INSERT INTO " + tableName
                    + " (title, contents)  Values ('" + title + "', '" + contents+"');");
            db.close();
            System.out.println("저장");
            Memo m = new Memo(title,contents);
            MemoList.add(m);
            mAdapter.notifyDataSetChanged();
        }
    }
}