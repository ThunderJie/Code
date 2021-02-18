package com.from_thunder_j.lianliankan.login;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

import com.from_thunder_j.lianliankan.R;

import java.util.Timer;
import java.util.TimerTask;

public class Login_Activity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.login_activity);

        //实现自动跳转，注意是继承自Activity，要跳转的页面不能继承此页面
        Timer timer=new Timer();
        TimerTask timerTask=new TimerTask() {
            @Override
            public void run() {
                //通过Intent实现跳转
                startActivity(new Intent(Login_Activity.this,login.class));
                Login_Activity.this.finish();
            }
        };
        //延迟2秒后跳转，注意单位是毫秒
        timer.schedule(timerTask,2000);
    }
}
