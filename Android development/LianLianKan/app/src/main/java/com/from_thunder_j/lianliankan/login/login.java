package com.from_thunder_j.lianliankan.login;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.Toast;

import com.from_thunder_j.lianliankan.R;

public class login extends Activity {
    private SharedPreferences pref;
    private SharedPreferences.Editor editor;
    private EditText accountEdit,passwordEdit;
    private Button btnLogin;
    private Button btnCancel;
    private Button btnregister;
    private CheckBox rememberPwd;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.login);

        accountEdit = (EditText)findViewById(R.id.username);
        passwordEdit = (EditText)findViewById(R.id.password);
        rememberPwd = (CheckBox)findViewById(R.id.remember_pwd);
        btnLogin = (Button)findViewById(R.id.login);
        btnCancel = (Button)findViewById(R.id.cancel);
        btnregister = (Button)findViewById(R.id.register);

        //注册
        btnregister.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(login.this,Register_Activity.class);
                startActivity(intent);
            }
        });

        //记住密码
        pref = PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
        boolean isRemember = pref.getBoolean("remember_password",false);
        if(isRemember){
            //将账号密码都设置在文本框内
            String account = pref.getString("用户名","");
            String password = pref.getString("密码","");
            accountEdit.setText(account);
            passwordEdit.setText(password);
            rememberPwd.setChecked(true);
        }

        //登陆
        btnLogin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String account = accountEdit.getText().toString();
                String password = passwordEdit.getText().toString();

                //pre查询本地保存的账号和密码，并与用户输入的用户名和密码进行比较
                SharedPreferences pre = getSharedPreferences("data",MODE_PRIVATE);

                if(account.equals(pre.getString("name",""))&&password.equals(pre.getString("password",""))){
                    editor = pref.edit();
                    if(rememberPwd.isChecked()){
                        editor.putBoolean("remember_password",true);
                        editor.putString("用户名",account);
                        editor.putString("密码",password);
                    }
                    else{
                        editor.clear();
                    }
                    editor.apply();
                    Toast.makeText(login.this,"登陆成功!",Toast.LENGTH_SHORT).show();
                    //这里修改一下next.class跳转到自己想要跳转的地方就可以了
                    Intent intent = new Intent(login.this,next.class);

                    startActivity(intent);
                }
                else {
                    Toast.makeText(login.this,"登录失败!",Toast.LENGTH_SHORT).show();
                }
            }
        });
        btnCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });
    }
}
