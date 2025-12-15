package io.securinets.level1;

import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.util.Base64;
import android.view.View;
import android.widget.EditText;
import android.widget.RelativeLayout;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatButton;

import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;

public class MainActivity extends AppCompatActivity {
    private boolean isTemplate = true;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        final RelativeLayout rootLayout = findViewById(R.id.rootLayout);
        final AppCompatButton loginButton = findViewById(R.id.loginButton);


        final EditText username = findViewById(R.id.username);
        final EditText password = findViewById(R.id.password);


        loginButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String user = username.getText().toString();
                String pass = password.getText().toString();
                int[] encpass = encrypt(user, pass);
                Resources res = getResources();
                int[] secrets = res.getIntArray(R.array.secrets);

                if (user.equals("logedin") && Arrays.equals(encpass, secrets)) {
                    Toast.makeText(MainActivity.this, "Login successful!", Toast.LENGTH_SHORT).show();
                    if (isTemplate) {
                        try {

                            InputStream is = getResources().openRawResource(R.raw.woho);
                            ByteArrayOutputStream baos = new ByteArrayOutputStream();
                            byte[] buffer = new byte[1024];
                            int read;
                            while ((read = is.read(buffer)) != -1) {
                                baos.write(buffer, 0, read);
                            }
                            is.close();

                            byte[] bytes = baos.toByteArray();

                            // Get SHA-256 hash as raw bytes (not hex string)
                            byte[] xorKey = chnoahetha(pass);

                            // XOR decrypt the image
                            for (int i = 0; i < bytes.length; i++) {
                                bytes[i] = (byte) (bytes[i] ^ xorKey[i % xorKey.length]);
                            }


                            Bitmap bitmap = BitmapFactory.decodeByteArray(bytes, 0, bytes.length);
                            if (bitmap != null) {
                                rootLayout.setBackground(new BitmapDrawable(getResources(), bitmap));
                            }

                        } catch (Exception e) {
                            e.printStackTrace();
                        }

                        isTemplate = false;
                    } else {

                        rootLayout.setBackgroundResource(R.drawable.template);
                        isTemplate = true;
                    }


                    loginButton.setVisibility(View.GONE);
                    username.setVisibility(View.GONE);
                    password.setVisibility(View.GONE);

                } else {
                    Toast.makeText(MainActivity.this, "Invalid credentials!", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }


    public static int[] encrypt(String login, String password) {
        try {
            byte[] key = login.getBytes("UTF-8");
            byte[] data = password.getBytes("UTF-8");

            // 1. XOR
            byte[] xored = new byte[data.length];
            for (int i = 0; i < data.length; i++) {
                xored[i] = (byte) (data[i] ^ key[i % key.length]);
            }

            // 2. Base64
            String b64 = Base64.encodeToString(xored, Base64.NO_WRAP);

            // 3. ROT10
            String rot = rot10(b64);

            // 4. Left bit shift (rotl)
            int[] encrypted = new int[rot.length()];
            for (int i = 0; i < rot.length(); i++) {
                encrypted[i] = rotl(rot.charAt(i), 1);
            }

            return encrypted;
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
            return new int[0];
        }
    }

    private static String rot10(String s) {
        StringBuilder out = new StringBuilder();
        for (char c : s.toCharArray()) {
            if (c >= 'a' && c <= 'z') {
                out.append((char) (((c - 'a' + 10) % 26) + 'a'));
            } else if (c >= 'A' && c <= 'Z') {
                out.append((char) (((c - 'A' + 10) % 26) + 'A'));
            } else if (c >= '0' && c <= '9') {
                out.append((char) (((c - '0' + 10) % 10) + '0'));
            } else {
                out.append(c);
            }
        }
        return out.toString();
    }

    private static int rotl(int bite, int shift) {
        return ((bite << shift) & 0xFF) | (bite >>> (8 - shift));
    }

    // Returns raw SHA-256 hash bytes (32 bytes)
    public static byte[] chnoahetha(String password) throws NoSuchAlgorithmException {
        MessageDigest digest = MessageDigest.getInstance("SHA-256");
        byte[] hashed = digest.digest(password.getBytes());
        return hashed;  // Return raw bytes, not hex string
    }

}