package com.example.leejiae.review;

import android.content.Intent;
import android.graphics.Typeface;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
/**
 * Created by Lee Jiae on 2018-11-22.
 */

public class MainActivity extends AppCompatActivity {
    //가게명 및 url은 지용햄이 넘겨줘야함!!! 현재 임의의 값
    String [] Store=new String[]{"a","b","c","d","e","f","g"};
    String storeURL="http";
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Intent intent = getIntent();
        final String id = intent.getStringExtra("ID");

        final Button login;
        final Button change;
        final Button delete;
        final Button review;
        final TextView text;
        final TextView text2;
        login = findViewById(R.id.login_b);
        change = findViewById(R.id.change_b);
        delete = findViewById(R.id.delete_b);
        review = findViewById(R.id.review_b);
        text=findViewById(R.id.t1);
        text2=findViewById(R.id.text_font2);

        final Button logout=(Button)findViewById(R.id.logout_b);
        final Button basket=(Button)findViewById(R.id.mybasket_b);
        final ImageView msg=(ImageView)findViewById(R.id.toastMessage);
        final Button zzim=(Button)findViewById(R.id.myzzim);

        Typeface type = Typeface.createFromAsset(getAssets(),"font/Binggrae-Bold.ttf");
        text.setTypeface(type);
        text2.setTypeface(type);

        if(id!=null){
            login.setVisibility(View.GONE);
            change.setVisibility(View.VISIBLE);
            delete.setVisibility(View.VISIBLE);
            review.setVisibility(View.VISIBLE);
            logout.setVisibility(View.VISIBLE);
            basket.setVisibility(View.VISIBLE);
        }

        msg.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Toast.makeText(getApplication(), "왼쪽 끝에서 슬라이드 해주세요.", Toast.LENGTH_SHORT).show();
            }
        });

        login.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent intent = new Intent(getApplicationContext(), LoginActivity.class);
                startActivity(intent);
            }
        });

        change.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent intent = new Intent(getApplicationContext(), memberUpdateActivity.class);
                intent.putExtra("ID", id);
                startActivity(intent);
            }
        });

        delete.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                DBHelper dbHelper = new DBHelper(getApplicationContext(), "member.db", null, 1);
                dbHelper.delete(id);
                Toast.makeText(getApplication(), id + "님 탈퇴되셨습니다.", Toast.LENGTH_SHORT).show();
                Intent intent = new Intent(getApplicationContext(), MainActivity.class);
                startActivity(intent);
            }
        });

        //찜버튼은 지용햄이 구현해야하지만 일단 임의로 구현해줬음
        //매번 i값이 0으로 초기화되기 때문에 같은 가게명이 만들어진다.
        //현재는 db에 배열값을 넘겨주지만 실제로는 크롤링한 가게명이 들어가게 됨.
        //즉, 나중에 바뀌어야한다.
        zzim.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(getApplicationContext(),SearchActivity.class);
                intent.putExtra("ID",id);
                startActivity(intent);
            }
        });

        logout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this, MainActivity.class);
                intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP|Intent.FLAG_ACTIVITY_SINGLE_TOP);
                login.setVisibility(View.VISIBLE);
                change.setVisibility(View.GONE);
                delete.setVisibility(View.GONE);
                review.setVisibility(View.GONE);
                logout.setVisibility(View.GONE);
                basket.setVisibility(View.GONE);
                Toast.makeText(getApplication(), "로그아웃 되었습니다.", Toast.LENGTH_SHORT).show();
                startActivity(intent);
            }
        });

        basket.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(getApplicationContext(), myBasketActivity.class);
                intent.putExtra("ID", id);
                startActivity(intent);
            }
        });

        review.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent intent = new Intent(getApplicationContext(), ReviewActivity.class);
                intent.putExtra("ID", id);
                startActivity(intent);
            }
        });
    }
}
