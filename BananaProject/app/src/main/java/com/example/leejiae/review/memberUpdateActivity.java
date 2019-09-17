package com.example.leejiae.review;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class memberUpdateActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_member_update);

        final Intent intent = getIntent();
        final String ID = intent.getStringExtra("ID");
        final DBHelper dbHelper = new DBHelper(getApplicationContext(), "member.db", null, 1);
        final EditText chgPW = (EditText) findViewById(R.id.change_PW);
        final EditText chgSF = (EditText) findViewById(R.id.change_scoreF);
        final EditText chgSS = (EditText) findViewById(R.id.change_scoreS);

        Button btnChg = (Button) findViewById(R.id.btn_change);
        Button btnChgCancel = (Button) findViewById(R.id.btn_change_Cancel);

        View.OnClickListener listener = new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                switch (view.getId()) {
                    case R.id.btn_change:
                        String PW = chgPW.getText().toString();
                        String sf = chgSF.getText().toString();
                        String ss = chgSS.getText().toString();

                        if (PW.getBytes().length <= 0) {
                            Toast.makeText(memberUpdateActivity.this, "변경할 PW를 적어주세요.", Toast.LENGTH_SHORT).show();
                        }
                        else if (sf.getBytes().length <= 0) {
                            Toast.makeText(memberUpdateActivity.this, "단짠 정도를 적어주세요.", Toast.LENGTH_SHORT).show();
                        }
                        else if (ss.getBytes().length <= 0) {
                            Toast.makeText(memberUpdateActivity.this, "매운맛 정도를 적어주세요.", Toast.LENGTH_SHORT).show();
                        }
                        else {
                            int SF = Integer.parseInt(sf);
                            int SS = Integer.parseInt(ss);
                            if(SF>5){
                                Toast.makeText(memberUpdateActivity.this, "단짠 정도는 0~5사이 숫자를 입력해주세요.", Toast.LENGTH_SHORT).show();
                            }
                            if(SS>5){
                                Toast.makeText(memberUpdateActivity.this, "매운맛 정도는 0~5사이 숫자를 입력해주세요.", Toast.LENGTH_SHORT).show();
                            }
                            if(SF<=5&&SS<=5){
                                dbHelper.updatePW(ID,PW);
                                dbHelper.updateSF(ID,SF);
                                dbHelper.updateSS(ID,SS);
                                Toast.makeText(getApplication(), ID + "님 정보 변경되셨습니다. "+"\n"+"다시 로그인해주세요.", Toast.LENGTH_SHORT).show();
                                Intent goMain = new Intent(memberUpdateActivity.this, MainActivity.class);
                                startActivity(goMain);
                            }
                        }
                        break;
                    case R.id.btn_change_Cancel:
                        //취소했으므로 메인 페이지로 이동
                        Intent goMain = new Intent(getApplicationContext(), MainActivity.class);
                        goMain.putExtra("ID", ID);
                        startActivity(goMain);
                        break;
                }
            }
        };
        btnChg.setOnClickListener(listener);
        btnChgCancel.setOnClickListener(listener);
    }
}