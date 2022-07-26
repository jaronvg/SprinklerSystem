package com.example.sprinklersystem;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.os.Handler;

import java.time.Clock;
import java.time.ZoneId;
import java.time.ZonedDateTime;

public class FirstPageActivity extends AppCompatActivity {
    Handler handler = new Handler();
    Runnable runnable;
    int delay = 1000;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_first_page);
        setTime();
    }

    @Override
    public void finish() {
        super.finish();
        overridePendingTransition(R.anim.slide_in_left, R.anim.slide_out_right);
    }

    public void setTime(){
        ZonedDateTime currTime = ZonedDateTime.now(ZoneId.of("US/Eastern"));
        String timeStr = String.valueOf(currTime);
        String date = "", time = "";
        date = timeStr.substring(5, 7) + "/" + timeStr.substring(8, 10) + "/" + timeStr.substring(0, 4);
        time = timeStr.substring(11, 13) + ":" + timeStr.substring(14, 16) + ":" + timeStr.substring(17, 19);
        TextView tvTime;
        tvTime = (TextView)findViewById(R.id.firstPageDate);
        tvTime.setText(date);
        tvTime = (TextView)findViewById(R.id.firstPageTime);
        tvTime.setText(time);
    }
    @Override
    protected void onResume() {
        handler.postDelayed(runnable = new Runnable() {
            public void run() {
                handler.postDelayed(runnable, delay);
                setTime();
            }
        }, delay);
        super.onResume();
    }
    @Override
    protected void onPause() {
        handler.removeCallbacks(runnable); //stop handler when activity not visible
        super.onPause();
    }
    public void switchToManual(View view) {
        Intent intent = new Intent(this, manualModeActivity.class);
        startActivity(intent);
        overridePendingTransition(R.anim.slide_in_right, R.anim.slide_out_left);
    }
    public void switchToProg(View view) {
        Intent intent = new Intent(this, progMode.class);
        startActivity(intent);
        overridePendingTransition(R.anim.slide_in_right, R.anim.slide_out_left);
    }
    public void switchToOff(View view) {
        Intent intent = new Intent(this, offMode.class);
        startActivity(intent);
        overridePendingTransition(R.anim.slide_in_right, R.anim.slide_out_left);
    }
    public void switchToSettings(View view) {
        Intent intent = new Intent(this, Settings.class);
        startActivity(intent);
        overridePendingTransition(R.anim.slide_in_right, R.anim.slide_out_left);
    }
}