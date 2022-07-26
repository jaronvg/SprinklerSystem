package com.example.sprinklersystem;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.NotificationCompat;
import androidx.core.app.NotificationManagerCompat;

import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.PopupWindow;
import android.widget.RelativeLayout;
import android.widget.Switch;
import android.widget.TextView;

public class progMode extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_prog_mode);
        EditText et1 = (EditText)findViewById(R.id.editTextTextPersonName12);
        EditText et2 = (EditText)findViewById(R.id.editTextTextPersonName13);
        EditText et3 = (EditText)findViewById(R.id.editTextTextPersonName14);
        EditText et4 = (EditText)findViewById(R.id.editTextTextPersonName15);
        EditText et5 = (EditText)findViewById(R.id.editTextTextPersonName16);
        EditText et6 = (EditText)findViewById(R.id.editTextTextPersonName17);
        EditText et7 = (EditText)findViewById(R.id.editTextTextPersonName18);
        EditText et8 = (EditText)findViewById(R.id.editTextTextPersonName19);
        Switch s1 = (Switch)findViewById(R.id.switch20);
        Switch s2 = (Switch)findViewById(R.id.switch21);
        Switch s3 = (Switch)findViewById(R.id.switch22);
        Switch s4 = (Switch)findViewById(R.id.switch23);
        Switch s5 = (Switch)findViewById(R.id.switch24);
        Switch s6 = (Switch)findViewById(R.id.switch25);
        Switch s7 = (Switch)findViewById(R.id.switch26);
        Switch s8 = (Switch)findViewById(R.id.switch27);
        TextView runText = findViewById(R.id.textView18);
        View div = findViewById(R.id.divider6);

        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.O){
            NotificationChannel channel = new NotificationChannel("Confirm Not", "Confirm Not", NotificationManager.IMPORTANCE_DEFAULT);
            NotificationManager manager = getSystemService(NotificationManager.class);
            manager.createNotificationChannel(channel);
        }

        s1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked) {
                    et1.setVisibility(View.VISIBLE);
                    runText.setVisibility(View.VISIBLE);
                    div.setVisibility(View.VISIBLE);
                } else {
                    et1.setVisibility(View.GONE);
                    runText.setVisibility(View.GONE);
                    div.setVisibility(View.GONE);
                }
            }
        });
        s2.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked) {
                    et2.setVisibility(View.VISIBLE);
                    runText.setVisibility(View.VISIBLE);
                    div.setVisibility(View.VISIBLE);
                } else {
                    et2.setVisibility(View.GONE);
                    runText.setVisibility(View.GONE);
                    div.setVisibility(View.GONE);
                }
            }
        });
        s3.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked) {
                    et3.setVisibility(View.VISIBLE);
                    runText.setVisibility(View.VISIBLE);
                    div.setVisibility(View.VISIBLE);
                } else {
                    et3.setVisibility(View.GONE);
                    runText.setVisibility(View.GONE);
                    div.setVisibility(View.GONE);
                }
            }
        });
        s4.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked) {
                    et4.setVisibility(View.VISIBLE);
                    runText.setVisibility(View.VISIBLE);
                    div.setVisibility(View.VISIBLE);
                } else {
                    et4.setVisibility(View.GONE);
                    runText.setVisibility(View.GONE);
                    div.setVisibility(View.GONE);
                }
            }
        });
        s5.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked) {
                    et5.setVisibility(View.VISIBLE);
                    runText.setVisibility(View.VISIBLE);
                    div.setVisibility(View.VISIBLE);
                } else {
                    et5.setVisibility(View.GONE);
                    runText.setVisibility(View.GONE);
                    div.setVisibility(View.GONE);
                }
            }
        });
        s6.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked) {
                    et6.setVisibility(View.VISIBLE);
                    runText.setVisibility(View.VISIBLE);
                    div.setVisibility(View.VISIBLE);
                } else {
                    et6.setVisibility(View.GONE);
                    runText.setVisibility(View.GONE);
                    div.setVisibility(View.GONE);
                }
            }
        });
        s7.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked) {
                    et7.setVisibility(View.VISIBLE);
                    runText.setVisibility(View.VISIBLE);
                    div.setVisibility(View.VISIBLE);
                } else {
                    et7.setVisibility(View.GONE);
                    runText.setVisibility(View.GONE);
                    div.setVisibility(View.GONE);
                }
            }
        });
        s8.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked) {
                    et8.setVisibility(View.VISIBLE);
                    runText.setVisibility(View.VISIBLE);
                    div.setVisibility(View.VISIBLE);
                } else {
                    et8.setVisibility(View.GONE);
                    runText.setVisibility(View.GONE);
                    div.setVisibility(View.GONE);
                }
            }
        });
    }
    @Override
    public void finish() {
        super.finish();
        overridePendingTransition(R.anim.slide_in_left, R.anim.slide_out_right);
    }
    public void switchToSettings(View view) {
        Intent intent = new Intent(this, Settings.class);
        startActivity(intent);
        overridePendingTransition(R.anim.slide_in_right, R.anim.slide_out_left);
    }

    public void createProg(View v){
        CheckBox ch = (CheckBox)v;
        View b = findViewById(R.id.progScoll);
        if(ch.isChecked()) {
            b.setVisibility(View.VISIBLE);
        }
        else{
            b.setVisibility(View.GONE);
        }
    }

    public void checkForRuntime(View v){
        LinearLayout linearLayout = findViewById(R.id.scrollLayout);
        Switch zone1 = findViewById(R.id.switch20);
        Switch zone2 = findViewById(R.id.switch21);
        Switch zone3 = findViewById(R.id.switch22);
        Switch zone4 = findViewById(R.id.switch23);
        Switch zone5 = findViewById(R.id.switch24);
        Switch zone6 = findViewById(R.id.switch25);
        Switch zone7 = findViewById(R.id.switch26);
        Switch zone8 = findViewById(R.id.switch27);

        Switch currSwitch = (Switch)v;
        TextView runText = findViewById(R.id.textView18);
        View div = findViewById(R.id.divider6);
        EditText editText = findViewById(R.id.editTextTextPersonName12);
        if(currSwitch.isChecked()){
            if(currSwitch.getId() == zone1.getId()){
                editText = findViewById(R.id.editTextTextPersonName12);
            }
            else if(currSwitch.getId() == zone2.getId()){
                editText = findViewById(R.id.editTextTextPersonName13);
            }
            else if(currSwitch.getId() == zone3.getId()){
                editText = findViewById(R.id.editTextTextPersonName14);
            }
            else if(currSwitch.getId() == zone4.getId()){
                editText = findViewById(R.id.editTextTextPersonName15);
            }
            else if(currSwitch.getId() == zone5.getId()){
                editText = findViewById(R.id.editTextTextPersonName16);
            }
            else if(currSwitch.getId() == zone6.getId()){
                editText = findViewById(R.id.editTextTextPersonName17);
            }
            else if(currSwitch.getId() == zone7.getId()){
                editText = findViewById(R.id.editTextTextPersonName18);
            }
            else if(currSwitch.getId() == zone8.getId()){
                editText = findViewById(R.id.editTextTextPersonName19);
            }
            editText.setVisibility(View.VISIBLE);
            runText.setVisibility(View.VISIBLE);
            div.setVisibility(View.VISIBLE);
        }
        else{
            if(currSwitch.getId() == zone1.getId()){
                editText = findViewById(R.id.editTextTextPersonName12);
            }
            else if(currSwitch.getId() == zone2.getId()){
                editText = findViewById(R.id.editTextTextPersonName13);
            }
            else if(currSwitch.getId() == zone3.getId()){
                editText = findViewById(R.id.editTextTextPersonName14);
            }
            else if(currSwitch.getId() == zone4.getId()){
                editText = findViewById(R.id.editTextTextPersonName15);
            }
            else if(currSwitch.getId() == zone5.getId()){
                editText = findViewById(R.id.editTextTextPersonName16);
            }
            else if(currSwitch.getId() == zone6.getId()){
                editText = findViewById(R.id.editTextTextPersonName17);
            }
            else if(currSwitch.getId() == zone7.getId()){
                editText = findViewById(R.id.editTextTextPersonName18);
            }
            else if(currSwitch.getId() == zone8.getId()){
                editText = findViewById(R.id.editTextTextPersonName19);
            }
            editText.setVisibility(View.GONE);
            runText.setVisibility(View.GONE);
            div.setVisibility(View.GONE);
        }
    }

    public void checkAll(View v){
        CheckBox ch = (CheckBox)v;
        Switch zoneOne = (Switch)findViewById(R.id.switch20);
        Switch zoneTwo = (Switch)findViewById(R.id.switch21);
        Switch zoneThree = (Switch)findViewById(R.id.switch22);
        Switch zoneFour = (Switch)findViewById(R.id.switch23);
        Switch zoneFive = (Switch)findViewById(R.id.switch24);
        Switch zoneSix = (Switch)findViewById(R.id.switch25);
        Switch zoneSeven = (Switch)findViewById(R.id.switch26);
        Switch zoneEight = (Switch)findViewById(R.id.switch27);
        EditText et1 = (EditText)findViewById(R.id.editTextTextPersonName12);
        EditText et2 = (EditText)findViewById(R.id.editTextTextPersonName13);
        EditText et3 = (EditText)findViewById(R.id.editTextTextPersonName14);
        EditText et4 = (EditText)findViewById(R.id.editTextTextPersonName15);
        EditText et5 = (EditText)findViewById(R.id.editTextTextPersonName16);
        EditText et6 = (EditText)findViewById(R.id.editTextTextPersonName17);
        EditText et7 = (EditText)findViewById(R.id.editTextTextPersonName18);
        EditText et8 = (EditText)findViewById(R.id.editTextTextPersonName19);
        TextView runText = findViewById(R.id.textView18);
        View div = findViewById(R.id.divider6);
        if(ch.isChecked()){
            zoneOne.setChecked(true);
            zoneTwo.setChecked(true);
            zoneThree.setChecked(true);
            zoneFour.setChecked(true);
            zoneFive.setChecked(true);
            zoneSix.setChecked(true);
            zoneSeven.setChecked(true);
            zoneEight.setChecked(true);
            et1.setVisibility(View.VISIBLE);
            et2.setVisibility(View.VISIBLE);
            et3.setVisibility(View.VISIBLE);
            et4.setVisibility(View.VISIBLE);
            et5.setVisibility(View.VISIBLE);
            et6.setVisibility(View.VISIBLE);
            et7.setVisibility(View.VISIBLE);
            et8.setVisibility(View.VISIBLE);
            runText.setVisibility(View.VISIBLE);
            div.setVisibility(View.VISIBLE);
        }
        else{
            zoneOne.setChecked(false);
            zoneTwo.setChecked(false);
            zoneThree.setChecked(false);
            zoneFour.setChecked(false);
            zoneFive.setChecked(false);
            zoneSix.setChecked(false);
            zoneSeven.setChecked(false);
            zoneEight.setChecked(false);
            et1.setVisibility(View.GONE);
            et2.setVisibility(View.GONE);
            et3.setVisibility(View.GONE);
            et4.setVisibility(View.GONE);
            et5.setVisibility(View.GONE);
            et6.setVisibility(View.GONE);
            et7.setVisibility(View.GONE);
            et8.setVisibility(View.GONE);
            runText.setVisibility(View.GONE);
            div.setVisibility(View.GONE);
        }
    }

    public void confirmProg(View v){
        int progNumber, startTime;
        int currZones[] = new int[8];
        int currDays[] = new int[7];
        int currRuntimes[] = new int[8];
        Switch zone1 = (Switch)findViewById(R.id.switch20);
        Switch zone2 = (Switch)findViewById(R.id.switch21);
        Switch zone3 = (Switch)findViewById(R.id.switch22);
        Switch zone4 = (Switch)findViewById(R.id.switch23);
        Switch zone5 = (Switch)findViewById(R.id.switch24);
        Switch zone6 = (Switch)findViewById(R.id.switch25);
        Switch zone7 = (Switch)findViewById(R.id.switch26);
        Switch zone8 = (Switch)findViewById(R.id.switch27);

        Switch sunday = (Switch)findViewById(R.id.switch29);
        Switch monday = (Switch)findViewById(R.id.switch30);
        Switch tuesday = (Switch)findViewById(R.id.switch31);
        Switch wednesday = (Switch)findViewById(R.id.switch32);
        Switch thursday = (Switch)findViewById(R.id.switch33);
        Switch friday = (Switch)findViewById(R.id.switch34);
        Switch saturday = (Switch)findViewById(R.id.switch35);

        EditText progNum = (EditText)findViewById(R.id.editTextTextPersonName10);
        EditText stTime = (EditText)findViewById(R.id.editTextTextPersonName11);

        EditText zone1Runtime = (EditText)findViewById(R.id.editTextTextPersonName12);
        EditText zone2Runtime = (EditText)findViewById(R.id.editTextTextPersonName13);
        EditText zone3Runtime = (EditText)findViewById(R.id.editTextTextPersonName14);
        EditText zone4Runtime = (EditText)findViewById(R.id.editTextTextPersonName15);
        EditText zone5Runtime = (EditText)findViewById(R.id.editTextTextPersonName16);
        EditText zone6Runtime = (EditText)findViewById(R.id.editTextTextPersonName17);
        EditText zone7Runtime = (EditText)findViewById(R.id.editTextTextPersonName18);
        EditText zone8Runtime = (EditText)findViewById(R.id.editTextTextPersonName19);

        NotificationCompat.Builder builder = new NotificationCompat.Builder(progMode.this, "Confirm Not");
        builder.setContentTitle("Title")
                .setContentText("Content Text")
                .setSmallIcon(R.drawable.ecoadjusted)
                .setAutoCancel(true);

        NotificationManagerCompat managerCompat = NotificationManagerCompat.from(progMode.this);
        managerCompat.notify(1, builder.build());

        LayoutInflater inflater = (LayoutInflater)
                getSystemService(LAYOUT_INFLATER_SERVICE);
        View popupView = inflater.inflate(R.layout.confirm_popup, null);

        // create the popup window
        int width = LinearLayout.LayoutParams.WRAP_CONTENT;
        int height = LinearLayout.LayoutParams.WRAP_CONTENT;
        boolean focusable = true; // lets taps outside the popup also dismiss it
        final PopupWindow popupWindow = new PopupWindow(popupView, width, height, focusable);

        // show the popup window
        popupWindow.showAtLocation(v, Gravity.CENTER, 0, 0);

        // dismiss the popup window when touched
        popupView.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                popupWindow.dismiss();
                return true;
            }
        });

        if(zone1.isChecked()){
            currZones[0] = 1;
        }
        else{
            currZones[0] = 0;
        }
        if(zone2.isChecked()){
            currZones[1] = 1;
        }
        else{
            currZones[1] = 0;
        }
        if(zone3.isChecked()){
            currZones[2] = 1;
        }
        else{
            currZones[2] = 0;
        }
        if(zone4.isChecked()){
            currZones[3] = 1;
        }
        else{
            currZones[3] = 0;
        }
        if(zone5.isChecked()){
            currZones[4] = 1;
        }
        else{
            currZones[4] = 0;
        }
        if(zone6.isChecked()){
            currZones[5] = 1;
        }
        else{
            currZones[5] = 0;
        }
        if(zone7.isChecked()){
            currZones[6] = 1;
        }
        else{
            currZones[6] = 0;
        }
        if(zone8.isChecked()){
            currZones[7] = 1;
        }
        else{
            currZones[7] = 0;
        }

        if(sunday.isChecked()){
            currDays[0] = 1;
        }
        else{
            currDays[0] = 0;
        }
        if(monday.isChecked()){
            currDays[1] = 1;
        }
        else{
            currDays[1] = 0;
        }
        if(tuesday.isChecked()){
            currDays[2] = 1;
        }
        else{
            currDays[2] = 0;
        }
        if(wednesday.isChecked()){
            currDays[3] = 1;
        }
        else{
            currDays[3] = 0;
        }
        if(thursday.isChecked()){
            currDays[4] = 1;
        }
        else{
            currDays[4] = 0;
        }
        if(friday.isChecked()){
            currDays[5] = 1;
        }
        else{
            currDays[5] = 0;
        }
        if(saturday.isChecked()){
            currDays[6] = 1;
        }
        else{
            currDays[6] = 0;
        }

        String tempTime = stTime.getText().toString();
        if(!tempTime.isEmpty()){
            startTime = Integer.parseInt(tempTime);
        }
        String tempRuntime;
        tempRuntime = zone1Runtime.getText().toString();
        if(!tempRuntime.isEmpty()){
            currRuntimes[0] = Integer.parseInt(tempRuntime);
        }
        tempRuntime = zone2Runtime.getText().toString();
        if(!tempRuntime.isEmpty()){
            currRuntimes[1] = Integer.parseInt(tempRuntime);
        }
        tempRuntime = zone3Runtime.getText().toString();
        if(!tempRuntime.isEmpty()){
            currRuntimes[2] = Integer.parseInt(tempRuntime);
        }
        tempRuntime = zone4Runtime.getText().toString();
        if(!tempRuntime.isEmpty()){
            currRuntimes[3] = Integer.parseInt(tempRuntime);
        }
        tempRuntime = zone5Runtime.getText().toString();
        if(!tempRuntime.isEmpty()){
            currRuntimes[4] = Integer.parseInt(tempRuntime);
        }
        tempRuntime = zone6Runtime.getText().toString();
        if(!tempRuntime.isEmpty()){
            currRuntimes[5] = Integer.parseInt(tempRuntime);
        }
        tempRuntime = zone7Runtime.getText().toString();
        if(!tempRuntime.isEmpty()){
            currRuntimes[6] = Integer.parseInt(tempRuntime);
        }
        tempRuntime = zone8Runtime.getText().toString();
        if(!tempRuntime.isEmpty()){
            currRuntimes[7] = Integer.parseInt(tempRuntime);
        }
    }
}