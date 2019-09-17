package com.example.leejiae.review;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.view.View;
import android.widget.Button;

/**
 * Created by Ji-Young on 2018-11-02.
 */

public class MyPickActivity extends Activity{
    Button btn;
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.mypick);
    btn = findViewById(R.id.mypick_back);
    btn.setOnClickListener(new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            Intent intent= new Intent(getApplicationContext(),MainActivity.class);
            startActivity(intent);
        }
    });

    }
}
