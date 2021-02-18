package com.from_thunder_j.lianliankan.login;

import android.app.Activity;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.from_thunder_j.lianliankan.R;

public class Register_Activity extends Activity {
    private Button button;
    private Button btnCancel;
    private EditText account;
    private EditText password;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.register_activity);

        button = (Button)findViewById(R.id.btn_sure);
        account = (EditText)findViewById(R.id.username);
        password = (EditText)findViewById(R.id.password);
        btnCancel = (Button)findViewById(R.id.btn_cancel);

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                SharedPreferences.Editor editor = getSharedPreferences("data",MODE_PRIVATE).edit();

                editor.putString("name",account.getText().toString());
                editor.putString("password",password.getText().toString());

                SharedPreferences pre = getSharedPreferences("data",MODE_PRIVATE);
                String Account = account.getText().toString();
                if(Account.equals(pre.getString("name","")))
                {
                    Toast.makeText(Register_Activity.this,"账户名已经被注册!",Toast.LENGTH_SHORT).show();
                }
                else {
                    //提交数据
                    editor.commit();
                    Toast.makeText(Register_Activity.this,"注册成功!",Toast.LENGTH_SHORT).show();
                    finish();
                }
            }
        });
        btnCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(Register_Activity.this,"注册失败!",Toast.LENGTH_SHORT).show();
                finish();
            }
        });
    }
}
