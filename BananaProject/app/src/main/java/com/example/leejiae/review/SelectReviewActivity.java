package com.example.leejiae.review;

import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class SelectReviewActivity extends AppCompatActivity {
    TextView text;
    EditText inputName;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_select_review);
        final SQLiteDatabase ReadDB = this.openOrCreateDatabase("database.db", MODE_PRIVATE, null);
        final Intent intent=getIntent();
        final String id=intent.getStringExtra("ID");
        Button select;
        Button back;
        select=findViewById(R.id.selectButton);
        back=findViewById(R.id.backButton);
        select.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                nameSortShowList();
            }
        });

        back.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent=new Intent(getApplicationContext(),ReviewActivity.class);
                intent.putExtra("ID",id);
                startActivity(intent);
            }
        });
    }

    protected void nameSortShowList() {
        String result="";
        inputName=findViewById(R.id.serchname);
        text=findViewById(R.id.selectshow);
        text.setText("");
        SQLiteDatabase ReadDB=this.openOrCreateDatabase("database.db",MODE_PRIVATE,null);
        String n=inputName.getText().toString();
        if(check(n)==true) {
            String sql_="select * from TotalReview where name = '"+n+"';";
            Cursor c = ReadDB.rawQuery(sql_, null);
            if(c!=null) {
                c.moveToFirst();
                do {
                    result="";
                    for(int i=0;i<c.getColumnCount();i++) {
                        if(i==0) {
                            result+="          가게명: "+c.getString(i)+"\n";
                        }
                        else if(i==1) {
                            result+="          아이디: "+c.getString(i)+"\n";
                        }
                        else if(i==2) {
                            result+="          총점: "+c.getString(i)+"\n";
                        }
                        else if(i==3) {
                            result+="          맵기: "+c.getString(i)+"\n";
                        }
                        else {
                            result+="          단짠: "+c.getString(i)+"\n";
                        }
                    }
                    result+="\n";
                    text.setText(text.getText()+result);
                }while (c.moveToNext());
            }
            ReadDB.close();
        }
        else {
            Toast.makeText(getApplication(),"입력하신 가게명과 일치하는 리뷰가 없습니다.",Toast.LENGTH_SHORT).show();
        }
    }

    public boolean check(String storeName) {
        SQLiteDatabase db = this.openOrCreateDatabase("database.db",MODE_PRIVATE,null);
        Cursor cursor = db.rawQuery("SELECT * FROM sqlite_master where type='table' and name='TotalReview';", null);
        cursor.moveToFirst();
        if(cursor.getCount()==0) {
            return false;
        }
        else {
            cursor = db.rawQuery("SELECT * FROM TotalReview;", null);
            cursor.moveToFirst();
            do {
                String cName = cursor.getString(0);
                if (storeName.equals(cName)) {
                    cursor.close();
                    return true;
                }
            }while (cursor.moveToNext());
            cursor.close();
            return false;
        }
    }
}
