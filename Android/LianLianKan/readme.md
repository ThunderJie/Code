# 0x00:前言
本篇主要介绍用Android Studio实现游戏登陆界面，自动跳转，代码和资源文件我会上传到GitHub上，需要的可以自行下载。登陆界面功能包括记住密码，注册等。
## 下载链接：
https://github.com/ThunderJie/Code/tree/master/Android%20development/LianLianKan
# 0x01:实现过程
## 一.实现开始欢迎界面自动跳转至登陆界面
1.	实现效果：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190123103419674.gif)

2.	实现原理：
首先我们创建四个继承自Activity的类Login_Activity.class，login.class，Register_Activity.class，next.class我们在AndroidManifest.xml中将游戏的图标和名字进行基本的一些设置，再将标题框隐藏。
```xml
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.from_thunder_j.lianliankan">

    <application
        android:allowBackup="true"
        android:icon="@drawable/icon"
        android:label="@string/app_name"
        android:roundIcon="@mipmap/ic_launcher_round"
        android:supportsRtl="true"
        android:theme="@style/AppTheme">
        <activity android:name=".login.Login_Activity"
            android:theme="@android:style/Theme.NoTitleBar.Fullscreen">
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />

                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
        <activity android:name=".login.login"
            android:theme="@android:style/Theme.NoTitleBar.Fullscreen"/>
        <activity android:name=".login.Register_Activity"
            android:theme="@android:style/Theme.NoTitleBar.Fullscreen"></activity>
        <activity android:name=".login.next"
            android:theme="@android:style/Theme.NoTitleBar.Fullscreen"></activity>
    </application>
</manifest>
```

我们通过TimerTask来实现2秒之后自动从欢迎界面跳转到登陆界面,下面是Login_Activity.class的代码。
```java
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

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
```

下面是login_activity.xml的代码，实现自动跳转的布局文件
```xml
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout
    xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:background="@drawable/frist_bg" >

    <ImageView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:background="@drawable/game_title" />

</LinearLayout>
```

Tips：Timer和TimerTask的区别
1）Timer是一种定时器工具，用来在一个后台线程计划执行指定任务。它可以计划执行一个任务一次或反复多次。
2）TimerTask一个抽象类，它的子类代表一个可以被Timer计划的任务。
## 二.实现登陆界面

1. 实现效果
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190129111740535.gif)

2. 实现原理
login.java文件使用SharePreferences实现登陆界面，登陆成功后我是跳转到next.class，根据需要修改为自己的游戏界面就可以了，下面是实现内容：
```java
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
```

login.xml文件使用线性布局，内容如下
```xml
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout
    xmlns:android="http://schemas.android.com/apk/res/android" android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:background="@drawable/frist_bg"
    android:orientation="vertical">

    <TextView
        android:id="@+id/textView"
        android:layout_width="wrap_content"
        android:layout_height="50dp"
        android:text="@string/username"
        android:textSize="20sp"
        android:textColor="#000"/>

    <EditText
        android:id="@+id/username"
        android:layout_width="350dp"
        android:layout_height="wrap_content"
        android:hint="@string/input_hint"
        android:maxLines="1"
        android:layout_gravity="center"
        />

    <TextView
        android:id="@+id/textView2"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="@string/password"
        android:textSize="20sp"
        android:textColor="#000"/>

    <EditText
        android:id="@+id/password"
        android:layout_width="350dp"
        android:layout_height="wrap_content"
        android:hint="@string/input_hint_passwd"
        android:maxLines="1"
        android:inputType="textPassword"
        android:layout_gravity="center"/>
    <LinearLayout
        android:layout_width="wrap_content"
        android:layout_height="wrap_content">
        <CheckBox
            android:id="@+id/remember_pwd"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content" />

        <TextView
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:text="@string/remember_pwd"
            android:textSize="18sp" />
    </LinearLayout>
    <LinearLayout
        android:layout_width="wrap_content"
        android:layout_height="wrap_content">
        <Button
            android:id="@+id/login"
            android:layout_width="100dp"
            android:layout_height="wrap_content"
            android:text="@string/button_sure"
            />

        <Button
            android:id="@+id/cancel"
            android:layout_width="100dp"
            android:layout_height="wrap_content"
            android:text="@string/btn_cancel" />
    </LinearLayout>
    <Button
        android:id="@+id/register"
        android:layout_width="100dp"
        android:layout_height="wrap_content"
        android:layout_gravity="end"
        android:text="@string/register"/>

</LinearLayout>
```

## 三.实现注册界面
Register_Activity.java文件主要实现注册内容，还是用了SharePreferences方法
```java
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
```

布局文件register_activity.xml还是线性布局
```xml
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:background="@drawable/frist_bg"
    android:orientation="vertical"
    >
    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="请输入您想要注册的用户名和密码"
        android:textSize="20sp"
        android:textColor="#000"
        />
    <TableLayout
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:stretchColumns="1">
        <TableRow>
            <TextView
                android:layout_height="wrap_content"
                android:text="@string/username"
                android:textSize="20sp"
                android:textColor="#000"
                />
            <EditText
                android:id="@+id/username"
                android:layout_height="wrap_content"
                android:hint="@string/input_hint"
                />
        </TableRow>
        <TableRow>
            <TextView
                android:layout_height="wrap_content"
                android:text="@string/password"
                android:textSize="20sp"
                android:textColor="#000"
                />
            <EditText
                android:id="@+id/password"
                android:layout_height="wrap_content"
                android:inputType="textPassword"
                android:hint="@string/input_hint_passwd"
                />
        </TableRow>
    </TableLayout>
    <LinearLayout
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:orientation="horizontal">
        <Button
            android:id="@+id/btn_sure"
            android:layout_width="match_parent"
            android:layout_height="wrap_content"
            android:text="@string/button_sure"
            android:textSize="20sp"/>
        <Button
            android:id="@+id/btn_cancel"
            android:layout_width="match_parent"
            android:layout_height="wrap_content"
            android:text="@string/btn_cancel"
            android:textSize="20sp"
            />

    </LinearLayout>

</LinearLayout>
```

引用到的字符串文件：
```xml
<resources>
    <string name="app_name">LianLianKan</string>
    <string name="username">用户名:</string>
    <string name="input_hint">请输入用户名</string>
    <string name="password">密码:</string>
    <string name="input_hint_passwd">请输入密码</string>
    <string name="remember_pwd">记住密码</string>
    <string name="button_sure">确定</string>
    <string name="btn_cancel">取消</string>
    <string name="register">注册</string>
</resources>
```
# 0x02:总结
代码还有很多地方可以优化，还可以判断登陆时是否存在用户名等，这只是一个大概的模板，需要的自行修改，代码可能有些地方写的不专业，希望大家多多包涵。