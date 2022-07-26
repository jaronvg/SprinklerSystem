package com.example.sprinklersystem;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Switch;
import android.widget.TextView;

public class manualModeActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_manual_mode);
        EditText et1 = (EditText)findViewById(R.id.editTextTextPersonName);
        EditText et2 = (EditText)findViewById(R.id.editTextTextPersonName2);
        EditText et3 = (EditText)findViewById(R.id.editTextTextPersonName7);
        EditText et4 = (EditText)findViewById(R.id.editTextTextPersonName8);
        EditText et5 = (EditText)findViewById(R.id.editTextTextPersonName3);
        EditText et6 = (EditText)findViewById(R.id.editTextTextPersonName4);
        EditText et7 = (EditText)findViewById(R.id.editTextTextPersonName5);
        EditText et8 = (EditText)findViewById(R.id.editTextTextPersonName6);
        et1.setEnabled(false);
        et2.setEnabled(false);
        et3.setEnabled(false);
        et4.setEnabled(false);
        et5.setEnabled(false);
        et6.setEnabled(false);
        et7.setEnabled(false);
        et8.setEnabled(false);
        Switch s1 = (Switch)findViewById(R.id.switch2);
        Switch s2 = (Switch)findViewById(R.id.switch5);
        Switch s3 = (Switch)findViewById(R.id.switch6);
        Switch s4 = (Switch)findViewById(R.id.switch7);
        Switch s5 = (Switch)findViewById(R.id.switch4);
        Switch s6 = (Switch)findViewById(R.id.switch8);
        Switch s7 = (Switch)findViewById(R.id.switch9);
        Switch s8 = (Switch)findViewById(R.id.switch10);
        s1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked) {
                    et1.setEnabled(true);
                } else {
                    et1.setEnabled(false);
                }
            }
        });
        s2.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked) {
                    et2.setEnabled(true);
                } else {
                    et2.setEnabled(false);
                }
            }
        });
        s3.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked) {
                    et3.setEnabled(true);
                } else {
                    et3.setEnabled(false);
                }
            }
        });
        s4.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked) {
                    et4.setEnabled(true);
                } else {
                    et4.setEnabled(false);
                }
            }
        });
        s5.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked) {
                    et5.setEnabled(true);
                } else {
                    et5.setEnabled(false);
                }
            }
        });
        s6.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked) {
                    et6.setEnabled(true);
                } else {
                    et6.setEnabled(false);
                }
            }
        });
        s7.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked) {
                    et7.setEnabled(true);
                } else {
                    et7.setEnabled(false);
                }
            }
        });
        s8.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked) {
                    et8.setEnabled(true);
                } else {
                    et8.setEnabled(false);
                }
            }
        });
    }
    @Override
    public void finish() {
        super.finish();
        overridePendingTransition(R.anim.slide_in_left, R.anim.slide_out_right);
    }

    public void checkAll(View v){
        CheckBox ch = (CheckBox)v;
        Switch zoneOne = (Switch)findViewById(R.id.switch2);
        Switch zoneTwo = (Switch)findViewById(R.id.switch5);
        Switch zoneThree = (Switch)findViewById(R.id.switch6);
        Switch zoneFour = (Switch)findViewById(R.id.switch7);
        Switch zoneFive = (Switch)findViewById(R.id.switch4);
        Switch zoneSix = (Switch)findViewById(R.id.switch8);
        Switch zoneSeven = (Switch)findViewById(R.id.switch9);
        Switch zoneEight = (Switch)findViewById(R.id.switch10);
        EditText et1 = (EditText)findViewById(R.id.editTextTextPersonName);
        EditText et2 = (EditText)findViewById(R.id.editTextTextPersonName2);
        EditText et3 = (EditText)findViewById(R.id.editTextTextPersonName3);
        EditText et4 = (EditText)findViewById(R.id.editTextTextPersonName4);
        EditText et5 = (EditText)findViewById(R.id.editTextTextPersonName5);
        EditText et6 = (EditText)findViewById(R.id.editTextTextPersonName6);
        EditText et7 = (EditText)findViewById(R.id.editTextTextPersonName7);
        EditText et8 = (EditText)findViewById(R.id.editTextTextPersonName8);
        if(ch.isChecked()){
            zoneOne.setChecked(true);
            zoneTwo.setChecked(true);
            zoneThree.setChecked(true);
            zoneFour.setChecked(true);
            zoneFive.setChecked(true);
            zoneSix.setChecked(true);
            zoneSeven.setChecked(true);
            zoneEight.setChecked(true);
            et1.setEnabled(true);
            et2.setEnabled(true);
            et3.setEnabled(true);
            et4.setEnabled(true);
            et5.setEnabled(true);
            et6.setEnabled(true);
            et7.setEnabled(true);
            et8.setEnabled(true);
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
            et1.setEnabled(false);
            et2.setEnabled(false);
            et3.setEnabled(false);
            et4.setEnabled(false);
            et5.setEnabled(false);
            et6.setEnabled(false);
            et7.setEnabled(false);
            et8.setEnabled(false);
        }
    }

    public void switchToSettings(View view) {
        Intent intent = new Intent(this, Settings.class);
        startActivity(intent);
        overridePendingTransition(R.anim.slide_in_right, R.anim.slide_out_left);
    }
}