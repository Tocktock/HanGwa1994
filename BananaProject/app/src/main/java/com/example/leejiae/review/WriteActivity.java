package com.example.leejiae.review;

import android.content.Intent;
import android.database.sqlite.SQLiteDatabase;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.RatingBar;
import android.widget.TextView;
import android.widget.Toast;

public class WriteActivity extends AppCompatActivity {
    static float value;
    static float spValue;
    static float swValue;
    RatingBar to;
    RatingBar sp;
    RatingBar sw;
    TextView t;
    String id;
    String storeName;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Intent intent=getIntent();
        id=intent.getStringExtra("ID");
        storeName = intent.getStringExtra("STORE_NAME");
        setContentView(R.layout.activity_write);
        to=findViewById(R.id.ratingBar);
        t=findViewById(R.id.total);
        sp=findViewById(R.id.ratingBar2);
        sw=findViewById(R.id.ratingBar3);
        final TextView t=findViewById(R.id.total);
        SQLiteDatabase sampleDB=null;
        sampleDB= this.openOrCreateDatabase("database.db",MODE_PRIVATE,null);
        to.setOnRatingBarChangeListener(new RatingBar.OnRatingBarChangeListener(){
            public void onRatingChanged(RatingBar ratingBar, float rating, boolean fromUser){
                if(ratingBar.getRating()<0.5f) {
                    ratingBar.setRating(0.5f);
                }
                t.setText("총점: " + rating);
                value=rating;
            }
        });

        sp.setOnRatingBarChangeListener(new RatingBar.OnRatingBarChangeListener(){
            public void onRatingChanged(RatingBar ratingBar, float rating, boolean fromUser){
                if(ratingBar.getRating()<0.5f) {
                    ratingBar.setRating(0.5f);
                }
                spValue=rating;
            }
        });

        sw.setOnRatingBarChangeListener(new RatingBar.OnRatingBarChangeListener(){
            public void onRatingChanged(RatingBar ratingBar, float rating, boolean fromUser){
                if(ratingBar.getRating()<0.5f) {
                    ratingBar.setRating(0.5f);
                }
                swValue=rating;
            }
        });

        Button button = findViewById(R.id.button);
        Button star=findViewById(R.id.star_button);
        button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent intent=new Intent(getApplicationContext(),ReviewActivity.class);
                intent.putExtra("ID",id);
                intent.putExtra("STORE_NAME",storeName);
                startActivity(intent);
            }
        });

        final SQLiteDatabase finalSampleDB = sampleDB;
        star.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finalSampleDB.execSQL("create table if not exists TotalReview (name TEXT not null, id TEXT not null, score FLOAT, spicy FLOAT, sweet FLOAT);");
                finalSampleDB.execSQL("insert into TotalReview (name,id,score,spicy,sweet) Values ('"+storeName+"','"+id+"','"+value+"','"+spValue+"','"+swValue+"');'");
                finalSampleDB.close();
                Toast.makeText(getApplication(),"리뷰 작성이 완료되었습니다.",Toast.LENGTH_SHORT).show();
                Intent intent=new Intent(getApplicationContext(),ReviewActivity.class);
                intent.putExtra("ID",id);
                intent.putExtra("STORE_NAME",storeName);
                startActivity(intent);
            }
        });
    }

    /**
     * Created by Lee Jiae on 2018-11-23.
     */

}
