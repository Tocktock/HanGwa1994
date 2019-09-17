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

public class LoginActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        final DBHelper dbHelper = new DBHelper(getApplicationContext(), "member.db", null, 1);
        final EditText textID = (EditText) findViewById(R.id.input_ID);
        final EditText textPW = (EditText) findViewById(R.id.input_PW);

        Button btnLogIn = (Button) findViewById(R.id.btn_Login);
        Button btnSignUp = (Button) findViewById(R.id.btn_Signup);
        Button btnGo=(Button)findViewById(R.id.btn_goMAIN);
        TextView t_2=(TextView)findViewById(R.id.t2);
        TextView t_3=(TextView)findViewById(R.id.t3);
        Typeface type = Typeface.createFromAsset(getAssets(),"font/Binggrae-Bold.ttf");
        t_2.setTypeface(type);
        t_3.setTypeface(type);

        View.OnClickListener listener = new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                switch (view.getId()) {
                    case R.id.btn_Login:
                        String userID = textID.getText().toString();
                        String password = textPW.getText().toString();
                        if(dbHelper.login(userID,password)){
                            //main화면으로 가야함.
                            Intent goMain=new Intent(getApplicationContext(),MainActivity.class);
                            goMain.putExtra("ID", userID);
                            startActivity(goMain);
                            Toast.makeText(getApplication(), userID + "님 환영합니다.", Toast.LENGTH_SHORT).show();
                            finish();
                        }
                        else {
                            Toast.makeText(getApplication(), "ID,PW 다시 확인해주세요.", Toast.LENGTH_SHORT).show();
                            Intent goLog = new Intent(getApplicationContext(), LoginActivity.class);
                            startActivity(goLog);
                            finish();
                        }
                        break;
                    case R.id.btn_Signup:
                        //회원가입 페이지로 이동
                        Intent goSign = new Intent(getApplicationContext(), SignUpActivity.class);
                        startActivity(goSign);
                        break;
                    case R.id.btn_goMAIN:
                        Intent goMain = new Intent(getApplicationContext(), MainActivity.class);
                        startActivity(goMain);
                        break;
                }
            }
        };
        btnLogIn.setOnClickListener(listener);
        btnSignUp.setOnClickListener(listener);
        btnGo.setOnClickListener(listener);
    }
}
