package com.example.leejiae.review;

import android.app.Activity;
import android.content.Intent;
import android.content.IntentSender;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.annotation.RequiresApi;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.TimePicker;


/**
 * Created by Ji-Young on 2018-11-02.
 */

public class SettingActivity extends Activity{

    Button btn,btnBack;
    TextView TV;
    TimePicker tp;
    Integer h,m;
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.setting);

        TV = findViewById(R.id.setting_text);
        btn = findViewById(R.id.btn_set);
        tp = findViewById(R.id.timepicker);

        btn.setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.M)
            @Override
            public void onClick(View v) {
                h = tp.getHour();
                m = tp.getMinute();
                TV.setText(h.toString() + "시 " + m.toString() + "분에 영업중인 곳만 표현");
            }
        });

        btnBack = findViewById(R.id.setting_back);

        btnBack.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent= new Intent(getApplicationContext(),MainActivity.class);
                intent.putExtra("time",h.toString() + "시 " + m.toString() + "분에 영업중인 곳만 표현");
                startActivity(intent);

            }
        });

    }
}
