package com.example.leejiae.review;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.os.SystemClock;
import android.support.annotation.Nullable;
import android.view.View;
import android.widget.Button;
import android.widget.Chronometer;
import android.widget.TextView;
import android.widget.ViewFlipper;

import org.w3c.dom.Text;

/**
 * Created by Ji-Young on 2018-10-28.
 */

public class WorldCupActivity extends Activity {
    Chronometer chrono;
    Button btn;
    Button btnBack;
    Integer count = 5;
    Integer worldCupCount = 0;
    String[] worldCup =  {"중식","한식","일식","양식","치킨"};
    String nowSelected = worldCup[0];
    TextView TVleft,TVright,TVresult;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.worldcup);
        chrono = findViewById(R.id.chrono);
        btn = findViewById(R.id.btn_worldcupStart);
        btnBack = findViewById(R.id.btn_worldcupBack);
        TVleft = findViewById(R.id.wordcup_left);
        TVright = findViewById(R.id.wordcup_right);
        TVresult =findViewById(R.id.worldcupResult);

        chrono.setText(count.toString());
        TVright.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                nowSelected = TVright.getText().toString();
                count = 0;
            }
        });

        TVleft.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                nowSelected = TVleft.getText().toString();
                count= 0;
            }
        });
        btnBack.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent= new Intent(getApplicationContext(),MainActivity.class);
                startActivity(intent);
                chrono.stop(); //메모리 릭 발생 방지.
            }
        });
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                chrono.start();
                chrono.setText(count.toString());
                TVleft.setText(worldCup[0]);
                TVright.setText(worldCup[1]);
                chrono.setOnChronometerTickListener(new Chronometer.OnChronometerTickListener() {
                    @Override
                    public void onChronometerTick(Chronometer chronometer)
                    {
                        chrono.setText(count.toString());
                        if(count > 0)
                        {
                            count--;
                        }
                        else
                        {
                            count = 5;
                            chrono.setText(count.toString());
                            if(worldCupCount >= worldCup.length - 1)
                            {
                                chrono.setOnChronometerTickListener(null);
                                TVresult.setText(nowSelected + "!!");
                                TVresult.setBackgroundColor(Color.rgb(255,0,0));
                                worldCupCount = 0;
                                chrono.stop();
                                chrono.setText(count.toString());
                                return;
                            }
                            TVleft.setText(nowSelected);
                            if(nowSelected.equals(worldCup[worldCupCount + 1]))
                                worldCupCount++;
                            TVright.setText(worldCup[worldCupCount + 1]);
                            worldCupCount++;
                        }
                    }
                });
            }
        });
    }
}
