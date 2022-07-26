package com.example.sprinklersystem;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    private String username,password;
    private Button login;
    private EditText editTextUsername,editTextPassword;
    private CheckBox rememberMeCheckBox;
    private SharedPreferences loginPreferences;
    private SharedPreferences.Editor loginPrefsEditor;
    private Boolean saveLogin;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        editTextUsername = (EditText)findViewById(R.id.editTextTextPassword);
        editTextPassword = (EditText)findViewById(R.id.editTextTextPassword2);
        rememberMeCheckBox = (CheckBox)findViewById(R.id.checkBox);
        loginPreferences = getSharedPreferences("loginPrefs", MODE_PRIVATE);
        loginPrefsEditor = loginPreferences.edit();

        saveLogin = loginPreferences.getBoolean("saveLogin", false);
        if (saveLogin == true) {
            editTextUsername.setText(loginPreferences.getString("username", ""));
            editTextPassword.setText(loginPreferences.getString("password", ""));
            rememberMeCheckBox.setChecked(true);
        }
        /*final EditText edittext = (EditText) findViewById(R.id.editTextTextPassword);
        edittext.setOnKeyListener(new View.OnKeyListener() {
            public boolean onKey(View v, int keyCode, KeyEvent event) {
                if ((event.getAction() == KeyEvent.ACTION_DOWN) && (keyCode == KeyEvent.KEYCODE_ENTER)) {
                    switchView();
                    return true;
                }
                return false;
            }
        });*/
    }

    public void onLogin(View view) {
        InputMethodManager imm = (InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.hideSoftInputFromWindow(editTextUsername.getWindowToken(), 0);
         username = editTextUsername.getText().toString();
         password = editTextPassword.getText().toString();
         if(rememberMeCheckBox.isChecked()){
             loginPrefsEditor.putBoolean("saveLogin", true);
             loginPrefsEditor.putString("username", username);
             loginPrefsEditor.putString("password", password);
             loginPrefsEditor.commit();
         }
         else{
             loginPrefsEditor.clear();
             loginPrefsEditor.commit();
         }
         switchView();
    }


    public void switchView() {
        Intent intent = new Intent(this, FirstPageActivity.class);
        startActivity(intent);
        overridePendingTransition(R.anim.slide_in_right, R.anim.slide_out_left);
    }
}