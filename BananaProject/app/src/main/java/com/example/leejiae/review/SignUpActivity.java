package com.example.leejiae.review;

import android.content.Intent;
import android.graphics.Typeface;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class SignUpActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sign_up);

        final DBHelper dbHelper=new DBHelper(getApplicationContext(),"member.db",null,1);
        final EditText textID=(EditText)findViewById(R.id.text_ID);
        final EditText textPW=(EditText)findViewById(R.id.text_PW);
        final EditText textSF=(EditText)findViewById(R.id.scoreF);
        final EditText textSS=(EditText)findViewById(R.id.scoreS);

        Button btnAccept =(Button)findViewById(R.id.btn_Accept);
        Button btnCancel=(Button)findViewById(R.id.btn_Cancel);

        TextView t_1=(TextView)findViewById(R.id.t1);
        TextView t_4=(TextView)findViewById(R.id.t4);
        Typeface type = Typeface.createFromAsset(getAssets(),"font/Binggrae-Bold.ttf");
        t_1.setTypeface(type);
        t_4.setTypeface(type);

        View.OnClickListener listener=new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent goLog=new Intent(getApplicationContext(),LoginActivity.class);
                switch (view.getId()){
                    case R.id.btn_Accept:
                        String ID=textID.getText().toString();
                        String PW=textPW.getText().toString();
                        String sf=textSF.getText().toString();
                        String ss=textSS.getText().toString();

                        if(ID.getBytes().length<=0){
                            Toast.makeText(SignUpActivity.this, "ID를 적어주세요.", Toast.LENGTH_SHORT).show();
                        }
                        else if(PW.getBytes().length<=0){
                            Toast.makeText(SignUpActivity.this, "PW를 적어주세요.", Toast.LENGTH_SHORT).show();
                        }
                        else if(sf.getBytes().length<=0){
                            Toast.makeText(SignUpActivity.this, "단짠 정도를 적어주세요.", Toast.LENGTH_SHORT).show();
                        }
                        else if(ss.getBytes().length<=0){
                            Toast.makeText(SignUpActivity.this, "매운맛 정도를 적어주세요.", Toast.LENGTH_SHORT).show();
                        }
                        else{
                            int SF=Integer.parseInt(sf);
                            int SS=Integer.parseInt(ss);
                            if(SF>5){
                                Toast.makeText(SignUpActivity.this, "단짠 정도는 0~5사이 숫자를 입력해주세요.", Toast.LENGTH_SHORT).show();
                            }
                            if(SS>5){
                                Toast.makeText(SignUpActivity.this, "매운맛 정도는 0~5사이 숫자를 입력해주세요.", Toast.LENGTH_SHORT).show();
                            }
                            if(SF<=5&&SS<=5){
                                if(dbHelper.select(ID)){
                                    Toast.makeText(getApplication(), "이미 가입된 아이디 입니다."+"\n"+" 다른 아이디를 적어주세요.", Toast.LENGTH_SHORT).show();
                                }
                                else{
                                    dbHelper.insert(ID,PW,SF,SS);
                                    dbHelper.createBasket(ID);
                                    Toast.makeText(getApplication(), ID+"님 가입되셨습니다.", Toast.LENGTH_SHORT).show();
                                    startActivity(goLog);
                                }
                            }
                        }
                        break;
                    case R.id.btn_Cancel:
                        //취소했으므로 로그인 페이지로 이동
                        startActivity(goLog);
                        break;
                }
            }
        };
        btnAccept.setOnClickListener(listener);
        btnCancel.setOnClickListener(listener);
    }
}